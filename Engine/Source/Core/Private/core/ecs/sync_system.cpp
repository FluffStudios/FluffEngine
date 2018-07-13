#include <core/ecs/sync_system.h>

namespace fluff { namespace ecs {

	SyncSystem::SyncSystem(std::shared_ptr<threading::ThreadPool> & ThreadPool, ISystem * SystemToExecute)
		: Pool_(ThreadPool), SystemToExecute_(SystemToExecute)
	{	}

	void SyncSystem::Configure(std::shared_ptr<EntityManager> & Entities, std::shared_ptr<EventManager> & Events)
	{
	}

	void SyncSystem::Update(std::shared_ptr<EntityManager> & Entities, std::shared_ptr<EventManager> & Events, double DeltaTime)
	{
		Pool_->Sync();
		SystemToExecute_->Update(Entities, Events, DeltaTime);
		Pool_->Sync(false);
	}

	void SyncSystem::FixedUpdate(std::shared_ptr<EntityManager> & Entities, std::shared_ptr<EventManager> & Events)
	{
	}

} }