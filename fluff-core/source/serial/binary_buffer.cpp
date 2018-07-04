#include <serial/binary_buffer.h>

namespace fluff { namespace serial {

	BinaryBuffer::BinaryBuffer(size_t Capacity)
		: Buffer(Capacity)
	{	}

	BinaryBuffer::~BinaryBuffer()
	{	}

	void BinaryBuffer::Add(char Data)
	{
		AddRawData(&Data, sizeof(Data));
	}

	void BinaryBuffer::Add(unsigned char Data)
	{
		AddRawData(&Data, sizeof(Data));
	}

	void BinaryBuffer::Add(short Data)
	{
		AddRawData(&Data, sizeof(Data));
	}

	void BinaryBuffer::Add(unsigned short Data)
	{
		AddRawData(&Data, sizeof(Data));
	}

	void BinaryBuffer::Add(int Data)
	{
		AddRawData(&Data, sizeof(Data));
	}

	void BinaryBuffer::Add(unsigned int Data)
	{
		AddRawData(&Data, sizeof(Data));
	}

	void BinaryBuffer::Add(long long Data)
	{
		AddRawData(&Data, sizeof(Data));
	}

	void BinaryBuffer::Add(unsigned long long Data)
	{
		AddRawData(&Data, sizeof(Data));
	}

	void BinaryBuffer::Add(float Data)
	{
		AddRawData(&Data, sizeof(Data));
	}

	void BinaryBuffer::Add(double Data)
	{
		AddRawData(&Data, sizeof(Data));
	}

	void BinaryBuffer::Add(char * Data, size_t Count)
	{
		AddRawData(Data, sizeof(char) * Count);
	}

	void BinaryBuffer::Add(unsigned char * Data, size_t Count)
	{
		AddRawData(Data, sizeof(unsigned char) * Count);
	}

	void BinaryBuffer::Add(short * Data, size_t Count)
	{
		AddRawData(Data, sizeof(short) * Count);
	}

	void BinaryBuffer::Add(unsigned short * Data, size_t Count)
	{
		AddRawData(Data, sizeof(unsigned short) * Count);
	}

	void BinaryBuffer::Add(int * Data, size_t Count)
	{
		AddRawData(Data, sizeof(int) * Count);
	}

	void BinaryBuffer::Add(unsigned int * Data, size_t Count)
	{
		AddRawData(Data, sizeof(unsigned int) * Count);
	}

	void BinaryBuffer::Add(long long * Data, size_t Count)
	{
		AddRawData(Data, sizeof(long long) * Count);
	}

	void BinaryBuffer::Add(unsigned long long * Data, size_t Count)
	{
		AddRawData(Data, sizeof(unsigned long long) * Count);
	}

	void BinaryBuffer::Add(float * Data, size_t Count)
	{
		AddRawData(Data, sizeof(float) * Count);
	}

	void BinaryBuffer::Add(double * Data, size_t Count)
	{
		AddRawData(Data, sizeof(double) * Count);
	}

	size_t BinaryBuffer::Read(char * Data, size_t Offset)
	{
		if (Size_ < Offset + sizeof(char))
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(char));
		return Offset += sizeof(char);
	}

