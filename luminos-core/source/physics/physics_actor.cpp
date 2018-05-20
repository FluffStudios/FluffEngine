#include <physics/physics_actor.h>

#include <Physx/extensions/PxRigidActorExt.h>
#include <Physx/extensions/PxRigidBodyExt.h>
#include <Physx/extensions/PxSimpleFactory.h>
#include <Physx/PxPhysics.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace luminos { namespace physics {

	PhysicsDynamicActor::PhysicsDynamicActor(physx::PxPhysics * pSDK, physx::PxTransform & Transform, physx::PxGeometry & Geometry, physx::PxMaterial & Material, float Density)
	{
		Pose_ = Transform;
		pActor = physx::PxCreateDynamic(*pSDK, Pose_, Geometry, Material, Density);
	}

	glm::vec3 PhysicsDynamicActor::GetPosition() const
	{
		auto pos = pActor->getGlobalPose().p;
		return glm::vec3(pos.x, pos.y, pos.z);
	}

	glm::vec3 PhysicsDynamicActor::GetRotation() const
	{
		auto rot = pActor->getGlobalPose().q;
		glm::quat rot_glm(rot.x, rot.y, rot.z, rot.w);
		return glm::degrees(glm::eulerAngles(rot_glm));
	}

	glm::vec3 PhysicsDynamicActor::GetLinearVelocity() const
	{
		auto vel = pActor->getLinearVelocity();
		return glm::vec3(vel.x, vel.y, vel.z);
	}

	glm::vec3 PhysicsDynamicActor::GetAngularVelocity() const
	{
		auto vel = pActor->getAngularVelocity();
		return glm::degrees(glm::vec3(vel.x, vel.y, vel.z));
	}

	void PhysicsDynamicActor::SetPosition(const glm::vec3 & NewPosition)
	{
		physx::PxRigidBody * body = (physx::PxRigidDynamic *) pActor;
		Pose_.p = physx::PxVec3(NewPosition.x, NewPosition.y, NewPosition.z);
		body->setGlobalPose(Pose_);
	}

	void PhysicsDynamicActor::AddToScene(physx::PxScene * Scene)
	{
		Scene->addActor(*pActor);
	}

	void PhysicsDynamicActor::RemoveFromScene(physx::PxScene * Scene)
	{
		Scene->removeActor(*pActor);
	}

	void PhysicsDynamicActor::ApplyForce(const glm::vec3 & Force)
	{
		pActor->addForce(physx::PxVec3(Force.x, Force.y, Force.z));
	}

	PhysicsStaticActor::PhysicsStaticActor(physx::PxPhysics * pSDK, physx::PxTransform & Transform, physx::PxGeometry & Geometry, physx::PxMaterial & Material)
	{
		Pose_ = Transform;
		pActor = physx::PxCreateStatic(*pSDK, Pose_, Geometry, Material);
	}

	glm::vec3 PhysicsStaticActor::GetPosition() const
	{
		return glm::vec3(Pose_.p.x, Pose_.p.y, Pose_.p.z);
	}

	glm::vec3 PhysicsStaticActor::GetRotation() const
	{
		auto rot = Pose_.q;
		glm::quat rot_glm(rot.x, rot.y, rot.z, rot.w);
		return glm::degrees(glm::eulerAngles(rot_glm));
	}

	void PhysicsStaticActor::AddToScene(physx::PxScene * Scene)
	{
		Scene->addActor(*(physx::PxActor *) pActor);
	}

	void PhysicsStaticActor::RemoveFromScene(physx::PxScene * Scene)
	{
		Scene->removeActor(*(physx::PxActor *) pActor);
	}
} }