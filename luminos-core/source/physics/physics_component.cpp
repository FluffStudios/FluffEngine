#include <physics/physics_component.h>
#include <Physx/PxMaterial.h>

#include <glm/gtx/euler_angles.hpp>

namespace luminos { namespace physics {

	PhysicsComponent::PhysicsComponent(ActorType Actor, GeometryType Geometry, GeometryDesc * GeomDesc, TransformationComponent * Transformation, PhysicsMaterialDescriptor Material, PhysicsSystem * System, float Density)
	{
		physx::PxTransform * transform = new physx::PxTransform;
		transform->p = physx::PxVec3(Transformation->GetPosition().x, Transformation->GetPosition().y, Transformation->GetPosition().z);
		auto quat = glm::toQuat(glm::orientate3(glm::radians(Transformation->GetRotation())));
		transform->q = physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
		pMaterial_ = new PhysicsMaterial(System->GetSDK(), Material);
		switch (Geometry)
		{
		case SPHERE:
			pGeometry_ = new physx::PxSphereGeometry(((SphereDesc *)GeomDesc)->radius);
			break;
		case BOX:
		{
			BoxDesc * box = (BoxDesc *)GeomDesc;
			pGeometry_ = new physx::PxBoxGeometry(box->HalfWidth, box->HalfHeight, box->HalfDepth);
			break;
		}
		case PLANE:
		{
			if (Actor != STATIC) LUMINOS_ERROR_FUNC("Actor must be static.")
				pGeometry_ = new physx::PxPlaneGeometry();
			break;
		}
		case CAPSULE:
		{
			CapsuleDesc * cap = (CapsuleDesc *)GeomDesc;
			pGeometry_ = new physx::PxCapsuleGeometry(cap->Radius, cap->HalfHeight);
			break;
		}
		case HEIGHTFIELD:
		{
			HeightFieldDesc * hf = (HeightFieldDesc*)GeomDesc;
			physx::PxHeightFieldDesc desc;
			desc.format = physx::PxHeightFieldFormat::eS16_TM;
			desc.nbColumns = hf->NumCols;
			desc.nbRows = hf->NumRows;
			desc.thickness = -3;

			physx::PxHeightFieldSample * samples = static_cast<physx::PxHeightFieldSample *>(calloc(hf->NumRows * hf->NumCols, sizeof(physx::PxHeightFieldSample)));
			memcpy(samples, hf->HeightMap, hf->NumCols * hf->NumRows * sizeof(int16_t));
			desc.samples.data = samples;
			desc.samples.stride = sizeof(physx::PxHeightFieldSample);

			physx::PxHeightField * field = hf->pManager->GetCooking()->createHeightField(desc, hf->pManager->GetSDK()->getPhysicsInsertionCallback());
			pGeometry_ = new physx::PxHeightFieldGeometry(field, physx::PxMeshGeometryFlags(), hf->HeightScale, hf->RowScale, hf->ColumnScale);
		}
		}
		switch (Actor)
		{
		case STATIC:
			pActor_ = new PhysicsStaticActor(System->GetSDK(), *transform, *pGeometry_, *(pMaterial_->GetPhysxMaterial()));
			break;
		case DYNAMIC:
			pActor_ = new PhysicsDynamicActor(System->GetSDK(), *transform, *pGeometry_, *(pMaterial_->GetPhysxMaterial()), Density);
			break;
		}

		delete transform;

		pActor_->AddToScene(System->GetScene()->GetScene());
	}

	PhysicsComponent::~PhysicsComponent()
	{
		delete pActor_;
		delete pMaterial_;
		delete pGeometry_;
	}

} }