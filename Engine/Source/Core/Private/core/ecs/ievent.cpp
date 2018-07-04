#include <core/ecs/ievent.h>

namespace fluff { namespace ecs {

	size_t IEvent::Count_ = 0;

	IEvent::~IEvent() { }

} }