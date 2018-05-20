#include "../includes/ds_state.h"
#include "../includes/movement_system.h"

#include <Physx/extensions/PxRigidBodyExt.h>

#define V_COUNT 16

void DSState::Configure()
{
	std::ifstream lt_in("lookup_table.bin", std::ios::binary);
	cereal::BinaryInputArchive archive_in(lt_in);

	{
		ComponentLookupTable table;
		archive_in(table);
	}

	lt_in.close();

	PipelineLibrary::LoadLibrary("pipelines.asset");
	PipelineLibrary::Create(pManager_);
	TextureLibrary::LoadLibrary("textures.asset");

	glm::mat4 proj = glm::perspective(70.0f, 16.0f / 9.0f, 0.2f, 5000.0f);

	auto cam = pManager_->GetEntityManager()->Create();
	cam.AddComponent<gfx::CameraComponent>();
	cam.GetComponent<gfx::CameraComponent>()->SetPosition(glm::vec3(0, 50, 0));
	cam.GetComponent<gfx::CameraComponent>()->SetDirection(glm::vec3(0, 0, 1));

	pManager_->GetSystemManager()->Add<render::RenderSystem>(pManager_, cam, proj);
	pManager_->GetSystemManager()->Add<DebugSystem>();
	pManager_->GetSystemManager()->Add<ecs::Dependency<render::RenderableComponent, TransformationComponent>>();
	pManager_->GetSystemManager()->Add<MovementSystem>(cam);
	pManager_->GetSystemManager()->Add<render::ScreenshotSystem>(pManager_);
	pManager_->GetSystemManager()->Add<physics::PhysicsSystem>(pManager_);
	pManager_->GetSystemManager()->Configure();

	pManager_->GetSystemManager()->UpdateAll(0);

	Window::GetCurrentWindow()->GetWidth().SetListener([&](uint32_t old_val, uint32_t new_val) {
		auto mat = glm::perspective(70.0f, (float)new_val / (float)Window::GetCurrentWindow()->GetHeight().GetValue(), 0.2f, 5000.0f);
		pManager_->GetSystemManager()->GetSystem<render::RenderSystem>()->GetSceneRenderer()->SetProjectionMatrix(mat);
	});

	Window::GetCurrentWindow()->GetHeight().SetListener([&](uint32_t old_val, uint32_t new_val) {
		auto mat = glm::perspective(70.0f, (float)Window::GetCurrentWindow()->GetWidth().GetValue() / (float)new_val, 0.2f, 5000.0f);
		pManager_->GetSystemManager()->GetSystem<render::RenderSystem>()->GetSceneRenderer()->SetProjectionMatrix(mat);
	});

	gfx::TextureCreateInfo create_info =
	{
		gfx::FILTER_MIN_LINEAR_MIPMAP_LINEAR_MAG_LINEAR,
		gfx::RGBA8,
	{
		true,
		-0.6f,
		2.0f
	},
		0,
		0,
		0,
		8,
		-0.6f,
		-50.0f,
		5.0f
	};

	FastNoise noise;
	noise.SetNoiseType(FastNoise::PerlinFractal);
	noise.SetSeed(1337);
	noise.SetFrequency(0.01);
	noise.SetInterp(FastNoise::Hermite);
	noise.SetFractalType(FastNoise::FBM);
	noise.SetFractalOctaves(8);
	noise.SetFractalLacunarity(.5);
	noise.SetFractalGain(0.1);
	noise.SetGradientPerturbAmp(2);
	noise.SetGradientPerturbFrequency(0.15);


	auto cube_mesh = new gfx::Mesh;
	ReadFromFile("cube.asset", *cube_mesh);
	cube_mesh->Name = "cube";

	auto mod3 = gfx::Context::LoadMesh(cube_mesh);

	int bpp;
	unsigned char * data;

	std::vector<unsigned char*> data_array;

	render::ResourceLibrary::GetInstance().LocationLibrary.AssetTypes.insert(std::make_pair("pine.asset", render::LIBRARY_TEXTURE));
	SendToFile("resource_library.asset", render::ResourceLibrary::GetInstance().LocationLibrary);

	glm::mat4 modl = glm::mat4(1.0);
	Transformation * t = new Transformation(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0.5));
	Transformation * t1 = new Transformation(glm::vec3(-2, 0, -2), glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0.5));
	Transformation * t2 = new Transformation(glm::vec3(5, 1.5, 5), glm::vec3(45, 45, 0), glm::vec3(1, 1, 1));
	glClearColor(0, 0, 0, 1);

	auto * mat	= new render::Material;
	auto * mat2 = new render::Material;
	auto * mat4 = new render::Material;

	ReadFromFile("material.asset", *mat);
	ReadFromFile("material_2.asset", *mat2);
	ReadFromFile("material_4.asset", *mat4);

	cam.GetComponent<gfx::CameraComponent>()->SetPosition(glm::vec3(0, 0, 0));
	cam.GetComponent<gfx::CameraComponent>()->SetDirection(glm::vec3(0, 0, 1));

	glm::vec3 scale(1, 1, 1);

	gfx::Context::Flush(pManager_);

	for (auto x = -2; x < 2; x++)
	{
		for (auto z = -2; z < 2; z++)
		{
			Transformation trans(glm::vec3(x * V_COUNT, 0, z * V_COUNT), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

//			auto * ter = new render::Terrain(V_COUNT, mat4, &trans, noise, 4, 1);
			auto * ter = render::Terrain::GenerateFlatTerrain(V_COUNT, mat4, &trans);

			physics::HeightFieldDesc hfd;
			hfd.ColumnScale = 1;
			hfd.RowScale = 1;
			hfd.NumCols = V_COUNT + 1;
			hfd.NumRows = V_COUNT + 1;
			hfd.pManager = pManager_->GetSystemManager()->GetSystem<physics::PhysicsSystem>()->GetManager();
			hfd.HeightScale = 1;

			hfd.HeightMap = static_cast<int16_t*>(calloc(sizeof(int16_t) * hfd.NumCols * hfd.NumRows, 1));
			memcpy(hfd.HeightMap, ter->GetHeightMap(), sizeof(int16_t) * (V_COUNT + 1) * (V_COUNT + 1));

			auto e = pManager_->GetEntityManager()->Create();

			physics::PhysicsMaterialDescriptor mat_desc;
			mat_desc.DynamicFriction = 0.5f;
			mat_desc.StaticFriction = 0.5f;
			mat_desc.Restitution = 0.5f;

			pManager_->GetEntityManager()->AddComponent<render::RenderableComponent>(e.GetID(), *ter);
			e.GetComponent<TransformationComponent>()->SetPosition(trans.GetPosition());
			e.GetComponent<TransformationComponent>()->SetScale(trans.GetScale());

			e.AddComponent<physics::PhysicsComponent>(physics::STATIC, physics::HEIGHTFIELD, &hfd, e.GetComponent<TransformationComponent>().Get(), mat_desc, pManager_->GetSystemManager()->GetSystem<physics::PhysicsSystem>().get(), 1.0f);
			e.GetComponent<TransformationComponent>()->SetScale(e.GetComponent<TransformationComponent>()->GetScale());

			delete ter;
		}
	}

	gfx::Context::Flush(pManager_);

	float ter_scale = V_COUNT / 64.0f;

	gfx::GraphicsPipeline::SetDefault();

	Allocator::Dispose();

	render::Renderable renderable(mat, mod3);
	render::Renderable renderable2(mat, mod3);
	render::Renderable renderable3(mat2, mod3);

	auto e = pManager_->GetEntityManager()->Create();
	e.AddComponent<render::RenderableComponent>(renderable);
	auto comp = e.GetComponent<TransformationComponent>().Get();
	comp->SetPosition(glm::vec3(1, 42, 3));
	comp->SetRotation(glm::vec3(0, 0, 0));

	e2 = pManager_->GetEntityManager()->Create();
	e2.AddComponent<render::RenderableComponent>(renderable2);
	e2.GetComponent<TransformationComponent>()->SetPosition(glm::vec3(0, 65, 0));
	
	for (auto i = 0; i < 3; i++)
	{
		e3 = pManager_->GetEntityManager()->Create();
		e3.AddComponent<render::RenderableComponent>(renderable3);
		e3.GetComponent<TransformationComponent>()->SetPosition(glm::vec3(i * 5 + 5, 5, 10));

		auto new_rot = e3.GetComponent<TransformationComponent>()->GetRotation();

		e3.GetComponent<render::RenderableComponent>()->renderables[0].GetMaterial()->SetTransparent(true);

		auto physics_system = pManager_->GetSystemManager()->GetSystem<physics::PhysicsSystem>();

		physics::BoxDesc desc;
		desc.HalfWidth = 1.0f;
		desc.HalfHeight = 1.0f;
		desc.HalfDepth = 1.0f;

		physics::PlaneDesc p_desc;

		physics::BoxDesc * desc2 = new physics::BoxDesc;
		desc2->HalfDepth = 1;
		desc2->HalfHeight = 1;
		desc2->HalfWidth = 1;

		physics::PhysicsMaterialDescriptor mat_desc;
		mat_desc.DynamicFriction = 0.5f;
		mat_desc.StaticFriction = 0.5f;
		mat_desc.Restitution = 0.5f;

		physics::PhysicsMaterialDescriptor mat_desc2;
		mat_desc2.DynamicFriction = 0.5f;
		mat_desc2.StaticFriction = 0.5f;
		mat_desc2.Restitution = 1.0f;

		e3.AddComponent<physics::PhysicsComponent>(physics::DYNAMIC, physics::BOX, &desc, e3.GetComponent<TransformationComponent>().Get(), mat_desc, physics_system.get(), 1.0f);

		delete desc2;
	}

	pManager_->GetEventManager()->EmitEvent<gfx::ModelSubmittedEvent>();

	ui::UILayer layer;
	widget = new ui::DebugWidget;
	layer.Widgets.push_back(widget);
	layer.Enable(pManager_);

	pManager_->GetSystemManager()->UpdateAll(0);
	gfx::Context::Clear();

	auto directional_light = pManager_->GetEntityManager()->Create();
	directional_light.AddComponent<render::DirectionalLightComponent>(gfx::DirectionalLight{
		render::Vec3{ 0, -1, 0 },
		render::Vec3{ .2f, .2f, .2f },
		render::Vec3{ .6f, .6f, .6f },
		render::Vec3{ .2f, .2f, .2f }
	});

	auto point_light = pManager_->GetEntityManager()->Create();
	point_light.AddComponent<render::PointLightComponent>(gfx::PointLight{
		render::Vec3{ 350, 100, 25 },
		render::Vec3{ .2f, .2f, .2f },
		render::Vec3{ .6f, .6f, .6f },
		render::Vec3{ .2f, .2f, .2f },
		render::Vec3{ 0.0f, 0.01f, 0.0f }
	});

	auto point_light_2 = pManager_->GetEntityManager()->Create();
	point_light_2.AddComponent<render::PointLightComponent>(gfx::PointLight{
		render::Vec3{ 50, 100, 25 },
		render::Vec3{ .2f, .2f, .2f },
		render::Vec3{ .6f, .6f, .6f },
		render::Vec3{ .2f, .2f, .2f },
		render::Vec3{ 0.0f, 0.01f, 0.0f }
	});
}

void DSState::Run()
{
	if (Keyboard::IsKeyPressed(KEY_1))
	{
		((physics::PhysicsDynamicActor *) e3.GetComponent<physics::PhysicsComponent>()->GetActor())->ApplyForce(glm::vec3(0, 50.0, 0));
	}
}

void DSState::Shutdown()
{
	TextureLibrary::Clear();
	PipelineLibrary::Clear();

	delete mat;
	delete mat2;
	delete mat4;

	delete widget;
}
