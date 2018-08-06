#include <rendering/scene_renderer.h>
#include <core/window.h>
#include <core/debug/debug_message.h>
#include <gfx/context.h>
#include <rendering/resource_library.h>
#include <rendering/lighting_manager.h>
#include <rendering/light_components.h>
#include <core/ecs/exclusive.h>
#include <gfx/camera_component.h>
#include <core/assets/material_library.h>
#include <glew.h>
#include <physics/physics_component.h>

namespace fluff { namespace render {

	SceneRenderer::SceneRenderer(std::shared_ptr<ECSManager> & Manager, glm::mat4& ProjectionMatrix)
		: Manager_(Manager)
	{
		this->ProjectionMatrix_ = ProjectionMatrix;
		ShaderBufferDesc viewproj_desc;
		viewproj_desc.ByteWidth = sizeof(ViewProj);
		viewproj_desc.ReadWrite = MAP_WRITE;
		viewproj_desc.Usage = MAP_COHERENT;
		viewproj_desc.AccessFlags = 0;
		viewproj_desc.MiscFlags = 0;
		viewproj_desc.StructureByteStride = 0;
		this->ViewProjBuffer_ = new gfx::ShaderBuffer(viewproj_desc);
		Context::UpdateSubResource(ViewProjBuffer_, &ProjectionMatrix_, 0, 256);

		ShaderBufferDesc lighting_desc;
		lighting_desc.ByteWidth = sizeof(SceneLighting);
		lighting_desc.ReadWrite = MAP_WRITE;
		lighting_desc.Usage = MAP_COHERENT;
		lighting_desc.AccessFlags = 0;
		lighting_desc.MiscFlags = 0;
		lighting_desc.StructureByteStride = 0;
		this->LightingBuffer_ = new gfx::ShaderBuffer(lighting_desc);

		Manager_->GetSystemManager()->Add<ecs::Exclusive<DirectionalLightComponent, PointLightComponent, SpotLightComponent>>();
		Manager_->GetSystemManager()->Add<ecs::Exclusive<PointLightComponent, DirectionalLightComponent, SpotLightComponent>>();
		Manager_->GetSystemManager()->Add<ecs::Exclusive<SpotLightComponent, DirectionalLightComponent, PointLightComponent>>();
	
		DeferredPass_ = new gfx::FrameBuffer(Manager_, Window::GetCurrentWindow()->GetWidth().GetValue(), Window::GetCurrentWindow()->GetHeight().GetValue());
		PBuffer_ = new gfx::FrameBuffer(Manager_, Window::GetCurrentWindow()->GetWidth().GetValue(), Window::GetCurrentWindow()->GetHeight().GetValue());

		Window::GetCurrentWindow()->GetBufferWidth().SetListener([&](uint32_t old_value, uint32_t new_value) {
			if (new_value)
			{
				DeferredPass_->Bind();
				DeferredPass_->Resize(new_value, DeferredPass_->Height());
				PBuffer_->Bind();
				PBuffer_->Resize(new_value, PBuffer_->Height());
				PBuffer_->Unbind();
			}
		});

		Window::GetCurrentWindow()->GetBufferHeight().SetListener([&](uint32_t old_value, uint32_t new_value) {
			if (new_value)
			{
				DeferredPass_->Bind();
				DeferredPass_->Resize(DeferredPass_->Width(), new_value);
				PBuffer_->Bind();
				PBuffer_->Resize(PBuffer_->Width(), new_value);
				PBuffer_->Unbind();
			}
		});

	
		DeferredPass_->Bind();
		DeferredPass_->AddTexture();
		DeferredPass_->AddTexture();
		DeferredPass_->AddTexture();
		DeferredPass_->AddTexture();
		int32_t depth = DeferredPass_->AddDepthTexture();

		if (!DeferredPass_->IsValid())
		{
			debug::DebugMessage(Manager_, debug::DebugErrorType::ILLEGAL_STATE, debug::DebugSeverity::FATAL, static_cast<size_t>(__LINE__), std::string(__FILE__), "Deferred pass framebuffer could not be created.");
		}

		PBuffer_->Bind();
		PBuffer_->AddTexture();
		PBuffer_->AddDepthTexture();

		static std::vector<unsigned int> indices = { 2, 1, 0, 0, 3, 2 };

		static std::vector<float32_t> positions = {
			 1,  1, 0,
			 1, -1, 0,
			-1, -1, 0,
			-1,  1, 0
		};

		static std::vector<float32_t> uvs = {
			1, 1, 0,
			1, 0, 0,
			0, 0, 0,
			0, 1, 0
		};

		static std::vector<float32_t> normals = {
			0, 0, 1,
			0, 0, 1,
			0, 0, 1,
			0, 0, 1
		};

		Mesh * mesh = new Mesh(positions, uvs, normals, indices);
		DeferredModel_ = Context::LoadMesh(mesh);
		Context::Flush(Manager_);

		DeferredMaterial_ = new Material(PipelineLibrary::Get("deferred_pipeline"));
		DeferredMaterial_->GetShader()->Bind();
		DeferredMaterial_->CreateTextureUniform(TEXTURE2D, DeferredPass_->GetTextureIDs()[0], 0, "position");
		DeferredMaterial_->CreateTextureUniform(TEXTURE2D, DeferredPass_->GetTextureIDs()[1], 1, "normal");
		DeferredMaterial_->CreateTextureUniform(TEXTURE2D, DeferredPass_->GetTextureIDs()[2], 2, "diffuse");
		DeferredMaterial_->CreateTextureUniform(TEXTURE2D, DeferredPass_->GetTextureIDs()[3], 3, "specular");
		DeferredMaterial_->CreateTextureUniform(TEXTURE2D, DeferredPass_->GetDepthID(), 4, "depth_map");

		float light_dir[3] = { 0, 1, 0.5 };
		camera_position = DeferredMaterial_->CreateUniform(VEC3, "camera_position", nullptr).get();

		DeferredMaterial_->GetShader()->Unbind();
		Manager_->GetSystemManager()->Configure();
	}

