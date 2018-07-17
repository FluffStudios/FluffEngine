#pragma once

#include <core/engine.h>
#include <fluff.h>
#include <core/assets/material_library.h>

using namespace fluff;

class TestTask : public fluff::ecs::Task<TestTask>
{
	size_t Updates;
public:
	void Update() override
	{
		Updates++;
	}
};

class DSState : public fluff::IGameState
{
private:
	render::Material * mat;
	render::Material * mat2;
	render::Material * mat4;

	physics::PhysicsDynamicActor * actor;
	physics::PhysicsStaticActor * static_actor;
public:
	void Configure() override;
	void Run() override;
	void Shutdown() override;
	bool ShouldShutdown() override;
};

REGISTER_TASK(TestTask)