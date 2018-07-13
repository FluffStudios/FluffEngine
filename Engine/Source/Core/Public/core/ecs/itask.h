#pragma once

#include <common.h>
#include <core/ecs/entity.h>

namespace fluff { namespace ecs {

	class ITask 
	{
	protected:
		Entity * Ent;
		static size_t FamilyIDCounter;
	public:
		void FLUFF_API SetEntity(Entity * Ent);

		virtual FLUFF_API ~ITask();
		virtual void FLUFF_API Start();
		virtual void FLUFF_API Update();
		virtual void FLUFF_API LateUpdate();
		virtual bool FLUFF_API ShouldRunOnMain() const { return false; }
		virtual size_t FLUFF_API SplitSize() const { return 2000; }
	};

} }