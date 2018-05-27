#pragma once

#include <common.h>

namespace luminos { namespace ecs {

	class LUMINOS_API IEvent
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