#include <serial/types/primitives.h>
#include <serial/types/types_defines.h>

#define NULL_TERMINATOR (unsigned char) 0

namespace fluff { namespace serial {

	size_t CharPrimitive::Size_ = sizeof(char);

	CharPrimitive::CharPrimitive(const char * Name, char Value)
		: Primitive<char>(Name, Value)
	{
	}

	const size_t CharPrimitive::GetTypeSize() const
	{
		return CharPrimitive::Size_;
	}

	const char CharPrimitive::GetTypeHandle() const
	{
		return CHAR;
	}

	size_t UCharPrimitive::Size_ = sizeof(unsigned char);

	UCharPrimitive::UCharPrimitive(const char * Name, unsigned char Value)
		: Primitive<unsigned char>(Name, Value)
	{
	}

	const size_t UCharPrimitive::GetTypeSize() const
	{
		return UCharPrimitive::Size_;
	}

	const char UCharPrimitive::GetTypeHandle() const
	{
		return UCHAR;
	}

	size_t ShortPrimitive::Size_ = sizeof(short);

	ShortPrimitive::ShortPrimitive(const char * Name, short Value)
		: Primitive<short>(Name, Value)
	{
	}

	const size_t ShortPrimitive::GetTypeSize() const
	{
		return ShortPrimitive::Size_;
	}

	const char ShortPrimitive::GetTypeHandle() const
	{
		return SHORT;
	}

	size_t UShortPrimitive::Size_ = sizeof(unsigned short);

	UShortPrimitive::UShortPrimitive(const char * Name, unsigned short Value)
		: Primitive<unsigned short>(Name, Value)
	{
	}

	const size_t UShortPrimitive::GetTypeSize() const
	{
		return UShortPrimitive::Size_;
	}

	const char UShortPrimitive::GetTypeHandle() const
	{
		return USHORT;
	}

	size_t IntPrimitive::Size_ = sizeof(int);

	IntPrimitive::IntPrimitive(const char * Name, int Value)
		: Primitive<int>(Name, Value)
	{
	}

	const size_t IntPrimitive::GetTypeSize() const
	{
		return IntPrimitive::Size_;
	}

	const char IntPrimitive::GetTypeHandle() const
	{
		return INT;
	}

	size_t UIntPrimitive::Size_ = sizeof(unsigned int);

	UIntPrimitive::UIntPrimitive(const char * Name, unsigned int Value)
		: Primitive<unsigned int>(Name, Value)
	{
	}

	const size_t UIntPrimitive::GetTypeSize() const
	{
		return UIntPrimitive::Size_;
	}

	const char UIntPrimitive::GetTypeHandle() const
	{
		return UINT;
	}

	size_t LongPrimitive::Size_ = sizeof(long long);

	LongPrimitive::LongPrimitive(const char * Name, long long Value)
		: Primitive<long long>(Name, Value)
	{
	}

	const size_t LongPrimitive::GetTypeSize() const
	{
		return LongPrimitive::Size_;
	}

	const char LongPrimitive::GetTypeHandle() const
	{
		return LONG;
	}

	size_t ULongPrimitive::Size_ = sizeof(unsigned long long);

	ULongPrimitive::ULongPrimitive(const char * Name, unsigned long long Value)
		: Primitive<unsigned long long>(Name, Value)
	{
	}

	const size_t ULongPrimitive::GetTypeSize() const
	{
		return ULongPrimitive::Size_;
	}

	const char ULongPrimitive::GetTypeHandle() const
	{
		return ULONG;
	}

	size_t FloatPrimitive::Size_ = sizeof(float);

	FloatPrimitive::FloatPrimitive(const char * Name, float Value)
		: Primitive<float>(Name, Value)
	{
	}

	const size_t FloatPrimitive::GetTypeSize() const
	{
		return FloatPrimitive::Size_;
	}

	const char FloatPrimitive::GetTypeHandle() const
	{
		return FLOAT;
	}

	size_t DoublePrimitive::Size_ = sizeof(float);

	DoublePrimitive::DoublePrimitive(const char * Name, double Value)
		: Primitive<double>(Name, Value)
	{
	}

	const size_t DoublePrimitive::GetTypeSize() const
	{
		return DoublePrimitive::Size_;
	}

	const char DoublePrimitive::GetTypeHandle() const
	{
		return DOUBLE;
	}

