#include <ds_state.h>
#include <movement_system.h>
#include <sphere_generators.h>

#include <iostream>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

#define V_COUNT 256

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
	cam.GetComponent<gfx::CameraComponent>()->SetPosition(glm::vec3(0, 100, 0));
	cam.GetComponent<gfx::CameraComponent>()->SetRotation(glm::vec3(-45, 0, 0));

	pManager_->GetSystemManager()->Add<render::RenderSystem>(pManager_, cam, proj);
	pManager_->GetSystemManager()->Add<debug::DebugSystem>();
	pManager_->GetSystemManager()->Add<ecs::Dependency<render::RenderableComponent, TransformationComponent>>();
	pManager_->GetSystemManager()->Add<MovementSystem>(cam);
	pManager_->GetSystemManager()->Add<render::ScreenshotSystem>(pManager_);
	pManager_->GetSystemManager()->Add<physics::PhysicsSystem>(pManager_);
	auto taskSys = pManager_->GetSystemManager()->Add<ecs::TaskSystem>();
	taskSys->AttachThreadpool(pManager_->GetSystemManager()->GetThreadPool());
	pManager_->GetSystemManager()->Configure();

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
		for (auto x = -1; x < 2; x++)
		{
			for (auto z = -1; z < 2; z++)
			{
				Transformation trans(glm::vec3((x * V_COUNT) - V_COUNT / 2, 0, (z * V_COUNT) - V_COUNT / 2), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

				auto * ter = new render::Terrain(V_COUNT, mat4, &trans, noise, 48, 1);

				trans.SetPosition(trans.GetPosition());

				physics::HeightFieldDesc hfd;
				hfd.ColumnScale = 1.0f;
				hfd.RowScale = 1.0f;
				hfd.NumCols = V_COUNT + 1;
				hfd.NumRows = V_COUNT + 1;
				hfd.pManager = pManager_->GetSystemManager()->GetSystem<physics::PhysicsSystem>()->GetManager();
				hfd.HeightScale = 48;

				hfd.HeightMap = static_cast<float*>(calloc(sizeof(float) * hfd.NumCols * hfd.NumRows, 1));
				memcpy(hfd.HeightMap, ter->GetHeightMap(), sizeof(float) * (V_COUNT + 1) * (V_COUNT + 1));

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
	Allocator::Dispose();

	for (size_t i = 0; i < 3; i++)
	{
		sandbox::Icosphere generator;
		auto icosphere = generator.GetMesh(i);
		gfx::Model * sphere = gfx::Context::LoadMesh(icosphere);

		ecs::Entity ent = pManager_->GetEntityManager()->Create();
		render::Renderable renderable(mat4, sphere);
		ent.AddComponent<render::RenderableComponent>(renderable);

		auto physics_system = pManager_->GetSystemManager()->GetSystem<physics::PhysicsSystem>();

		physics::BoxDesc desc;
		desc.HalfWidth = 1.0f;
		desc.HalfHeight = 1.0f;
		desc.HalfDepth = 1.0f;


		physics::PhysicsMaterialDescriptor mat_desc;
		mat_desc.DynamicFriction = 0.5f;
		mat_desc.StaticFriction = 0.5f;
		mat_desc.Restitution = 0.5f;

		ent.GetComponent<TransformationComponent>()->SetPosition(glm::vec3(i * 5, 100, 0));
		ent.GetComponent<TransformationComponent>()->SetScale(glm::vec3(1, 1, 1));

		ent.AddComponent<physics::PhysicsComponent>(physics::STATIC, physics::BOX, &desc, ent.GetComponent<TransformationComponent>().Get(), mat_desc, physics_system.get());
	
		gfx::Context::Flush(pManager_);
	}

	float ter_scale = V_COUNT / 64.0f;

	gfx::GraphicsPipeline::SetDefault();

//	Allocator::Dispose();

	render::Renderable renderable(mat, mod3);
	render::Renderable renderable2(mat, mod3);
	render::Renderable renderable3(mat2, mod3);


	for (auto i = -50; i < 50; i++)
	{
		for (auto j = -50; j < 50; j++)
		{
			auto ent = pManager_->GetEntityManager()->Create();
			ent.AddComponent<render::RenderableComponent>(renderable2);
			ent.GetComponent<TransformationComponent>()->SetPosition(glm::vec3(i * 7 + 1, 65, j * 7 + 1));
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

	auto point_light = pManager_->GetEntityManager()->Create();
	point_light.AddComponent<render::PointLightComponent>(gfx::PointLight{
		render::Vec3{ -20, 120, 0 },
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
	std::ofstream out("entity_manager.bin");
	cereal::BinaryOutputArchive out_arch(out);
	out_arch(pManager_->GetEntityManager());

	std::ofstream jout("task_system.json");
	cereal::JSONOutputArchive jout_arch(jout);
	jout_arch(pManager_->GetSystemManager()->GetSystem<fluff::ecs::TaskSystem>());

	TextureLibrary::Clear();
	PipelineLibrary::Clear();
}

bool DSState::ShouldShutdown() 
{
	return Keyboard::IsKeyPressed(KEY_ESCAPE);
}
