#include <ds_state.h>
#include <movement_system.h>

#define V_COUNT 512

#ifndef UI_MAX_VERTEX_MEM
#define UI_MAX_VERTEX_MEM 1024 * 512
#endif

#ifndef UI_MAX_ELEMENT_MEM
#define UI_MAX_ELEMENT_MEM 1024 * 128
#endif

class TestTask : public fluff::ecs::Task<TestTask>
{
	size_t Updates;
public:
	void Update() override
	{
		Updates++;
	}
};

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
	cam.GetComponent<gfx::CameraComponent>()->SetPosition(glm::vec3(0, 120, -200));
	cam.GetComponent<gfx::CameraComponent>()->SetRotation(glm::vec3(-45, 0, 0));

	pManager_->GetSystemManager()->Add<render::RenderSystem>(pManager_, cam, proj);
	pManager_->GetSystemManager()->Add<debug::DebugSystem>();
	pManager_->GetSystemManager()->Add<ecs::Dependency<render::RenderableComponent, TransformationComponent>>();
	pManager_->GetSystemManager()->Add<MovementSystem>(cam);
	pManager_->GetSystemManager()->Add<render::ScreenshotSystem>(pManager_);
	pManager_->GetSystemManager()->Add<physics::PhysicsSystem>(pManager_);
	pManager_->GetSystemManager()->Add<ecs::TaskSystem>(pManager_->GetSystemManager()->GetThreadPool());
	pManager_->GetSystemManager()->Configure();

	debug::DebugMessage(pManager_, debug::DebugErrorType::ILLEGAL_DATA, debug::DebugSeverity::WARN, __LINE__, __FILE__, "Test!");

	pManager_->GetSystemManager()->GetSystem<ecs::TaskSystem>()->Add<TestTask>();

	Window::GetCurrentWindow()->GetWidth().SetListener([&](uint32_t old_val, uint32_t new_val) {
		if (!new_val) return;
		auto mat = glm::perspective(70.0f, (float)new_val / (float)Window::GetCurrentWindow()->GetHeight().GetValue(), 0.2f, 5000.0f);
		pManager_->GetSystemManager()->GetSystem<render::RenderSystem>()->GetSceneRenderer()->SetProjectionMatrix(mat);
	});

	Window::GetCurrentWindow()->GetHeight().SetListener([&](uint32_t old_val, uint32_t new_val) {
		if (!new_val) return;
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
	noise.SetSeed(9832);
	noise.SetFrequency(0.01f);
	noise.SetInterp(FastNoise::Hermite);
	noise.SetFractalType(FastNoise::Billow);
	noise.SetFractalOctaves(8);
	noise.SetFractalLacunarity(.5f);
	noise.SetFractalGain(0.01f);
	noise.SetGradientPerturbAmp(2);
	noise.SetGradientPerturbFrequency(0.15f);


	auto cube_mesh = new gfx::Mesh;
	ReadFromFile("cube.asset", *cube_mesh);
	cube_mesh->Name = "cube";

	auto mod3 = gfx::Context::LoadMesh(cube_mesh);

	std::vector<unsigned char*> data_array;

	render::ResourceLibrary::GetInstance().LocationLibrary.AssetTypes.insert(std::make_pair("pine.asset", render::LIBRARY_TEXTURE));
	SendToFile("resource_library.asset", render::ResourceLibrary::GetInstance().LocationLibrary);

	glm::mat4 modl = glm::mat4(1.0);
	Transformation * t = new Transformation(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0.5));
	Transformation * t1 = new Transformation(glm::vec3(-2, 0, -2), glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0.5));
	Transformation * t2 = new Transformation(glm::vec3(5, 1.5, 5), glm::vec3(45, 45, 0), glm::vec3(1, 1, 1));

	auto * mat	= new render::Material;
	auto * mat2 = new render::Material;
	auto * mat4 = new render::Material;

	ReadFromFile("material.asset", *mat);
	ReadFromFile("material_2.asset", *mat2);
	ReadFromFile("material_4.asset", *mat4);

	glm::vec3 scale(1, 1, 1);

	gfx::Context::Flush(pManager_);

	std::vector<std::future<void>> tasks;

	tasks.push_back(pManager_->GetSystemManager()->GetThreadPool()->PushTask([&](size_t) {
		for (auto x = -6; x < 5; x++)
		{
			for (auto z = -6; z < 5; z++)
			{
				Transformation trans(glm::vec3(x * V_COUNT - 1, 0, z * V_COUNT - 1), glm::vec3(0, 0, 0), glm::vec3(0.5f, 1, 0.5f));

				auto * ter = new render::Terrain(V_COUNT + 2, mat4, &trans, noise, 48, 1);

				trans.SetPosition(trans.GetPosition() / 2.0f);

				physics::HeightFieldDesc hfd;
				hfd.ColumnScale = 0.5f;
				hfd.RowScale = 0.5f;
				hfd.NumCols = V_COUNT + 3;
				hfd.NumRows = V_COUNT + 3;
				hfd.pManager = pManager_->GetSystemManager()->GetSystem<physics::PhysicsSystem>()->GetManager();
				hfd.HeightScale = 64;

				hfd.HeightMap = static_cast<float*>(calloc(sizeof(float) * hfd.NumCols * hfd.NumRows, 1));
				memcpy(hfd.HeightMap, ter->GetHeightMap(), sizeof(float) * (V_COUNT + 3) * (V_COUNT + 3));

				auto e = pManager_->GetEntityManager()->Create();

				physics::PhysicsMaterialDescriptor mat_desc;
				mat_desc.DynamicFriction = 0.5f;
				mat_desc.StaticFriction = 0.5f;
				mat_desc.Restitution = 0.5f;

				pManager_->GetEntityManager()->AddComponent<render::RenderableComponent>(e.GetID(), *ter);
				e.GetComponent<TransformationComponent>()->SetPosition(trans.GetPosition());
				e.GetComponent<TransformationComponent>()->SetScale(trans.GetScale());

				e.AddComponent<physics::PhysicsComponent>(physics::STATIC, physics::HEIGHTFIELD, &hfd, e.GetComponent<TransformationComponent>().Get(), mat_desc, pManager_->GetSystemManager()->GetSystem<physics::PhysicsSystem>().get(), 1.0f);

				pManager_->GetSystemManager()->GetSystem<ecs::TaskSystem>()->AddEntity<TestTask>(e);

				delete ter;
			}
		}
	}));

	for (auto & task : tasks)
	{
		task.get();
	}

	gfx::Context::Flush(pManager_);

	float ter_scale = V_COUNT / 64.0f;

	gfx::GraphicsPipeline::SetDefault();

	Allocator::Dispose();

	render::Renderable renderable(mat, mod3);
	render::Renderable renderable2(mat, mod3);
	render::Renderable renderable3(mat2, mod3);
	
	for (auto i = -10; i < 5; i++)
	{
		for (auto j = -10; j < 5; j++)
		{
			auto ent = pManager_->GetEntityManager()->Create();
			ent.AddComponent<render::RenderableComponent>(renderable2);
			ent.GetComponent<TransformationComponent>()->SetPosition(glm::vec3(i * 15 + 1, 65, j * 15 + 1));
			ent.GetComponent<TransformationComponent>()->SetScale(glm::vec3(1, 1, 1));

			auto new_rot = ent.GetComponent<TransformationComponent>()->GetRotation();

			ent.GetComponent<render::RenderableComponent>()->renderables[0].GetMaterial()->SetTransparent(false);

			auto physics_system = pManager_->GetSystemManager()->GetSystem<physics::PhysicsSystem>();

			physics::BoxDesc desc;
			desc.HalfWidth = 1.0f;
			desc.HalfHeight = 1.0f;
			desc.HalfDepth = 1.0f;


			physics::PhysicsMaterialDescriptor mat_desc;
			mat_desc.DynamicFriction = 0.5f;
			mat_desc.StaticFriction = 0.5f;
			mat_desc.Restitution = 0.5f;

			ent.AddComponent<physics::PhysicsComponent>(physics::DYNAMIC, physics::BOX, &desc, ent.GetComponent<TransformationComponent>().Get(), mat_desc, physics_system.get(), 1.0f);
		}
	}

	pManager_->GetEventManager()->EmitEvent<gfx::ModelSubmittedEvent>();
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

	pManager_->GetSystemManager()->UpdateAll(0);
}

void DSState::Run()
{
	std::cout << Timer::FPS() << std::endl;
	if (Keyboard::IsKeyPressed(KEY_0))
	{
		render::Screenshot(this->GetManager());
	}
}

void DSState::Shutdown()
{
	TextureLibrary::Clear();
	PipelineLibrary::Clear();
}

bool DSState::ShouldShutdown() 
{
	return Keyboard::IsKeyPressed(KEY_ESCAPE);
}
