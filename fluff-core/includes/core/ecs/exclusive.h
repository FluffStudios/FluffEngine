#pragma once

#include <common.h>
#include <core/ecs/system.h>

namespace fluff { namespace ecs {

	template <typename Type, typename ... Dependencies>
	class Exclusive : public System<Exclusive<Type, Dependencies ...>>, public Receiver<Exclusive<Type, Dependencies ...>>
	{
	public:
		/*
			Removes components from entity when type is added

			Event - Component of Type is added event
		*/
		void Receive(const ComponentAddedEvent<Type> & Event)
		{

		}

		/*
			Configures system to handle removing components

			Events - Event manager associated with system
		*/
		void Configure(EventManager & Events) override
		{
			Events.SubscribeToEvent<ComponentAddedEvent<Type>>(*this);
		}

		/*
			Does nothing.  Components are removed when received, not on update loop

			Entities - Entity manager associated with system
			Events - Event manager associated with system
			Delta - change in time between this and last update call
		*/
		void Update(EntityManager & Entities, EventManager & Events, double Delta) override { }
	private:
		template <typename Dep> void Remove(Entity E)
		{
			if (E.HasComponent<Dep>()) E.RemoveComponent<D>();
		}

		template <typename Dep, typename D1, typename Ds> void Remove(Entity E)
		{
			Remove<Dep>(E);
			Remove<D1, Ds ...>(E);
		}
	};

} }