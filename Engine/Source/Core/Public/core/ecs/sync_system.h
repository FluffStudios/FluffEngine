#pragma once

#include <core/ecs/system.h>
#include <core/threading/thread_pool.h>

#include <memory>

namespace fluff { namespace ecs {

	class SyncSystem : public System<SyncSystem>
	{
		std::shared_ptr<threading::ThreadPool> & Pool_;
		ISystem * SystemToExecute_;
	public:
		FLUFF_API SyncSystem(std::shared_ptr<threading::ThreadPool> & ThreadPool, ISystem * SystemToExecute);
		void FLUFF_API Configure(std::shared_ptr<EntityManager> & Entities, std::shared_ptr<EventManager> & Events) override;
		void FLUFF_API Update(std::shared_ptr<EntityManager> & Entities, std::shared_ptr<EventManager> & Events, double DeltaTime) override;
		void FLUFF_API FixedUpdate(std::shared_ptr<EntityManager> & Entities, std::shared_ptr<EventManager> & Events) override;
		inline bool UseMainThread() override { return true; }
	};

} }