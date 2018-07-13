#include <core/ecs/isystem.h>

namespace fluff { namespace ecs {

	size_t ISystem::FamilyIDCounter = 0;

	ISystem::~ISystem()
	{

	}

	void ISystem::Configure(std::shared_ptr<EntityManager> & Entities, std::shared_ptr<EventManager> & Events)
	{
		Configure(Events);
	}

	void ISystem::Configure(std::shared_ptr<EventManager> & Events)
	{
	}

} }