	SceneRenderer::SceneRenderer(std::shared_ptr<ECSManager> & Manager, ecs::Entity Cam, glm::mat4& ProjectionMatrix) : SceneRenderer(Manager, ProjectionMatrix)
	{
		this->Cam = std::make_shared<ecs::Entity>(Cam);
	}

	SceneRenderer::~SceneRenderer()
	{
		delete ViewProjBuffer_;
		delete LightingBuffer_;
	}

	void SceneRenderer::Configure(std::shared_ptr<ecs::EventManager> & Events)
	{
		Events->SubscribeToEvent<ecs::EntityDestroyedEvent>(*this);
		Events->SubscribeToEvent<ecs::ComponentRemovedEvent<RenderableComponent>>(*this);
		Events->SubscribeToEvent<ModelSubmittedEvent>(*this);
	}

	void SceneRenderer::Receive(const ModelSubmittedEvent & ModelSubmission)
	{
		std::unordered_map<VertexBuffer *, std::unordered_map<GraphicsPipeline *, std::unordered_map<Material *, std::vector<ecs::ID>>>> sort_map;
		std::unordered_map<VertexBuffer *, std::unordered_map<GraphicsPipeline *, std::unordered_map<Material *, std::vector<ecs::ID>>>> transparent_sort_map;
		
		StateChanges_.clear();
		Entities_.clear();
		RenderableIDs_.clear();

		TEntities_.clear();
		TStateChanges_.clear();
		TRenderableIDs_.clear();

		auto manager = Manager_->GetEntityManager();

		for (auto & ent : Manager_->GetEntityManager()->GetEntitiesWithComponents<RenderableComponent>())
		{
			auto id = ent.GetID();
			auto & ent = manager->Get(id);
			size_t renderable_id = 0;

			for (auto renderable : ent.GetComponent<RenderableComponent>()->renderables)
			{
				if (renderable.GetMaterial()->IsTransparent())
				{
					TRenderableIDs_.push_back(renderable_id++);
					auto modl = transparent_sort_map.find(renderable.GetModel()->Buffer);
					if (modl == transparent_sort_map.end())
					{
						std::unordered_map<GraphicsPipeline*, std::unordered_map<Material *, std::vector<ecs::ID>>> material_sort_map;
						std::unordered_map<Material *, std::vector<ecs::ID>> mats;
						std::vector<ecs::ID> ids = { id };
						mats.insert(std::make_pair(renderable.GetMaterial(), ids));
						material_sort_map.insert(std::make_pair(renderable.GetMaterial()->GetPipeline(), mats));
						transparent_sort_map.insert(std::make_pair(renderable.GetModel()->Buffer, material_sort_map));
					}
					else
					{
						auto pipe = modl->second.find(renderable.GetMaterial()->GetPipeline());
						if (pipe == modl->second.end())
						{
							std::unordered_map<Material *, std::vector<ecs::ID>> mats;
							std::vector<ecs::ID> ids = { id };
							mats.insert(std::make_pair(renderable.GetMaterial(), ids));
							modl->second.insert(std::make_pair(renderable.GetMaterial()->GetPipeline(), mats));
						}
						else
						{
							auto matl = pipe->second.find(renderable.GetMaterial());
							if (matl == pipe->second.end())
							{
								std::vector<ecs::ID> ids = { id };
								pipe->second.insert(std::make_pair(renderable.GetMaterial(), ids));
							}
							else
							{
								matl->second.push_back(id);
							}
						}
					}
				}
				else
				{
					RenderableIDs_.push_back(renderable_id++);
					auto modl = sort_map.find(renderable.GetModel()->Buffer);
					if (modl == sort_map.end())
					{
						std::unordered_map<GraphicsPipeline*, std::unordered_map<Material *, std::vector<ecs::ID>>> material_sort_map;
						std::unordered_map<Material *, std::vector<ecs::ID>> mats;
						std::vector<ecs::ID> ids = { id };
						mats.insert(std::make_pair(renderable.GetMaterial(), ids));
						material_sort_map.insert(std::make_pair(renderable.GetMaterial()->GetPipeline(), mats));
						sort_map.insert(std::make_pair(renderable.GetModel()->Buffer, material_sort_map));
					}
					else
					{
						auto pipe = modl->second.find(renderable.GetMaterial()->GetPipeline());
						if (pipe == modl->second.end())
						{
							std::unordered_map<Material *, std::vector<ecs::ID>> mats;
							std::vector<ecs::ID> ids = { id };
							mats.insert(std::make_pair(renderable.GetMaterial(), ids));
							modl->second.insert(std::make_pair(renderable.GetMaterial()->GetPipeline(), mats));
						}
						else
						{
							auto matl = pipe->second.find(renderable.GetMaterial());
							if (matl == pipe->second.end())
							{
								std::vector<ecs::ID> ids = { id };
								pipe->second.insert(std::make_pair(renderable.GetMaterial(), ids));
							}
							else
							{
								matl->second.push_back(id);
							}
						}
					}
				}

			}
		}

		size_t counter = 0;
		for (auto model : sort_map)
		{
			if (counter == StateChanges_.size()) StateChanges_.push_back(MODL);
			for (auto pipeline : model.second)
			{
				if (counter == StateChanges_.size()) StateChanges_.push_back(PIPE);
				for (auto & material : pipeline.second)
				{
					if (counter == StateChanges_.size()) StateChanges_.push_back(MATL);
					for (auto & instance : material.second)
					{
						Entities_.push_back(std::move(instance));
						if (counter == StateChanges_.size()) StateChanges_.push_back(NONE);
						counter++;
					}
				}
			}
		}
		FLUFF_ASSERT(Entities_.size() == StateChanges_.size())

		counter = 0;
		for (auto model : transparent_sort_map)
		{
			if (counter == TStateChanges_.size()) TStateChanges_.push_back(MODL);
			for (auto pipeline : model.second)
			{
				if (counter == TStateChanges_.size()) TStateChanges_.push_back(PIPE);
				for (auto & material : pipeline.second)
				{
					if (counter == TStateChanges_.size()) TStateChanges_.push_back(MATL);
					for (auto & instance : material.second)
					{
						TEntities_.push_back(std::move(instance));
						if (counter == TStateChanges_.size()) TStateChanges_.push_back(NONE);
						counter++;
					}
				}
			}
		}
		FLUFF_ASSERT(TEntities_.size() == TStateChanges_.size())
	}

