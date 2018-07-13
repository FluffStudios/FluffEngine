#pragma once

#include <common.h>

namespace fluff { namespace ecs {

	class IEvent
	{
	public:
		/*
			Destructor
		*/
		virtual FLUFF_API ~IEvent();
	protected:
		static size_t Count_;
	};

} }