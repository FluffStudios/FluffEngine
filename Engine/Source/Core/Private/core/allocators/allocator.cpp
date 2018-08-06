#include <core/allocators/allocator.h>
#include <debug.h>
#include <algorithm>

namespace fluff
{
	PoolAllocator::PoolAllocator()
	{
	}

	PoolAllocator::PoolAllocator(const size_t Count)
	{
		FreeBlocks_.reserve(Count);
	}

	PoolAllocator::~PoolAllocator()
	{
		Dispose();
	}

	void PoolAllocator::Dispose()
	{
		for (auto block : Blocks_)
		{
			if (block.Ptr != nullptr) free(block.Ptr);
		}
		for (auto block : FreeBlocks_)
		{
			if (block.Ptr != nullptr) free(block.Ptr);
		}

		Blocks_.clear();
		FreeBlocks_.clear();
	}


	void* PoolAllocator::Alloc(const size_t Size)
	{
		for (auto block : FreeBlocks_)
		{
			if (Size <= block.Size)
			{
				Blocks_.push_back(block);
				sort(Blocks_.begin(), Blocks_.end(), Sort);
				FreeBlocks_.erase(std::find(FreeBlocks_.begin(), FreeBlocks_.end(), block));
				return block.Ptr;
			}
		}
		const Block block =
		{
			Size,
			malloc(Size)
		};
		Blocks_.push_back(block);
		return block.Ptr;
	}

	void PoolAllocator::Free(void * Ptr)
	{
		for (auto block : Blocks_)
		{
			if (block.Ptr == Ptr)
			{
				FreeBlocks_.push_back(block);
				sort(FreeBlocks_.begin(), FreeBlocks_.end(), Sort);
				Blocks_.erase(find(Blocks_.begin(), Blocks_.end(), block));
				return;
			}
		}
		free(Ptr);
	}

	bool PoolAllocator::Sort(const Block Left, const Block Right)
	{
		return Left.Size < Right.Size;
	}

	PoolAllocator Allocator::Alloc_;

	void* Allocator::Alloc(const size_t Size)
	{
		return Alloc_.Alloc(Size);
	}
	
	void Allocator::Free(void * Ptr)
	{
		Alloc_.Free(Ptr);
	}

	void Allocator::Dispose()
	{
		Alloc_.Dispose();
	}
}
