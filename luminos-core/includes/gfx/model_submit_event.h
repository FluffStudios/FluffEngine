#pragma once

#include <core/ecs/entity.h>
#include <common.h>

namespace luminos { namespace gfx {

	struct LUMINOS_API ModelSubmittedEvent : public ecs::Event<ModelSubmittedEvent>
	{
		/*
			Creates a new Model Submitted Event
		*/
		explicit ModelSubmittedEvent() { }

		/*
			Creates a new Model Submitted Event

			Ent		Entity created
		*/
		explicit ModelSubmittedEvent(ecs::Entity Ent) : E(Ent) { Ent.Destroy(); }

		/*
			Destructor
		*/
		virtual ~ModelSubmittedEvent() { };

		ecs::Entity E;
	};

} }