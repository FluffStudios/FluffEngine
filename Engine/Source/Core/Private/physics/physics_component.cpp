#include <physics/physics_component.h>
#include <Physx/PxMaterial.h>

#include <glm/gtx/euler_angles.hpp>

namespace fluff { namespace physics {

	PhysicsComponent::PhysicsComponent()
		: pActor_(nullptr), pMaterial_(nullptr)
	{
	}

	PhysicsComponent::PhysicsComponent(ActorType Actor, GeometryType Geometry, GeometryDesc * GeomDesc, TransformationComponent * Transformation, PhysicsMaterialDescriptor Material, PhysicsSystem * System, float Density)
	{
		auto p = glm::vec3(Transformation->GetPosition().x, Transformation->GetPosition().y, Transformation->GetPosition().z);
		auto quat = glm::toQuat(glm::orientate3(glm::radians(Transformation->GetRotation())));
		switch (Actor)
		{
		case STATIC:
			pActor_ = new PhysicsStaticActor(System->GetSDK(), Transformation->GetPosition(), Transformation->GetRotation(), Transformation->GetScale(), Geometry, GeomDesc, Material);
			break;
		case DYNAMIC:
			pActor_ = new PhysicsDynamicActor(System->GetSDK(), p, quat, Geometry, GeomDesc, Material, Density);
			break;
		}

		System->GetScene()->AddToScene(pActor_);
	}

	PhysicsComponent::~PhysicsComponent()
	{
		delete pActor_;
	}

} }