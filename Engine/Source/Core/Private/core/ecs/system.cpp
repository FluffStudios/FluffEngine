#include <core/ecs/system.h>
#include <core/timer.h>

#include <core/ecs/entity.h>
#include <core/ecs/event_manager.h>

void Update(std::shared_ptr<fluff::ecs::ISystem> System, fluff::ecs::EntityManager & Entities, fluff::ecs::EventManager & Events, float DeltaTime)
{
	System->Update(Entities, Events, DeltaTime);
}

namespace fluff { namespace ecs
{
	
	SystemManager::SystemManager(EntityManager& Entities, EventManager& Events)
		: SystemManager(Entities, Events, 1.0f / 60.0f)
	{ }

	SystemManager::SystemManager(EntityManager & Entities, EventManager & Events, double RefreshTime) 
		: EntityManager_(Entities), EventManager_(Events)
	{
		Pool_ = std::make_shared<threading::ThreadPool>(2);
		FixedRefreshTime_ = RefreshTime; 
	}

	void SystemManager::UpdateAll(const double DeltaTime)
	{
		FLUFF_ASSERT(Init_)
		Timer::Update(FixedRefreshTime_);
		std::vector<std::future<void>> funcs;
		for (auto it : Systems_)
		{
			if (!it.second->UseMainThread())
			{
				{
					funcs.push_back(Pool_->PushTask([this, it, DeltaTime](size_t) {
						it.second->Update(EntityManager_, EventManager_, DeltaTime);
					}));
					std::this_thread::sleep_for(std::chrono::microseconds(1));
				}
			} 
			else
			{
				it.second->Update(EntityManager_, EventManager_, DeltaTime);
			}
		}
		auto tmp = this->LastUpdateTime_;
		if (Timer::CurrentTime() >= this->LastUpdateTime_ + this->FixedRefreshTime_)
		{
			for (auto it : Systems_)
			{
				if (!it.second->UseMainThread())
				{
					{
						funcs.push_back(Pool_->PushTask([this, it, DeltaTime](size_t) {
							it.second->FixedUpdate(EntityManager_, EventManager_);
						}));
						std::this_thread::sleep_for(std::chrono::microseconds(1));
					}
				}
				else
				{
					it.second->FixedUpdate(EntityManager_, EventManager_);
				}
			}
			this->LastUpdateTime_ = Timer::CurrentTime();
		}

		for (auto & task : funcs)
		{
			task.get();
		}
	}

	void SystemManager::Configure()
	{
		for (auto it : Systems_) it.second->Configure(EntityManager_, EventManager_);
		Init_ = true;
	}

	std::shared_ptr<threading::ThreadPool> & SystemManager::GetThreadPool()
	{
		return Pool_;
	}

} }