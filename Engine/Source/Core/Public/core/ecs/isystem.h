#pragma once

#include <non_copyable.h>
#include <core/ecs/event_manager.h>

#include <memory>

namespace fluff { namespace ecs {

	class EntityManager;

	class ISystem
	{
	protected:
		size_t FamilyID_;
	public:
		/*
			Configures system

			Entities - Entity Manager associated with system
			Events - Event Manager associated with system
		*/
		virtual void FLUFF_API Configure(std::shared_ptr<EntityManager> & Entities, std::shared_ptr<EventManager> & Events);

		/*
			Configures system

			Events - Event Manager associated with system
		*/
		virtual void FLUFF_API Configure(std::shared_ptr<EventManager> & Events);

		/*
			Updates system

			Entities - Entity manager associated with system
			Events - Event manager associated with system
			DeltaTime - time between update calls
		*/
		virtual void FLUFF_API Update(std::shared_ptr<EntityManager> & Entities, std::shared_ptr<EventManager> & Events, double DeltaTime) { };

		/*
			Updates system

			Entities - Entity manager associated with system
			Events - Event manager associated with system
		*/
		virtual void FLUFF_API FixedUpdate(std::shared_ptr<EntityManager> & Entities, std::shared_ptr<EventManager> & Events) { }

		virtual bool FLUFF_API UseMainThread() { return true; }

		static size_t FamilyIDCounter;
		
		/*
			Destructor
		*/
		virtual FLUFF_API ~ISystem();
	};

} }