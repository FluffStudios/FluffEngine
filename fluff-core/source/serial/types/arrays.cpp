#include <serial/types/arrays.h>

#include <serial/types/types_defines.h>

namespace fluff { namespace serial {

	char * const IArray::GetName() const
	{
		return Name_;
	}

	const size_t IArray::GetNameLength() const
	{
		return NameLength_;
	}

	const size_t IArray::GetDataCount() const
	{
		return Count_;
	}

	size_t CharArray::Size_ = sizeof(char);

	CharArray::CharArray(const char * Name, char * Value, size_t Count)
		: Array<char>(Name, Value, Count)
	{	}

	const size_t CharArray::GetTypeSize() const
	{
		return Size_;
	}

	const char CharArray::GetTypeHandle() const
	{
		return CHAR;
	}

	size_t UCharArray::Size_ = sizeof(unsigned char);

	UCharArray::UCharArray(const char * Name, unsigned char * Value, size_t Count)
		: Array<unsigned char>(Name, Value, Count)
	{	}

	const size_t UCharArray::GetTypeSize() const
	{
		return Size_;
	}

	const char UCharArray::GetTypeHandle() const
	{
		return UCHAR;
	}

	size_t ShortArray::Size_ = sizeof(short);

	ShortArray::ShortArray(const char * Name, short * Value, size_t Count)
		: Array<short>(Name, Value, Count)
	{	}

	const size_t ShortArray::GetTypeSize() const
	{
		return Size_;
	}

	const char ShortArray::GetTypeHandle() const
	{
		return SHORT;
	}

	size_t UShortArray::Size_ = sizeof(unsigned char);

	UShortArray::UShortArray(const char * Name, unsigned short * Value, size_t Count)
		: Array<unsigned short>(Name, Value, Count)
	{	}

	const size_t UShortArray::GetTypeSize() const
	{
		return Size_;
	}

	const char UShortArray::GetTypeHandle() const
	{
		return USHORT;
	}

	size_t IntArray::Size_ = sizeof(int);

	IntArray::IntArray(const char * Name, int * Value, size_t Count)
		: Array<int>(Name, Value, Count)
	{	}

	const size_t IntArray::GetTypeSize() const
	{
		return Size_;
	}

	const char IntArray::GetTypeHandle() const
	{
		return INT;
	}

	size_t UIntArray::Size_ = sizeof(unsigned int);

	UIntArray::UIntArray(const char * Name, unsigned int * Value, size_t Count)
		: Array<unsigned int>(Name, Value, Count)
	{	}

	const size_t UIntArray::GetTypeSize() const
	{
		return Size_;
	}

	const char UIntArray::GetTypeHandle() const
	{
		return UINT;
	}

	size_t LongArray::Size_ = sizeof(long long);

	LongArray::LongArray(const char * Name, long long * Value, size_t Count)
		: Array<long long>(Name, Value, Count)
	{	}

	const size_t LongArray::GetTypeSize() const
	{
		return Size_;
	}

	const char LongArray::GetTypeHandle() const
	{
		return LONG;
	}

	size_t ULongArray::Size_ = sizeof(unsigned long long);

	ULongArray::ULongArray(const char * Name, unsigned long long * Value, size_t Count)
		: Array<unsigned long long>(Name, Value, Count)
	{	}

	const size_t ULongArray::GetTypeSize() const
	{
		return Size_;
	}

	const char ULongArray::GetTypeHandle() const
	{
		return ULONG;
	}

	size_t FloatArray::Size_ = sizeof(float);

	FloatArray::FloatArray(const char * Name, float * Value, size_t Count)
		: Array<float>(Name, Value, Count)
	{	}

	const size_t FloatArray::GetTypeSize() const
	{
		return Size_;
	}

	const char FloatArray::GetTypeHandle() const
	{
		return FLOAT;
	}

	size_t DoubleArray::Size_ = sizeof(double);

	DoubleArray::DoubleArray(const char * Name, double * Value, size_t Count)
		: Array<double>(Name, Value, Count)
	{	}

	const size_t DoubleArray::GetTypeSize() const
	{
		return Size_;
	}

	const char DoubleArray::GetTypeHandle() const
	{
		return DOUBLE;
	}

