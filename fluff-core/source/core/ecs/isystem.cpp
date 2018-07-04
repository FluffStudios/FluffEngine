#include <core/ecs/isystem.h>

namespace fluff { namespace ecs {

	size_t ISystem::FamilyIDCounter = 0;

	ISystem::~ISystem()
	{

	}

	void ISystem::Configure(EntityManager & Entities, EventManager & Events)
	{
		Configure(Events);
	}

	void ISystem::Configure(EventManager & Events)
	{
	}

} }