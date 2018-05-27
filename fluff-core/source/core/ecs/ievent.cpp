#include <core/ecs/ievent.h>

namespace luminos { namespace ecs {

	size_t IEvent::Count_ = 0;

	IEvent::~IEvent() { }

} }