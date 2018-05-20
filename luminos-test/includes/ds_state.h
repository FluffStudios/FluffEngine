#pragma once

#include <core/engine.h>
#include <luminos.h>
#include <iostream>
#include <core/texture_data.h>
#include <core/assets/material_library.h>

using namespace luminos;

class DSState : public luminos::IGameState
{
	render::Material * mat;
	render::Material * mat2;
	render::Material * mat4;
	std::vector<render::Terrain> terrains;
	ui::DebugWidget * widget;

	ecs::Entity e2, e3;
	physics::PhysicsDynamicActor * actor;
	physics::PhysicsStaticActor * static_actor;
	physx::PxTransform * transform;
public:
	void Configure() override;
	void Run() override;
	void Shutdown() override;
};