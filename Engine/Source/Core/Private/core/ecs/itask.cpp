#include <core/ecs/itask.h>

namespace fluff { namespace ecs {

	size_t ITask::FamilyIDCounter = 0;

	void ITask::SetEntity(Entity Ent)
	{
		this->Ent = Ent;
	}

	ITask::~ITask()
	{	}

	void ITask::Start()
	{	}

	void ITask::Update()
	{	}

	void ITask::LateUpdate()
	{	}

} }