#pragma once

#include <core/ecs/entity.h>
#include <common.h>

namespace fluff { namespace gfx {

	struct ModelSubmittedEvent : public ecs::Event<ModelSubmittedEvent>
	{
		/*
			Creates a new Model Submitted Event
		*/
		FLUFF_API ModelSubmittedEvent() { }

		/*
			Creates a new Model Submitted Event

			Ent		Entity created
		*/
		explicit FLUFF_API ModelSubmittedEvent(ecs::Entity Ent) : E(Ent) { Ent.Destroy(); }

		/*
			Destructor
		*/
		virtual FLUFF_API ~ModelSubmittedEvent() { };

		ecs::Entity E;
	};

} }