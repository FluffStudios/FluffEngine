#pragma once

#include <Physx/foundation/PxAllocatorCallback.h>
#include <common.h>

namespace luminos { namespace physics {

	LUMINOS_API class PhysicsAllocator : public physx::PxAllocatorCallback
	{
	public:
		/*
			Allocates memory
			
			Size - Size of allocation block
			TypeName - Name of allocated type
			Filename - Name of file
			Line - Line number at allocation
			Returns pointer to memory block
		 */
		void * allocate(size_t Size, const char *TypeName, const char *Filename, int Line) override;

		/*
			Deallocates memory
			
			Ptr	- Pointer to memory block
		 */
		void deallocate(void * Ptr) override;
	};

} }
