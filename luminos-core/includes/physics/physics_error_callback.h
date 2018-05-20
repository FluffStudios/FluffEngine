#pragma once

#include <Physx/foundation/PxErrorCallback.h>
#include <Physx/foundation/PxErrors.h>

namespace luminos { namespace physics {

	class PhysicsErrorCallback : public physx::PxErrorCallback
	{
	public:
		/*
			Reports error to console
			
			code - Error code
			message - Message text
			file - File error is in
			line - Line of error
		 */
		void reportError(physx::PxErrorCode::Enum code, const char *message, const char *file, int line) override;
	};

} }
