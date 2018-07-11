#pragma once

#include <core/ecs/system.h>
#include <core/ecs/event_manager.h>
#include <core/ecs/entity.h>

using luminos::ecs::System;
using luminos::ecs::EventManager;
using luminos::ecs::EntityManager;

class TestSystem : public System<TestSystem>
{
	std::vector<float> floats;
public:
	TestSystem()
	{
		for (auto i = 0; i < 1024 * 1024; i++) floats.push_back(static_cast<float>(rand()) / std::numeric_limits<float>::max());
	}

	void FixedUpdate(EntityManager & Entities, EventManager & Events) override 
	{
		for (auto i : floats)
		{
			floats[i] = sqrtf(i);
		}
	}
};