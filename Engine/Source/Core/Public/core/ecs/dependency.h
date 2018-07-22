#pragma once

#include <core/ecs/system.h>
#include <core/ecs/event.h>
#include <core/ecs/entity.h>
#include <common.h>

namespace fluff { namespace ecs {

	template <typename Type, typename ... Dependencies>
	class FLUFF_API Dependency : public System<Dependency<Type, Dependencies ...>>, public Receiver<Dependency<Type, Dependencies ...>>
	{
	public:
		/*
			Adds dependencies to entity when type is added

			Event - Component of Type is added event
		*/
		void Receive(const ComponentAddedEvent<Type> & Event)
		{
			this->Assign<Dependencies ...>(Event.E);
		}

		/*
			Configures system to handle adding dependencies

			Events - Event manager associated with system
		*/
		void Configure(std::shared_ptr<EventManager> & Events) override
		{
			Events->SubscribeToEvent<ComponentAddedEvent<Type>>(*this);
		}

		/*
			Does nothing.  Components are added when received, not on update loop

			Entities - Entity manager associated with system
			Events - Event manager associated with system
			Delta - change in time between this and last update call
		*/
		void Update(std::shared_ptr<ecs::EntityManager> & Entities, std::shared_ptr<ecs::EventManager> & Events, double Delta) override { }
	private:
		template <typename D> void Assign(Entity Ent)
		{
			if (!Ent.HasComponent<D>()) Ent.AddComponent<D>();
		}
		template <typename D, typename D1, typename ... Ds> void Assign(Entity Ent)
		{
			Assign<D>(Ent);
			Assign<D1, Ds...>(Ent);
		}
	};

} }