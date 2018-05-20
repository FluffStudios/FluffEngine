#include <core/ecs/system.h>
#include <core/timer.h>

namespace luminos { namespace ecs
{
	
	SystemManager::SystemManager(EntityManager& Entities, EventManager& Events)
		: EntityManager_(Entities), EventManager_(Events)
	{ }

	SystemManager::SystemManager(EntityManager & Entities, EventManager & Events, double RefreshTime) 
		: SystemManager(Entities, Events)
	{
		FixedRefreshTime_ = RefreshTime; 
	}

	void SystemManager::UpdateAll(const double DeltaTime)
	{
		LUMINOS_ASSERT(Init_)
		Timer::Update(FixedRefreshTime_);
		for (auto it : Systems_) it.second->Update(EntityManager_, EventManager_, DeltaTime);
		auto tmp = this->LastUpdateTime_;
		if (Timer::CurrentTime() >= this->LastUpdateTime_ + this->FixedRefreshTime_)
		{
			for (auto it : Systems_)
			{
				it.second->FixedUpdate(EntityManager_, EventManager_);
			}
			this->LastUpdateTime_ = Timer::CurrentTime();
		}
	}

	void SystemManager::Configure()
	{
		for (auto it : Systems_) it.second->Configure(EntityManager_, EventManager_);
		Init_ = true;
	}

} }