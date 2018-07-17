#include <core/ecs/itask.h>

namespace fluff { namespace ecs {

	size_t ITask::FamilyIDCounter = 0;

	void ITask::SetEntity(Entity * Ent)
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

	bool FLUFF_API ITask::ShouldRunOnMain() const
	{
		return false;
	}

	size_t FLUFF_API ITask::SplitSize() const
	{
		return 100;
	}

	bool FLUFF_API ITask::IsComplete() const
	{
		return Complete_;
	}

	void FLUFF_API ITask::SetComplete(bool Complete)
	{
		Complete_ = Complete;
	}

} }