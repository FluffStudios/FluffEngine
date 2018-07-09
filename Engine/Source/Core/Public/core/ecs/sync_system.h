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
		SyncSystem(std::shared_ptr<threading::ThreadPool> & ThreadPool, ISystem * SystemToExecute);
		void Configure(EntityManager & Entities, EventManager & Events) override;
		void Update(EntityManager & Entities, EventManager & Events, double DeltaTime) override;
		void FixedUpdate(EntityManager & Entities, EventManager & Event) override;
		inline bool UseMainThread() override { return true; }
	};

} }