	void Write(IArchive * Archive, IArray * Ar)
	{
		Archive->GetBuffer()->Add(ARRAY);
		Archive->GetBuffer()->Add(Ar->GetNameLength());
		Archive->GetBuffer()->Add(Ar->GetName(), Ar->GetNameLength());
		Archive->GetBuffer()->Add(Ar->GetTypeHandle());
		Archive->GetBuffer()->Add(Ar->GetDataCount());
		switch (Ar->GetTypeHandle())
		{
		case UCHAR:
			Archive->GetBuffer()->Add(*reinterpret_cast<unsigned char**>(Ar->GetDataPtr()), Ar->GetDataCount());
			break;
		case CHAR:
			Archive->GetBuffer()->Add(*reinterpret_cast<char**>(Ar->GetDataPtr()), Ar->GetDataCount());
			break;
		case USHORT:
			Archive->GetBuffer()->Add(*reinterpret_cast<unsigned short**>(Ar->GetDataPtr()), Ar->GetDataCount());
			break;
		case SHORT:
			Archive->GetBuffer()->Add(*reinterpret_cast<short**>(Ar->GetDataPtr()), Ar->GetDataCount());
			break;
		case UINT:
			Archive->GetBuffer()->Add(*reinterpret_cast<unsigned int**>(Ar->GetDataPtr()), Ar->GetDataCount());
			break;
		case INT:
			Archive->GetBuffer()->Add(*reinterpret_cast<int**>(Ar->GetDataPtr()), Ar->GetDataCount());
			break;
		case ULONG:
			Archive->GetBuffer()->Add(*reinterpret_cast<unsigned long long**>(Ar->GetDataPtr()), Ar->GetDataCount());
			break;
		case LONG:
			Archive->GetBuffer()->Add(*reinterpret_cast<long long**>(Ar->GetDataPtr()), Ar->GetDataCount());
			break;
		case FLOAT:
			Archive->GetBuffer()->Add(*reinterpret_cast<float**>(Ar->GetDataPtr()), Ar->GetDataCount());
			break;
		case DOUBLE:
			Archive->GetBuffer()->Add(*reinterpret_cast<double**>(Ar->GetDataPtr()), Ar->GetDataCount());
			break;
		default:
			throw std::exception("Invalid Type Handle!");
		}
	}

	void Read(IArchive * Archive, IArray ** Ar, size_t * Pointer)
	{
		size_t ptr = *Pointer;

		unsigned char objectType;
		size_t nameLength;
		char * name;
		char typeHandle;
		size_t count;

		ptr = Archive->GetBuffer()->Read(&objectType, ptr);
		FLUFF_ASSERT(objectType == ARRAY)

		ptr = Archive->GetBuffer()->Read(&nameLength, ptr);
		FLUFF_ASSERT(nameLength > 0)

		name = static_cast<char*>(calloc(nameLength, 1));
		ptr = Archive->GetBuffer()->Read(&name, ptr, nameLength);

		ptr = Archive->GetBuffer()->Read(&typeHandle, ptr);
		ptr = Archive->GetBuffer()->Read(&count, ptr);

		switch (typeHandle)
		{
		case UCHAR:
		{
			unsigned char * value = static_cast<unsigned char*>(calloc(count, sizeof(unsigned char)));
			ptr = Archive->GetBuffer()->Read(&value, ptr, count);
			*Ar = new UCharArray(name, value, count);
			break;
		}
		case CHAR:
		{
			char * value = static_cast<char*>(calloc(count, sizeof(char)));
			ptr = Archive->GetBuffer()->Read(&value, ptr, count);
			*Ar = new CharArray(name, value, count);
			break;
		}
		case USHORT:
		{
			unsigned short * value = static_cast<unsigned short*>(calloc(count, sizeof(unsigned short)));
			ptr = Archive->GetBuffer()->Read(&value, ptr, count);
			*Ar = new UShortArray(name, value, count);
			break;
		}
		case SHORT:
		{
			short * value = static_cast<short*>(calloc(count, sizeof(short)));
			ptr = Archive->GetBuffer()->Read(&value, ptr, count);
			*Ar = new ShortArray(name, value, count);
			break;
		}
		case UINT:
		{
			unsigned int * value = static_cast<unsigned int*>(calloc(count, sizeof(unsigned int)));
			ptr = Archive->GetBuffer()->Read(&value, ptr, count);
			*Ar = new UIntArray(name, value, count);
			break;
		}
		case INT:
		{
			int * value = static_cast<int*>(calloc(count, sizeof(int)));
			ptr = Archive->GetBuffer()->Read(&value, ptr, count);
			*Ar = new IntArray(name, value, count);
			break;
		}
		case ULONG:
		{
			unsigned long long * value = static_cast<unsigned long long*>(calloc(count, sizeof(unsigned long long)));
			ptr = Archive->GetBuffer()->Read(&value, ptr, count);
			*Ar = new ULongArray(name, value, count);
			break;
		}
		case LONG:
		{
			long long * value = static_cast<long long*>(calloc(count, sizeof(long long)));
			ptr = Archive->GetBuffer()->Read(&value, ptr, count);
			*Ar = new LongArray(name, value, count);
			break;
		}
		case FLOAT:
		{
			float * value = static_cast<float*>(calloc(count, sizeof(float)));
			ptr = Archive->GetBuffer()->Read(&value, ptr, count);
			*Ar = new FloatArray(name, value, count);
			break;
		}
		case DOUBLE:
		{
			double * value = static_cast<double*>(calloc(count, sizeof(double)));
			ptr = Archive->GetBuffer()->Read(&value, ptr, count);
			*Ar = new DoubleArray(name, value, count);
			break;
		}
		default:
			throw std::exception("Invalid Type Handle!");
		}

		free(name);

		*Pointer = ptr;
	}

}	}