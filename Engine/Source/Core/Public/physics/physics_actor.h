#pragma once

#include <physics/physics_geometry.h>
#include <physics/physics_material.h>
#include <glm/gtc/quaternion.hpp>
#include <memory>

namespace fluff { namespace physics {

	enum ActorType
	{
		STATIC,
		DYNAMIC
	};

	class PhysicsActor
	{
	public:
		virtual glm::vec3 GetPosition() const = 0;
		virtual glm::vec3 GetRotation() const = 0;
		virtual glm::vec3 GetLinearVelocity() const = 0;
		virtual glm::vec3 GetAngularVelocity() const = 0;
		virtual void * GetPointer() const = 0;
		virtual const glm::mat4 GetTransform() const = 0;
		virtual void Update() = 0;
	};

	class PhysicsDynamicActor : public PhysicsActor
	{
		struct PhysicsDynamicActorImpl;
		std::unique_ptr<PhysicsDynamicActorImpl> Impl_;
	public:
		PhysicsDynamicActor(void * pSDK, glm::vec3 Position, glm::quat Rotation, GeometryType Geometry, GeometryDesc * GeomDesc, PhysicsMaterialDescriptor Material, float Density);
		glm::vec3 GetPosition() const override;
		glm::vec3 GetRotation() const override;
		glm::vec3 GetLinearVelocity() const override;
		glm::vec3 GetAngularVelocity() const override;
		void SetPosition(const glm::vec3 & NewPosition);
		void ApplyForce(const glm::vec3 & Force);
		void * GetPointer() const override;
		const glm::mat4 GetTransform() const override;
		void Update() override;
	};

	class PhysicsStaticActor : public PhysicsActor
	{
		struct PhysicsStaticActorImpl;
		std::unique_ptr<PhysicsStaticActorImpl> Impl_;
	public:
		PhysicsStaticActor(void * pSDK, glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale, GeometryType Geometry, GeometryDesc * GeomDesc, PhysicsMaterialDescriptor Material);
		glm::vec3 GetPosition() const override;
		glm::vec3 GetRotation() const override;
		glm::vec3 GetLinearVelocity() const override { return glm::vec3(0.0f); }
		glm::vec3 GetAngularVelocity() const override { return glm::vec3(0.0f); }
		void * GetPointer() const override;
		const glm::mat4 GetTransform() const override;
		void Update() override;
	};

} }