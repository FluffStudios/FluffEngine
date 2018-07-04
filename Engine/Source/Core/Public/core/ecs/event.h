#pragma once

#include <core/ecs/ievent.h>
#include <common.h>

namespace fluff { namespace ecs {

	template <typename T>
	class FLUFF_API Event : public IEvent
	{
	public:
		/*
			Gets the family ID of the event

			Returns family ID
		*/
		static size_t EventFamilyID();
	};

}

	template<typename T>
	inline size_t ecs::Event<T>::EventFamilyID()
	{
		static size_t c = IEvent::Count_++;
		return c;
	}
}