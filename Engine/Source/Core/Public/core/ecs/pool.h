#pragma once

#include <common.h>
#include <vector>

namespace fluff { namespace ecs {

	class IPool
	{
	protected:
		size_t ElementSize_ = 0;
		size_t Capacity_ = 0;
		size_t ChunkSize_ = 0;
		size_t Size_ = 0;
		std::vector<char *> Chunks_;
	public:
		/*
			Creates a Pool
			
			ElementSize - Size of object to be stored
			ChunkSize - Size of pool chunks
		*/
		FLUFF_API IPool(size_t ElementSize, size_t ChunkSize = 8192);

		/*
			Destructor
		*/
		virtual FLUFF_API ~IPool();

		/*
			Gets the size of the pool

			Returns used size
		*/
		inline size_t FLUFF_API Size() const { return Size_; }

		/*
			Gets the capacity of the pool

			Returns capacity
		*/
		size_t FLUFF_API Capacity() const { return Capacity_; }

		/*
			Gets the number of chunks in the pool

			Returns chunks
		*/
		size_t FLUFF_API Chunks() const { return Chunks_.size(); }

		/*
			Expands the pool at the given index

			Index - index to expand at
		*/
		void FLUFF_API Expand(size_t Index);

		/*
			Reserves more space in the pool

			NewSize - New size of pool
		*/
		void FLUFF_API Reserve(size_t NewSize);

		/*
			Gets a pointer to the value stored at the given index

			Index - Index to retreive data at
			Returns data at index
		*/
		void FLUFF_API * GetAt(size_t Index);

		/*
			Gets a pointer to the value stored at the given index

			Index - Index to retreive data at
			Returns data at index
		*/
		const void FLUFF_API * GetAt(size_t Index) const;

		/*
			Destroys contents of the pool at an index

			Index - Location to destroy
		*/
		virtual void Destroy(size_t Index) = 0;
	};
	
	template<typename T, size_t ChunkSize = 8192>
	class Pool : public IPool
	{
	public:
		/*
			Creates a new pool
		*/
		Pool() : IPool(sizeof(T), ChunkSize) 
		{
			this->ChunkSize_ = ChunkSize;
		};

		/*
			Destructor
		*/
		virtual ~Pool() { }

		/*
			Destroys contents of the pool at an index

			Index - Location to destroy
		*/
		virtual void Destroy(size_t Index) override
		{
			FLUFF_ASSERT(Index < Size_)
			auto ptr = static_cast<T*>(GetAt(Index));
			ptr->~T();
		}
	};

} }