	void SceneRenderer::Receive(const ecs::EntityDestroyedEvent & EntityDestroyed)
	{
		
	}

	void SceneRenderer::Receive(const ecs::ComponentRemovedEvent<RenderableComponent> & RenderableComponent)
	{
		auto id = RenderableComponent.E.GetID();
		for (auto & i : Entities_)
		{
			if (i == id) i = ecs::Entity::INVALID;
		}
		for (auto & i : TEntities_)
		{
			if (i == id) i = ecs::Entity::INVALID;
		}
	}

	void SceneRenderer::Render()
	{
		ViewProjMatrix_.ViewMatrix = Cam->GetComponent<gfx::CameraComponent>()->GetViewMatrix();
		ViewProjMatrix_.ProjectionMatrix = ProjectionMatrix_;
		Context::UpdateSubResource(ViewProjBuffer_, &ViewProjMatrix_, 0, 256);
		Context::BindRange(ViewProjBuffer_, 0, sizeof(ViewProj), 0);

		camera_position->SetValue(glm::value_ptr(Cam->GetComponent<gfx::CameraComponent>()->GetPosition()));
		DeferredPass();
		LightingPass();
		TransparencyPass();
		PBuffer_->DrawToScreen(Window::GetCurrentWindow()->GetWidth().GetValue(), Window::GetCurrentWindow()->GetHeight().GetValue());
	}

