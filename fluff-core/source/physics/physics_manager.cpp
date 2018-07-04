#include <physics/physics.h>
#include <Physx/foundation/PxFoundationVersion.h>

#include <core/debug_message.h>
#include <Physx/PxPhysicsVersion.h>
#include <Physx/common/PxTolerancesScale.h>
#include <Physx/extensions/PxDefaultCpuDispatcher.h>

#include <Physx/foundation/PxFoundation.h>
#include <Physx/foundation/PxErrorCallback.h>
#include <Physx/extensions/PxDefaultAllocator.h>
#include <Physx/extensions/PxDefaultErrorCallback.h>
#include <Physx/PxPhysics.h>
#include <Physx/pvd/PxPvd.h>
#include <Physx/pvd/PxPvdTransport.h>
#include <Physx/PxPhysics.h>
#include <Physx/task/PxCpuDispatcher.h>
#include <Physx/cooking/PxCooking.h>

#ifndef PVD_HOST
#define PVD_HOST "127.0.0.1"
#endif

namespace fluff {	namespace physics {

	struct PhysicsManager::PhysicsManagerImpl
	{
		physx::PxDefaultErrorCallback DefaultErrorCallback_;
		physx::PxDefaultAllocator DefaultAllocatorCallback_;
		physx::PxFoundation * Foundation_;
		physx::PxPhysics * Physics_;
		physx::PxPvd * PVD_ = nullptr;
		physx::PxCooking * Cooking_;
		physx::PxCpuDispatcher * CPUDispatch_;
	};

	PhysicsManager::PhysicsManager()
		: Impl_(new PhysicsManager::PhysicsManagerImpl)
	{

		Impl_->Foundation_ = PxCreateFoundation(PX_FOUNDATION_VERSION, Impl_->DefaultAllocatorCallback_, Impl_->DefaultErrorCallback_);
		if (Impl_->Foundation_ == nullptr)
		{
			std::cout << "PxCreateFoundation failed!" << std::endl;
		}
#ifdef _DEBUG
		Impl_->PVD_ = physx::PxCreatePvd(*(Impl_->Foundation_));
		auto transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
		Impl_->PVD_->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
#endif
		
		Impl_->Physics_ = PxCreatePhysics(PX_PHYSICS_VERSION, *(Impl_->Foundation_), physx::PxTolerancesScale(), false, Impl_->PVD_);
		if (!Impl_->Physics_)
		{
			std::cout << "Could not create physics!" << std::endl;
		}

		
		Impl_->Cooking_ = PxCreateCooking(PX_PHYSICS_VERSION, *(Impl_->Foundation_), physx::PxCookingParams(physx::PxTolerancesScale()));
		if (Impl_->Cooking_ == nullptr)
		{
			std::cout << "PxCreateCooking failed!" << std::endl;
		}
	}

	PhysicsManager::~PhysicsManager()
	{
		if (Impl_) delete Impl_;
	}

	void * PhysicsManager::GetErrorCallback()
	{
		return &(Impl_->DefaultErrorCallback_);
	}

	void * PhysicsManager::GetAllocatorCallback()
	{
		return &(Impl_->DefaultAllocatorCallback_);
	}

	void * PhysicsManager::GetCPUDispatcher()
	{
		return Impl_->CPUDispatch_;
	}

	void * PhysicsManager::GetSDK()
	{
		return Impl_->Physics_;
	}

	void * PhysicsManager::GetCooking()
	{
		return Impl_->Cooking_;
	}

} }