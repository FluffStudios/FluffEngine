#pragma once

#include <debug.h>
#include <vector>

namespace fluff
{
	class PoolAllocator
	{
		struct Block
		{
			size_t Size;
			void * Ptr;

			bool operator == (const Block & Other) const { return Size == Other.Size && Ptr == Other.Ptr; }
			bool operator != (const Block & Other) const { return Size != Other.Size || Ptr != Other.Ptr; }
		};

		std::vector<Block> Blocks_;
		std::vector<Block> FreeBlocks_;
	public:
		FLUFF_API PoolAllocator();
		explicit FLUFF_API PoolAllocator(size_t Count);
		FLUFF_API ~PoolAllocator();
		void * Alloc(size_t Size);
		void Free(void * Ptr);
		void Dispose();
	private:
		static bool Sort(const Block Left, const Block Right);
	};

	class Allocator
	{
		static PoolAllocator Alloc_;
	public:
		static void * Alloc(size_t Size);
		static void Free(void * Ptr);
		static void Dispose();
	};	
}
