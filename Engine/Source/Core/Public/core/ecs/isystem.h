#pragma once

#include <non_copyable.h>
#include <core/ecs/event_manager.h>

namespace fluff { namespace ecs {

	class EntityManager;

	class FLUFF_API ISystem
	{
	protected:
		size_t FamilyID_;
	public:
		/*
			Configures system

			Entities - Entity Manager associated with system
			Events - Event Manager associated with system
		*/
		virtual void Configure(EntityManager & Entities, EventManager & Events);

		/*
			Configures system

			Events - Event Manager associated with system
		*/
		virtual void Configure(EventManager & Events);

		/*
			Updates system

			Entities - Entity manager associated with system
			Events - Event manager associated with system
			DeltaTime - time between update calls
		*/
		virtual void Update(EntityManager & Entities, EventManager & Events, double DeltaTime) { };

		/*
			Updates system

			Entities - Entity manager associated with system
			Events - Event manager associated with system
		*/
		virtual void FixedUpdate(EntityManager & Entities, EventManager & Events) { }

		virtual bool UseMainThread() { return true; }

		static size_t FamilyIDCounter;
		
		/*
			Destructor
		*/
		virtual ~ISystem();
	};

} }