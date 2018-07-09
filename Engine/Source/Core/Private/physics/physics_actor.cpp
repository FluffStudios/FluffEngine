#include <physics/physics_actor.h>

#include <Physx/extensions/PxRigidActorExt.h>
#include <Physx/extensions/PxRigidBodyExt.h>
#include <Physx/extensions/PxSimpleFactory.h>
#include <Physx/PxPhysics.h>
#include <Physx/PxRigidActor.h>
#include <Physx/PxRigidStatic.h>
#include <Physx/PxPhysics.h>
#include <Physx/PxRigidDynamic.h>
#include <Physx/PxScene.h>
#include <Physx/geometry/PxHeightFieldDesc.h>
#include <Physx/geometry/PxHeightFieldSample.h>
#include <Physx/geometry/PxHeightFieldGeometry.h>
#include <Physx/geometry/PxHeightFieldFlag.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Physx/cooking/PxCooking.h>

float _clamp(float x) {

	return x > 1.0f ? 1.0f : (x < -1.0f ? -1.0f : x);

}

namespace fluff { namespace physics {

	struct PhysicsDynamicActor::PhysicsDynamicActorImpl
	{
		physx::PxTransform Pose_;
		physx::PxRigidDynamic * pActor;
		physx::PxMaterial * pMat;
		physx::PxReal Min_;
	};

	PhysicsDynamicActor::PhysicsDynamicActor(void * pSDK, glm::vec3 Position, glm::quat Rotation, GeometryType Geometry, GeometryDesc * GeomDesc, PhysicsMaterialDescriptor Material, float Density)
		: Impl_(new PhysicsDynamicActor::PhysicsDynamicActorImpl)
	{
		Impl_->Pose_ = physx::PxTransform
		{
			physx::PxVec3(Position.x, Position.y, Position.z),
			physx::PxQuat(Rotation.x, Rotation.y, Rotation.z, Rotation.w)
		};

		physx::PxGeometry * geometry = nullptr;

		switch (Geometry)
		{
		case SPHERE:
			geometry = new physx::PxSphereGeometry(((SphereDesc *)GeomDesc)->radius);
			break;
		case BOX:
		{
			BoxDesc * box = (BoxDesc *)GeomDesc;
			geometry = new physx::PxBoxGeometry(box->HalfWidth, box->HalfHeight, box->HalfDepth);
			break;
		}
		case PLANE:
		{
			geometry = new physx::PxPlaneGeometry();
			break;
		}
		case CAPSULE:
		{
			CapsuleDesc * cap = (CapsuleDesc *)GeomDesc;
			geometry = new physx::PxCapsuleGeometry(cap->Radius, cap->HalfHeight);
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

			physx::PxHeightField * field = ((physx::PxCooking*) hf->pManager->GetCooking())->createHeightField(desc, ((physx::PxPhysics *)hf->pManager->GetSDK())->getPhysicsInsertionCallback());
			geometry = new physx::PxHeightFieldGeometry(field, physx::PxMeshGeometryFlags(), hf->HeightScale, hf->RowScale, hf->ColumnScale);
		}
		}

		Impl_->pMat = ((physx::PxPhysics*)pSDK)->createMaterial(Material.StaticFriction, Material.DynamicFriction, Material.Restitution);
		if (geometry) Impl_->pActor = physx::PxCreateDynamic(*((physx::PxPhysics *)pSDK), Impl_->Pose_, *geometry, *(Impl_->pMat), Density);
	}

	glm::vec3 PhysicsDynamicActor::GetPosition() const
	{
		auto pos = Impl_->pActor->getGlobalPose().p;
		return glm::vec3(pos.x, pos.y, pos.z);
	}

	glm::vec3 PhysicsDynamicActor::GetRotation() const
	{
		auto rot = Impl_->pActor->getGlobalPose().q;
		glm::quat q(rot.w, rot.x, rot.y, rot.z);
		q = glm::normalize(q);

		auto sqx = q.x * q.x;
		auto sqy = q.y * q.y;
		auto sqz = q.z * q.z;
		auto sqw = q.w * q.w;

		auto x = atan2(2 * (q.x * q.w - q.y * q.z), (sqw - sqx - sqy + sqz));
		auto y = asin(_clamp(2 * (q.x * q.z + q.y * q.w)));
		auto z = atan2(2 * (q.z * q.w - q.x * q.y), (sqw + sqx - sqy - sqz));

		return glm::degrees(glm::vec3(x, y, z));
	}

	glm::vec3 PhysicsDynamicActor::GetLinearVelocity() const
	{
		auto vel = Impl_->pActor->getLinearVelocity();
		return glm::vec3(vel.x, vel.y, vel.z);
	}

	glm::vec3 PhysicsDynamicActor::GetAngularVelocity() const
	{
		auto vel = Impl_->pActor->getAngularVelocity();
		return glm::degrees(glm::vec3(vel.x, vel.y, vel.z));
	}

	void PhysicsDynamicActor::SetPosition(const glm::vec3 & NewPosition)
	{
		physx::PxRigidBody * body = (physx::PxRigidDynamic *) Impl_->pActor;
		Impl_->Pose_.p = physx::PxVec3(NewPosition.x, NewPosition.y, NewPosition.z);
		body->setGlobalPose(Impl_->Pose_);
	}