	void Write(IArchive * Archive, IPrimitive * Prim)
	{
		Archive->GetBuffer()->Add(PRIMITIVE);
		Archive->GetBuffer()->Add(Prim->GetNameLength());
		Archive->GetBuffer()->Add(Prim->GetName(), Prim->GetNameLength());
		Archive->GetBuffer()->Add(Prim->GetTypeHandle());
		switch (Prim->GetTypeHandle())
		{
		case UCHAR:
			Archive->GetBuffer()->Add(*reinterpret_cast<unsigned char*>(Prim->GetDataPtr()));
			break;
		case CHAR:
			Archive->GetBuffer()->Add(*reinterpret_cast<char*>(Prim->GetDataPtr()));
			break;
		case USHORT:
			Archive->GetBuffer()->Add(*reinterpret_cast<unsigned short*>(Prim->GetDataPtr()));
			break;
		case SHORT:
			Archive->GetBuffer()->Add(*reinterpret_cast<short*>(Prim->GetDataPtr()));
			break;
		case UINT:
			Archive->GetBuffer()->Add(*reinterpret_cast<unsigned int*>(Prim->GetDataPtr()));
			break;
		case INT:
			Archive->GetBuffer()->Add(*reinterpret_cast<int*>(Prim->GetDataPtr()));
			break;
		case ULONG:
			Archive->GetBuffer()->Add(*reinterpret_cast<unsigned long long*>(Prim->GetDataPtr()));
			break;
		case LONG:
			Archive->GetBuffer()->Add(*reinterpret_cast<long*>(Prim->GetDataPtr()));
			break;
		case FLOAT:
			Archive->GetBuffer()->Add(*reinterpret_cast<float*>(Prim->GetDataPtr()));
			break;
		case DOUBLE:
			Archive->GetBuffer()->Add(*reinterpret_cast<double*>(Prim->GetDataPtr()));
			break;
		default:
			throw std::exception("Invalid Type Handle!");
		}
	}

	void FLUFF_API Read(IArchive * Archive, IPrimitive ** Prim, size_t * Pointer)
	{
		size_t ptr = *Pointer;

		unsigned char objectType;
		size_t nameLength;
		char * name;
		char typeHandle;

		ptr = Archive->GetBuffer()->Read(&objectType, ptr);
		FLUFF_ASSERT(objectType == PRIMITIVE)

		ptr = Archive->GetBuffer()->Read(&nameLength, ptr);
		FLUFF_ASSERT(nameLength > 0)

		name = static_cast<char*>(calloc(nameLength, 1));
		ptr = Archive->GetBuffer()->Read(&name, ptr, nameLength);

		ptr = Archive->GetBuffer()->Read(&typeHandle, ptr);

		switch (typeHandle)
		{
		case UCHAR:
		{
			unsigned char value;
			ptr = Archive->GetBuffer()->Read(&value, ptr);
			*Prim = new UCharPrimitive(name, value);
			break;
		}
		case CHAR:
		{
			char value;
			ptr = Archive->GetBuffer()->Read(&value, ptr);
			*Prim = new CharPrimitive(name, value);
			break;
		}
		case USHORT:
		{
			unsigned short value;
			ptr = Archive->GetBuffer()->Read(&value, ptr);
			*Prim = new UShortPrimitive(name, value);
			break;
		}
		case SHORT:
		{
			short value;
			ptr = Archive->GetBuffer()->Read(&value, ptr);
			*Prim = new ShortPrimitive(name, value);
			break;
		}
		case UINT:
		{
			unsigned int value;
			ptr = Archive->GetBuffer()->Read(&value, ptr);
			*Prim = new UIntPrimitive(name, value);
			break;
		}
		case INT:
		{
			int value;
			ptr = Archive->GetBuffer()->Read(&value, ptr);
			*Prim = new IntPrimitive(name, value);
			break;
		}
		case ULONG:
		{
			unsigned long long value;
			ptr = Archive->GetBuffer()->Read(&value, ptr);
			*Prim = new ULongPrimitive(name, value);
			break;
		}
		case LONG:
		{
			long long value;
			ptr = Archive->GetBuffer()->Read(&value, ptr);
			*Prim = new LongPrimitive(name, value);
			break;
		}
		case FLOAT:
		{
			float value;
			ptr = Archive->GetBuffer()->Read(&value, ptr);
			*Prim = new FloatPrimitive(name, value);
			break;
		}
		case DOUBLE:
		{
			double value;
			ptr = Archive->GetBuffer()->Read(&value, ptr);
			*Prim = new DoublePrimitive(name, value);
			break;
		}
		default:
			throw std::exception("Invalid Type Handle!");
		}

		free(name);

		*Pointer = ptr;
	}

}	}