#pragma once

#include <string.h>
#include <stdlib.h>

#include <exception>
#include <common.h>

namespace fluff { namespace serial {

	class FLUFF_API Buffer
	{
	protected:
		void * Data_;
		size_t Size_;
		size_t Capacity_;
	public:
		Buffer(size_t Capacity)
		{
			Data_ = calloc(Capacity, 1);
			Size_ = 0;
			Capacity_ = Capacity;
		}

		virtual ~Buffer()
		{
			if (Data_)
			{
				free(Data_);
			}

			Data_ = nullptr;
		}

		virtual void Add(char Data) = 0;
		virtual void Add(unsigned char Data) = 0;
		virtual void Add(short Data) = 0;
		virtual void Add(unsigned short Data) = 0;
		virtual void Add(int Data) = 0;
		virtual void Add(unsigned int Data) = 0;
		virtual void Add(long long Data) = 0;
		virtual void Add(unsigned long long Data) = 0;
		virtual void Add(float Data) = 0;
		virtual void Add(double Data) = 0;

		virtual void Add(char * Data, size_t Count) = 0;
		virtual void Add(unsigned char * Data, size_t Count) = 0;
		virtual void Add(short * Data, size_t Count) = 0;
		virtual void Add(unsigned short * Data, size_t Count) = 0;
		virtual void Add(int * Data, size_t Count) = 0;
		virtual void Add(unsigned int * Data, size_t Count) = 0;
		virtual void Add(long long * Data, size_t Count) = 0;
		virtual void Add(unsigned long long * Data, size_t Count) = 0;
		virtual void Add(float * Data, size_t Count) = 0;
		virtual void Add(double * Data, size_t Count) = 0;

		virtual size_t Read(char * Data, size_t Offset) = 0;
		virtual size_t Read(unsigned char * Data, size_t Offset) = 0;
		virtual size_t Read(short * Data, size_t Offset) = 0;
		virtual size_t Read(unsigned short * Data, size_t Offset) = 0;
		virtual size_t Read(int * Data, size_t Offset) = 0;
		virtual size_t Read(unsigned int * Data, size_t Offset) = 0;
		virtual size_t Read(long long * Data, size_t Offset) = 0;
		virtual size_t Read(unsigned long long * Data, size_t Offset) = 0;
		virtual size_t Read(float * Data, size_t Offset) = 0;
		virtual size_t Read(double * Data, size_t Offset) = 0;

		virtual size_t Read(char ** Data, size_t Offset, size_t Count) = 0;
		virtual size_t Read(unsigned char ** Data, size_t Offset, size_t Count) = 0;
		virtual size_t Read(short ** Data, size_t Offset, size_t Count) = 0;
		virtual size_t Read(unsigned short ** Data, size_t Offset, size_t Count) = 0;
		virtual size_t Read(int ** Data, size_t Offset, size_t Count) = 0;
		virtual size_t Read(unsigned int ** Data, size_t Offset, size_t Count) = 0;
		virtual size_t Read(long long ** Data, size_t Offset, size_t Count) = 0;
		virtual size_t Read(unsigned long long ** Data, size_t Offset, size_t Count) = 0;
		virtual size_t Read(float ** Data, size_t Offset, size_t Count) = 0;
		virtual size_t Read(double ** Data, size_t Offset, size_t Count) = 0;

		virtual void const * GetData() const = 0;
		virtual size_t GetSize() const = 0;
	};

} }