	void PhysicsDynamicActor::ApplyForce(const glm::vec3 & Force)
	{
		Impl_->pActor->addForce(physx::PxVec3(Force.x, Force.y, Force.z));
	}

	void * PhysicsDynamicActor::GetPointer() const
	{
		return Impl_->pActor;
	}

	struct PhysicsStaticActor::PhysicsStaticActorImpl
	{
		physx::PxTransform Pose_;
		physx::PxRigidStatic * pActor;
		physx::PxMaterial * pMat;
		physx::PxReal Min_;
	};

	PhysicsStaticActor::PhysicsStaticActor(void * pSDK, glm::vec3 Position, glm::quat Rotation, GeometryType Geometry, GeometryDesc * GeomDesc, PhysicsMaterialDescriptor Material)
		: Impl_(new PhysicsStaticActor::PhysicsStaticActorImpl)
	{
		Impl_->Pose_ = physx::PxTransform
		{
			physx::PxVec3(-Position.x, Position.y, Position.z),
			physx::PxQuat(Rotation.x, Rotation.y, Rotation.z, Rotation.w)
		};

		physx::PxGeometry * geometry = nullptr;

		switch (Geometry)
		{
		case SPHERE:
			geometry = new physx::PxSphereGeometry(((SphereDesc *)GeomDesc)->radius);
			break;
		case BOX:
		{
			BoxDesc * box = (BoxDesc *)GeomDesc;
			geometry = new physx::PxBoxGeometry(box->HalfWidth, box->HalfHeight, box->HalfDepth);
			break;
		}
		case PLANE:
		{
			geometry = new physx::PxPlaneGeometry();
			break;
		}
		case CAPSULE:
		{
			CapsuleDesc * cap = (CapsuleDesc *)GeomDesc;
			geometry = new physx::PxCapsuleGeometry(cap->Radius, cap->HalfHeight);
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

			using physx::PxReal;
			using physx::PxI16;

			PxReal minHeight = PX_MAX_F32;
			PxReal maxHeight = -PX_MAX_F32;

			for (uint32_t s = 0; s < hf->NumRows * hf->NumCols; s++)
			{
				minHeight = physx::PxMin(minHeight, hf->HeightMap[s]);
				maxHeight = physx::PxMax(maxHeight, hf->HeightMap[s]);
			}

			PxReal dHeight = maxHeight - minHeight;
			PxReal quantization = (physx::PxReal) 0x7fff;

			PxReal heightScale = physx::PxMax(dHeight / quantization, PX_MIN_HEIGHTFIELD_Y_SCALE);

			physx::PxHeightFieldSample * samples = new physx::PxHeightFieldSample[hf->NumRows * hf->NumCols];
			for (uint32_t i = 0; i < hf->NumRows * hf->NumCols; i++)
			{
				PxI16 height = PxI16(quantization * ((hf->HeightMap[i] - minHeight) / dHeight));
				physx::PxHeightFieldSample& smp = (physx::PxHeightFieldSample&) samples[i];
				smp.height = height;
			}

			desc.samples.data = samples;
			desc.samples.stride = sizeof(physx::PxU32);

			physx::PxHeightField * field = ((physx::PxCooking*) hf->pManager->GetCooking())->createHeightField(desc, ((physx::PxPhysics *)hf->pManager->GetSDK())->getPhysicsInsertionCallback());
			geometry = new physx::PxHeightFieldGeometry(field, physx::PxMeshGeometryFlag::eDOUBLE_SIDED, dHeight != 0.0f ? heightScale : 1.0f, hf->RowScale, hf->ColumnScale);
			Impl_->Pose_.p.y += minHeight;
			Impl_->Min_ = minHeight;
			delete [] samples;
		}
		}

		Impl_->pMat = ((physx::PxPhysics*)pSDK)->createMaterial(Material.StaticFriction, Material.DynamicFriction, Material.Restitution);
		if (geometry) Impl_->pActor = physx::PxCreateStatic(*((physx::PxPhysics *)pSDK), Impl_->Pose_, *geometry, *(Impl_->pMat));
	}

	glm::vec3 PhysicsStaticActor::GetPosition() const
	{
		return glm::vec3(Impl_->Pose_.p.x, Impl_->Pose_.p.y - Impl_->Min_, Impl_->Pose_.p.z);
	}

	glm::vec3 PhysicsStaticActor::GetRotation() const
	{
		auto rot = Impl_->pActor->getGlobalPose().q;
		glm::quat q(rot.w, rot.x, rot.y, rot.z);
		q = glm::normalize(q);

		auto sqx = q.x * q.x;
		auto sqy = q.y * q.y;
		auto sqz = q.z * q.z;
		auto sqw = q.w * q.w;

		auto x = atan2(2 * (q.x * q.w - q.y * q.z), (sqw - sqx - sqy + sqz));
		auto y = asin(_clamp(2 * (q.x * q.z + q.y * q.w)));
		auto z = atan2(2 * (q.z * q.w - q.x * q.y), (sqw + sqx - sqy - sqz));

		return glm::degrees(glm::vec3(x, y, z));
	}

	void * PhysicsStaticActor::GetPointer() const
	{
		return Impl_->pActor;
	}
} }