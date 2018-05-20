#include <physics/physics_allocator.h>

#include <common.h>

namespace luminos { namespace physics {

	void* PhysicsAllocator::allocate(size_t size, const char *typeName, const char *filename, int line)
	{
		void * ptr = _aligned_malloc(size, 16);
		LUMINOS_ASSERT(ptr != NULL)
		return ptr;
	}

	void PhysicsAllocator::deallocate(void * ptr)
	{
		_aligned_free(ptr);
		ptr = nullptr;
	}

} }