#pragma once

#include <fluff.h>
#include <glm/glm.hpp>

#define MOVEMENT_SPEED 75
#define LOOK_SPEED 125

using namespace fluff;

struct MovementSystem : ecs::System<MovementSystem>
{
	ecs::Entity Cam;
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Velocity;
	glm::vec3 AngularVelocity;

	MovementSystem(ecs::Entity Cam) : Cam(Cam) {
		Position = this->Cam.GetComponent<gfx::CameraComponent>()->GetPosition();
		Rotation = this->Cam.GetComponent<gfx::CameraComponent>()->GetRotation();
	}

	void Update(std::shared_ptr<ecs::EntityManager> & Entities, std::shared_ptr<ecs::EventManager> & Events, const double Delta) override
	{
		Cam.GetComponent<gfx::CameraComponent>()->SetPosition(Cam.GetComponent<gfx::CameraComponent>()->GetPosition() + Velocity * (float)Delta);
		Cam.GetComponent<gfx::CameraComponent>()->SetRotation(Cam.GetComponent<gfx::CameraComponent>()->GetRotation() + AngularVelocity * (float)Delta);
	}

	void FixedUpdate(std::shared_ptr<ecs::EntityManager> & Entities, std::shared_ptr<ecs::EventManager> & Events) override
	{
		Velocity = glm::vec3(0);
		AngularVelocity = glm::vec3(0);

		if (fluff::Keyboard::IsKeyPressed(KEY_W))
		{
			const auto rot = Cam.GetComponent<gfx::CameraComponent>()->GetRotation();
			auto x = sin(glm::radians(rot.y));
			auto y = sin(glm::radians(rot.x));
			auto z = cos(glm::radians(rot.y));
			x *= MOVEMENT_SPEED;
			y *= MOVEMENT_SPEED;
			z *= MOVEMENT_SPEED;
			Velocity += glm::vec3(x, y, z);
		}

		if (fluff::Keyboard::IsKeyPressed(KEY_S))
		{
			const auto rot = Cam.GetComponent<gfx::CameraComponent>()->GetRotation();
			auto x = -sin(glm::radians(rot.y));
			auto y = -sin(glm::radians(rot.x));
			auto z = -cos(glm::radians(rot.y));
			x *= MOVEMENT_SPEED;
			y *= MOVEMENT_SPEED;
			z *= MOVEMENT_SPEED;
			Velocity += glm::vec3(x, y, z);
		}

		if (fluff::Keyboard::IsKeyPressed(KEY_A))
		{
			const auto rot = Cam.GetComponent<gfx::CameraComponent>()->GetRotation();
			auto x = cos(glm::radians(rot.y));
			auto z = -sin(glm::radians(rot.y));
			x *= MOVEMENT_SPEED;
			z *= MOVEMENT_SPEED;
			Velocity += glm::vec3(x, 0, z);
		}

		if (fluff::Keyboard::IsKeyPressed(KEY_D))
		{
			const auto rot = Cam.GetComponent<gfx::CameraComponent>()->GetRotation();
			auto x = -cos(glm::radians(rot.y));
			auto z = sin(glm::radians(rot.y));
			x *= MOVEMENT_SPEED;
			z *= MOVEMENT_SPEED;
			Velocity += glm::vec3(x, 0, z);
		}

		if (fluff::Keyboard::IsKeyPressed(KEY_PAGE_UP))
		{
			Velocity = glm::vec3(0, MOVEMENT_SPEED, 0);
		}

		if (fluff::Keyboard::IsKeyPressed(KEY_PAGE_DOWN))
		{
			Velocity = glm::vec3(0, -MOVEMENT_SPEED, 0);
		}

		if (fluff::Keyboard::IsKeyPressed(KEY_UP))
		{
			AngularVelocity = glm::vec3(LOOK_SPEED, 0, 0);
		}

		if (fluff::Keyboard::IsKeyPressed(KEY_DOWN))
		{
			AngularVelocity = glm::vec3(-LOOK_SPEED, 0, 0);
		}

		if (fluff::Keyboard::IsKeyPressed(KEY_RIGHT))
		{
			AngularVelocity = glm::vec3(0, -LOOK_SPEED, 0);
		}
		if (fluff::Keyboard::IsKeyPressed(KEY_LEFT))
		{
			AngularVelocity = glm::vec3(0, LOOK_SPEED, 0);
		}
	}
};
