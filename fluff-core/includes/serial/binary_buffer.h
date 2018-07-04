#pragma once

#include <common.h>
#include <serial/buffer.h>

namespace fluff { namespace serial {

	class FLUFF_API BinaryBuffer final : public Buffer
	{
	public:
		BinaryBuffer(size_t Capacity = 4096);
		~BinaryBuffer();

		virtual void Add(char Data) override;
		virtual void Add(unsigned char Data) override;
		virtual void Add(short Data) override;
		virtual void Add(unsigned short Data) override;
		virtual void Add(int Data) override;
		virtual void Add(unsigned int Data) override;
		virtual void Add(long long Data) override;
		virtual void Add(unsigned long long Data) override;
		virtual void Add(float Data) override;
		virtual void Add(double Data) override;

		virtual void Add(char * Data, size_t Count) override;
		virtual void Add(unsigned char * Data, size_t Count) override;
		virtual void Add(short * Data, size_t Count) override;
		virtual void Add(unsigned short * Data, size_t Count) override;
		virtual void Add(int * Data, size_t Count) override;
		virtual void Add(unsigned int * Data, size_t Count) override;
		virtual void Add(long long * Data, size_t Count) override;
		virtual void Add(unsigned long long * Data, size_t Count) override;
		virtual void Add(float * Data, size_t Count) override;
		virtual void Add(double * Data, size_t Count) override;

		virtual size_t Read(char * Data, size_t Offset) override;
		virtual size_t Read(unsigned char * Data, size_t Offset) override;
		virtual size_t Read(short * Data, size_t Offset) override;
		virtual size_t Read(unsigned short * Data, size_t Offset) override;
		virtual size_t Read(int * Data, size_t Offset) override;
		virtual size_t Read(unsigned int * Data, size_t Offset) override;
		virtual size_t Read(long long * Data, size_t Offset) override;
		virtual size_t Read(unsigned long long * Data, size_t Offset) override;
		virtual size_t Read(float * Data, size_t Offset) override;
		virtual size_t Read(double * Data, size_t Offset) override;

		virtual size_t Read(char ** Data, size_t Offset, size_t Count) override;
		virtual size_t Read(unsigned char ** Data, size_t Offset, size_t Count) override;
		virtual size_t Read(short ** Data, size_t Offset, size_t Count) override;
		virtual size_t Read(unsigned short ** Data, size_t Offset, size_t Count) override;
		virtual size_t Read(int ** Data, size_t Offset, size_t Count) override;
		virtual size_t Read(unsigned int ** Data, size_t Offset, size_t Count) override;
		virtual size_t Read(long long ** Data, size_t Offset, size_t Count) override;
		virtual size_t Read(unsigned long long ** Data, size_t Offset, size_t Count) override;
		virtual size_t Read(float ** Data, size_t Offset, size_t Count) override;
		virtual size_t Read(double ** Data, size_t Offset, size_t Count) override;

		void const * GetData() const override;
		size_t GetSize() const override;
	private:
		void AddRawData(void * Data, size_t Size);
		void ResizeAndCopy(size_t NewCapacity);
	};

} }