	void SceneRenderer::DeferredPass()
	{
		static uint32_t DrawBuffers_[4] = { 0, 1, 2, 3 };
		DeferredPass_->Bind();
		glViewport(0, 0, DeferredPass_->Width(), DeferredPass_->Height());
		Context::Clear();
		DeferredPass_->SetDrawBuffers(DrawBuffers_, 4);
		for (auto i = 0; i < Entities_.size(); i++)
		{
			auto & id = Entities_[i];
			if (!Manager_->GetEntityManager()->IsValid(id)) continue;
			auto & ent = Manager_->GetEntityManager()->Get(id);
			auto & state = StateChanges_[i];

			auto modl = ent.GetComponent<RenderableComponent>()->renderables[RenderableIDs_[i]].GetModel();
			auto matl = ent.GetComponent<RenderableComponent>()->renderables[RenderableIDs_[i]].GetMaterial();

			switch (state)
			{
			case MODL:
			{
				modl->Buffer->Bind();
			}
			case PIPE:
			{
				GraphicsPipeline::SetDefault();
				matl->GetPipeline()->Enable();
				matl->GetShader()->Bind();
			}
			case MATL:
			{
				matl->Update();
			}
			case NONE:
			{
				auto actor = ent.GetComponent<physics::PhysicsComponent>()->GetActor();
				if (actor) {
					auto transform = actor->GetTransform();
					matl->SetModelMatrix(const_cast<float*>(transform));
					modl->Draw();
				}
				else
				{
					FLUFF_LOG(fluff::debug::DebugErrorType::ILLEGAL_STATE, fluff::debug::DebugSeverity::WARN, "No physics component added.");
				}
			}
			default: break;
			}
		}
	}

	void SceneRenderer::LightingPass()
	{
		auto lighting = ClosestLights(SceneLights_.LightCounts);
		for (auto i : Manager_->GetEntityManager()->GetEntitiesWithComponents<DirectionalLightComponent>())
		{
			SceneLights_.D_Light = i.GetComponent<DirectionalLightComponent>()->Light;
			break;
		}

		uint8_t spot = 0, point = 0;
		for (auto id : lighting)
		{
			if (Manager_->GetEntityManager()->HasComponent<SpotLightComponent>(id)) 
				SceneLights_.S_Lights[spot++] = Manager_->GetEntityManager()->GetComponentPtr<SpotLightComponent>(id)->Light;

			if (Manager_->GetEntityManager()->HasComponent<PointLightComponent>(id))
				SceneLights_.P_Lights[point++] = Manager_->GetEntityManager()->GetComponentPtr<PointLightComponent>(id)->Light;
		}

		Context::UpdateSubResource(LightingBuffer_, &SceneLights_, 0, sizeof(SceneLighting));
		Context::BindRange(LightingBuffer_, 0, sizeof(SceneLighting), 1);
		PBuffer_->Bind();
		Context::Clear();
		GraphicsPipeline::SetDefault();
		DeferredMaterial_->GetPipeline()->Enable();
		DeferredMaterial_->GetShader()->Bind();
		DeferredMaterial_->Update();
		DeferredModel_->Buffer->Bind();
		DeferredModel_->Draw();
	}

