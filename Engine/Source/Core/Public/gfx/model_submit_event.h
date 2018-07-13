#pragma once

#include <core/ecs/entity.h>
#include <common.h>

namespace fluff { namespace gfx {

	struct FLUFF_API ModelSubmittedEvent : public ecs::Event<ModelSubmittedEvent>
	{
		/*
			Destructor
		*/
		virtual ~ModelSubmittedEvent() { };
	};

} }