	size_t BinaryBuffer::Read(unsigned char * Data, size_t Offset)
	{
		if (Size_ < Offset + sizeof(unsigned char))
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(unsigned char));
		return Offset += sizeof(unsigned char);
	}

	size_t BinaryBuffer::Read(short * Data, size_t Offset)
	{
		if (Size_ < Offset + sizeof(short))
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(short));
		return Offset += sizeof(short);
	}

	size_t BinaryBuffer::Read(unsigned short * Data, size_t Offset)
	{
		if (Size_ < Offset + sizeof(unsigned short))
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(unsigned));
		return Offset += sizeof(unsigned short);
	}

	size_t BinaryBuffer::Read(int * Data, size_t Offset)
	{
		if (Size_ < Offset + sizeof(int))
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(int));
		return Offset += sizeof(int);
	}

	size_t BinaryBuffer::Read(unsigned int * Data, size_t Offset)
	{
		if (Size_ < Offset + sizeof(unsigned int))
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(unsigned int));
		return Offset += sizeof(unsigned int);
	}

	size_t BinaryBuffer::Read(long long * Data, size_t Offset)
	{
		if (Size_ < Offset + sizeof(long long))
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(long long));
		return Offset += sizeof(long long);
	}

	size_t BinaryBuffer::Read(unsigned long long * Data, size_t Offset)
	{
		if (Size_ < Offset + sizeof(unsigned long long))
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(unsigned long long));
		return Offset += sizeof(unsigned long long);
	}

	size_t BinaryBuffer::Read(float * Data, size_t Offset)
	{
		if (Size_ < Offset + sizeof(float))
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(float));
		return Offset += sizeof(float);
	}

	size_t BinaryBuffer::Read(double * Data, size_t Offset)
	{
		if (Size_ < Offset + sizeof(double))
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(double));
		return Offset += sizeof(double);
	}

	size_t BinaryBuffer::Read(char ** Data, size_t Offset, size_t Count)
	{
		if (Size_ < Offset + sizeof(char) * Count)
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(*Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(char) * Count);
		return Offset += (sizeof(char) * Count);
	}

	size_t BinaryBuffer::Read(unsigned char ** Data, size_t Offset, size_t Count)
	{
		if (Size_ < Offset + sizeof(unsigned char) * Count)
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(*Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(unsigned char) * Count);
		return Offset += (sizeof(unsigned char) * Count);
	}

	size_t BinaryBuffer::Read(short ** Data, size_t Offset, size_t Count)
	{
		if (Size_ < Offset + sizeof(short) * Count)
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(*Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(short) * Count);
		return Offset += (sizeof(short) * Count);
	}

	size_t BinaryBuffer::Read(unsigned short ** Data, size_t Offset, size_t Count)
	{
		if (Size_ < Offset + sizeof(unsigned short) * Count)
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(*Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(unsigned short) * Count);
		return Offset += (sizeof(unsigned short) * Count);
	}

	size_t BinaryBuffer::Read(int ** Data, size_t Offset, size_t Count)
	{
		if (Size_ < Offset + sizeof(int) * Count)
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(*Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(int) * Count);
		return Offset += (sizeof(int) * Count);
	}

	size_t BinaryBuffer::Read(unsigned int ** Data, size_t Offset, size_t Count)
	{
		if (Size_ < Offset + sizeof(unsigned int) * Count)
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(*Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(unsigned int) * Count);
		return Offset += (sizeof(unsigned int) * Count);
	}

	size_t BinaryBuffer::Read(long long ** Data, size_t Offset, size_t Count)
	{
		if (Size_ < Offset + sizeof(long long) * Count)
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(*Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(long long) * Count);
		return Offset += (sizeof(long long) * Count);
	}

	size_t BinaryBuffer::Read(unsigned long long ** Data, size_t Offset, size_t Count)
	{
		if (Size_ < Offset + sizeof(unsigned long long) * Count)
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(*Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(unsigned long long) * Count);
		return Offset += (sizeof(unsigned long long) * Count);
	}

	size_t BinaryBuffer::Read(float ** Data, size_t Offset, size_t Count)
	{
		if (Size_ < Offset + sizeof(float) * Count)
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(*Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(float) * Count);
		return Offset += (sizeof(float) * Count);
	}

	size_t BinaryBuffer::Read(double ** Data, size_t Offset, size_t Count)
	{
		if (Size_ < Offset + sizeof(double) * Count)
		{
			throw std::exception("Offset out of bounds");
		}
		memcpy(*Data, reinterpret_cast<char*>(Data_) + Offset, sizeof(double) * Count);
		return Offset += (sizeof(double) * Count);
	}

	void const * BinaryBuffer::GetData() const
	{
		return Data_;
	}

	size_t BinaryBuffer::GetSize() const
	{
		return Size_;
	}

	void BinaryBuffer::AddRawData(void * Data, size_t Size)
	{
		if (Size_ + Size >= Capacity_)
		{
			ResizeAndCopy(Capacity_ * 2);
		}
		memcpy(reinterpret_cast<char*>(Data_) + Size_, Data, Size);
		Size_ += Size;
	}

	void BinaryBuffer::ResizeAndCopy(size_t NewCapacity)
	{
		void * tmp = Data_;
		Data_ = calloc(NewCapacity, 1);
		memcpy(Data_, tmp, Size_);
		free(tmp);

		Capacity_ = NewCapacity;
	}

} }