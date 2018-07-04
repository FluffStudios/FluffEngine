#pragma once

#include <common.h>

namespace fluff { namespace ecs {

	class FLUFF_API IEvent
	{
	public:
		/*
			Destructor
		*/
		virtual ~IEvent();
	protected:
		static size_t Count_;
	};

} }