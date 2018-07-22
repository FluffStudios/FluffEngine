#pragma once

#include <common.h>
#include <core/ecs/entity.h>
#include <atomic>

#include <cereal/access.hpp>

namespace fluff { namespace ecs {

	class ITask 
	{
	protected:
		std::atomic_bool Complete_;
		Entity * Ent;
		static size_t FamilyIDCounter;
		mutable size_t ID_;

		friend class cereal::access;

		template <typename Archive>
		void serialize(Archive & Ar)
		{
			Ar(ID_);
		}

	public:
		void FLUFF_API SetEntity(Entity * Ent);

		virtual FLUFF_API ~ITask();
		virtual void FLUFF_API Start();
		virtual void FLUFF_API Update();
		virtual void FLUFF_API FixedUpdate();
		virtual void FLUFF_API LateUpdate();
		virtual bool FLUFF_API ShouldRunOnMain() const;
		virtual size_t FLUFF_API SplitSize() const;
		virtual bool FLUFF_API IsComplete() const;
		virtual void FLUFF_API SetComplete(bool Complete);
		virtual size_t FLUFF_API GetTaskID() const = 0;
	};

} }