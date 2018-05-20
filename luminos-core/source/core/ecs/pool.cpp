#include <core/ecs/pool.h>

namespace luminos { namespace ecs {

	IPool::IPool(size_t ElementSize, size_t ChunkSize)
		: ElementSize_(ElementSize), ChunkSize_(ChunkSize_), Capacity_(0)
	{ }

	IPool::~IPool()
	{
		for (char * ptr : Chunks_) free(ptr);
	}

	void IPool::Expand(size_t Index)
	{
		if (Index >= Size_)
		{
			if (Index >= Capacity_) Reserve(Index);
			Size_ = Index;
		}
	}

	void IPool::Reserve(size_t NewSize)
	{
		while (Capacity_ < NewSize)
		{
			char * chunk = new char[ElementSize_ * ChunkSize_];
			Chunks_.push_back(chunk);
			Capacity_ += ChunkSize_;
		}
	}

	void * IPool::GetAt(size_t Index)
	{
		LUMINOS_ASSERT(Index < Size_)
		return Chunks_[Index / ChunkSize_] + ((Index % ChunkSize_) * ElementSize_);
	}

	const void * IPool::GetAt(size_t Index) const
	{
		LUMINOS_ASSERT(Index < Size_)
		return Chunks_[Index / ChunkSize_] + ((Index % ChunkSize_) * ElementSize_);
	}

} }