	void SceneRenderer::TransparencyPass()
	{
		std::sort(TEntities_.begin(), TEntities_.end(), [&](ecs::ID left, ecs::ID right) {
			auto l = Manager_->GetEntityManager()->Get(left);
			auto r = Manager_->GetEntityManager()->Get(right);

			auto d_l = glm::length2(l.GetComponent<TransformationComponent>()->GetPosition() - Cam->GetComponent<gfx::CameraComponent>()->GetPosition());
			auto d_r = glm::length2(r.GetComponent<TransformationComponent>()->GetPosition() - Cam->GetComponent<gfx::CameraComponent>()->GetPosition());
			return d_l > d_r;
		});

		PBuffer_->Bind();

		for (auto i = 0; i < TEntities_.size(); i++)
		{
			auto & id = TEntities_[i];
			if (!Manager_->GetEntityManager()->IsValid(id)) continue;
			auto & ent = Manager_->GetEntityManager()->Get(id);
			auto & state = TStateChanges_[i];

			auto modl = ent.GetComponent<RenderableComponent>()->renderables[TRenderableIDs_[i]].GetModel();
			auto matl = ent.GetComponent<RenderableComponent>()->renderables[TRenderableIDs_[i]].GetMaterial();

			switch (state)
			{
			case MODL:
			{
				modl->Buffer->Bind();
			}
			case PIPE:
			{
				GraphicsPipeline::SetDefault();
				matl->GetPipeline()->Enable();
				matl->GetShader()->Bind();
			}
			case MATL:
			{
				matl->Update();
			}
			case NONE:
			{
				auto transformation = ent.GetComponent<TransformationComponent>()->GetTransformationMatrix();
				matl->SetModelMatrix(const_cast<float*>(glm::value_ptr(transformation)));
				modl->Draw();
			}
			default: break;
			}
		}

		PBuffer_->Unbind(Window::GetCurrentWindow()->GetWidth().GetValue(), Window::GetCurrentWindow()->GetHeight().GetValue());
	}

	std::vector<ecs::ID> SceneRenderer::ClosestLights(IVec3 & LightCount)
	{
		std::vector<ecs::ID> point_lights;
		std::vector<ecs::ID> spot_lights;
		for (auto light : Manager_->GetEntityManager()->GetEntitiesWithComponents<PointLightComponent>())
		{
			point_lights.push_back(light.GetID());
		}

		for (auto light : Manager_->GetEntityManager()->GetEntitiesWithComponents<SpotLightComponent>())
		{
			spot_lights.push_back(light.GetID());
		}

		std::sort(point_lights.begin(), point_lights.end(), [&](ecs::ID left, ecs::ID right)
		{
			Vec3 LPosition = Manager_->GetEntityManager()->GetComponentPtr<PointLightComponent>(left)->Light.Position;
			Vec3 RPosition = Manager_->GetEntityManager()->GetComponentPtr<PointLightComponent>(left)->Light.Position;

			return (Cam->GetComponent<gfx::CameraComponent>()->GetPosition().x * LPosition.Value[0] + Cam->GetComponent<gfx::CameraComponent>()->GetPosition().y * LPosition.Value[1] + Cam->GetComponent<gfx::CameraComponent>()->GetPosition().z * LPosition.Value[2]) <
				(Cam->GetComponent<gfx::CameraComponent>()->GetPosition().x * RPosition.Value[0] + Cam->GetComponent<gfx::CameraComponent>()->GetPosition().y * RPosition.Value[1] + Cam->GetComponent<gfx::CameraComponent>()->GetPosition().z * RPosition.Value[2]);
		});

		std::sort(spot_lights.begin(), spot_lights.end(), [&](ecs::ID left, ecs::ID right)
		{
			Vec3 LPosition = Manager_->GetEntityManager()->GetComponentPtr<SpotLightComponent>(left)->Light.Position;
			Vec3 RPosition = Manager_->GetEntityManager()->GetComponentPtr<SpotLightComponent>(left)->Light.Position;

			return (Cam->GetComponent<gfx::CameraComponent>()->GetPosition().x * LPosition.Value[0] + Cam->GetComponent<gfx::CameraComponent>()->GetPosition().y * LPosition.Value[1] + Cam->GetComponent<gfx::CameraComponent>()->GetPosition().z * LPosition.Value[2]) <
				(Cam->GetComponent<gfx::CameraComponent>()->GetPosition().x * RPosition.Value[0] + Cam->GetComponent<gfx::CameraComponent>()->GetPosition().y * RPosition.Value[1] + Cam->GetComponent<gfx::CameraComponent>()->GetPosition().z * RPosition.Value[2]);
		});

		std::vector<ecs::ID> return_values;
		for (auto i = 0; i < MAX_LIGHTS_SCENE; i++)
		{
			if (i < point_lights.size()) return_values.push_back(point_lights[i]);
			else
			{
				break;
			}
		}

		for (auto i = 0; i < MAX_LIGHTS_SCENE; i++)
		{
			if (i < spot_lights.size()) return_values.push_back(spot_lights[i]);
			else
			{
				break;
			}
		}

		LightCount.Value[0] = 0;
		for (auto light : Manager_->GetEntityManager()->GetEntitiesWithComponents<DirectionalLightComponent>())
		{
			LightCount.Value[0] = 1;
			break;
		}

		LightCount.Value[1] = static_cast<int32_t>(point_lights.size());
		LightCount.Value[2] = static_cast<int32_t>(spot_lights.size());

		return return_values;
	}

} }