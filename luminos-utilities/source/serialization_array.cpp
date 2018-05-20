#include <serialization_array.h>

#include <serialization_utilities.h>

namespace luminos { namespace utilities
{
	SerializationArray SerializationArray::Create(std::string Name, bool* Values, size_t Count)
	{
		SerializationArray result;
		result.Name = Name;
		result.Type = BOOLEAN_FIELD;
		result.Count = Count;
		result.Value = new unsigned char[Count * sizeof(bool)];
		size_t Pointer = 0;
		SerializationUtils::WriteToArray(Values, result.Value, Pointer, Count);
		return result;
	}

	SerializationArray SerializationArray::Create(std::string Name, unsigned char* Values, size_t Count)
	{
		SerializationArray result;
		result.Name = Name;
		result.Type = UCHAR_FIELD;
		result.Count = Count;
		result.Value = new unsigned char[Count * sizeof(unsigned char)];
		size_t Pointer = 0;
		SerializationUtils::WriteToArray(Values, result.Value, Pointer, Count);
		return result;
	}

	SerializationArray SerializationArray::Create(std::string Name, char* Values, size_t Count)
	{
		SerializationArray result;
		result.Name = Name;
		result.Type = CHAR_FIELD;
		result.Count = Count;
		result.Value = new unsigned char[Count * sizeof(char)];
		size_t Pointer = 0;
		SerializationUtils::WriteToArray(Values, result.Value, Pointer, Count);
		return result;
	}

	SerializationArray SerializationArray::Create(std::string Name, unsigned short* Values, size_t Count)
	{
		SerializationArray result;
		result.Name = Name;
		result.Type = USHORT_FIELD;
		result.Count = Count;
		result.Value = new unsigned char[Count * sizeof(unsigned char)];
		size_t Pointer = 0;
		SerializationUtils::WriteToArray(Values, result.Value, Pointer, Count);
		return result;
	}

	SerializationArray SerializationArray::Create(std::string Name, short* Values, size_t Count)
	{
		SerializationArray result;
		result.Name = Name;
		result.Type = SHORT_FIELD;
		result.Count = Count;
		result.Value = new unsigned char[Count * sizeof(short)];
		size_t Pointer = 0;
		SerializationUtils::WriteToArray(Values, result.Value, Pointer, Count);
		return result;
	}

	SerializationArray SerializationArray::Create(std::string Name, unsigned int* Values, size_t Count)
	{
		SerializationArray result;
		result.Name = Name;
		result.Type = UINT_FIELD;
		result.Count = Count;
		result.Value = new unsigned char[Count * sizeof(unsigned int)];
		size_t Pointer = 0;
		SerializationUtils::WriteToArray(Values, result.Value, Pointer, Count);
		return result;
	}

	SerializationArray SerializationArray::Create(std::string Name, int* Values, size_t Count)
	{
		SerializationArray result;
		result.Name = Name;
		result.Type = INT_FIELD;
		result.Count = Count;
		result.Value = new unsigned char[Count * sizeof(int)];
		size_t Pointer = 0;
		SerializationUtils::WriteToArray(Values, result.Value, Pointer, Count);
		return result;
	}

	SerializationArray SerializationArray::Create(std::string Name, unsigned long long* Values, size_t Count)
	{
		SerializationArray result;
		result.Name = Name;
		result.Type = ULONG_FIELD;
		result.Count = Count;
		result.Value = new unsigned char[Count * sizeof(unsigned long long)];
		size_t Pointer = 0;
		SerializationUtils::WriteToArray(Values, result.Value, Pointer, Count);
		return result;
	}

	SerializationArray SerializationArray::Create(std::string Name, long long* Values, size_t Count)
	{
		SerializationArray result;
		result.Name = Name;
		result.Type = LONG_FIELD;
		result.Count = Count;
		result.Value = new unsigned char[Count * sizeof(long long)];
		size_t Pointer = 0;
		SerializationUtils::WriteToArray(Values, result.Value, Pointer, Count);
		return result;
	}

	SerializationArray SerializationArray::Create(std::string Name, float* Values, size_t Count)
	{
		SerializationArray result;
		result.Name = Name;
		result.Type = FLOAT_FIELD;
		result.Count = Count;
		result.Value = new unsigned char[Count * sizeof(float)];
		size_t Pointer = 0;
		SerializationUtils::WriteToArray(Values, result.Value, Pointer, Count);
		return result;
	}

	SerializationArray SerializationArray::Create(std::string Name, long double* Values, size_t Count)
	{
		SerializationArray result;
		result.Name = Name;
		result.Type = DOUBLE_FIELD;
		result.Count = Count;
		result.Value = new unsigned char[Count * sizeof(long double)];
		size_t Pointer = 0;
		SerializationUtils::WriteToArray(Values, result.Value, Pointer, Count);
		return result;
	}

	void SerializationArray::ToBool(SerializationArray& Src, bool*& Dest)
	{
		size_t Pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, Pointer, Src.Count);
	}

	void SerializationArray::ToUnsignedChar(SerializationArray& Src, unsigned char*& Dest)
	{
		size_t Pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, Pointer, Src.Count);
	}

	void SerializationArray::ToChar(SerializationArray& Src, char*& Dest)
	{
		size_t Pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, Pointer, Src.Count);
	}

	void SerializationArray::ToUnsignedShort(SerializationArray& Src, unsigned short*& Dest)
	{
		size_t Pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, Pointer, Src.Count);
	}

	void SerializationArray::ToShort(SerializationArray& Src, short*& Dest)
	{
		size_t Pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, Pointer, Src.Count);
	}

	void SerializationArray::ToUnsignedInt(SerializationArray& Src, unsigned*& Dest)
	{
		size_t Pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, Pointer, Src.Count);
	}

	void SerializationArray::ToInt(SerializationArray& Src, int*& Dest)
	{
		size_t Pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, Pointer, Src.Count);
	}

	void SerializationArray::ToUnsignedLong(SerializationArray& Src, unsigned long long*& Dest)
	{
		size_t Pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, Pointer, Src.Count);
	}

	void SerializationArray::ToLong(SerializationArray& Src, long long*& Dest)
	{
		size_t Pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, Pointer, Src.Count);
	}

	void SerializationArray::ToFloat(SerializationArray& Src, float*& Dest)
	{
		size_t Pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, Pointer, Src.Count);
	}

	void SerializationArray::ToDouble(SerializationArray& Src, long double*& Dest)
	{
		size_t Pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, Pointer, Src.Count);
	}

	void SerializationArray::Serialize(unsigned char* Dest, size_t& Pointer) const
	{
		const auto name_array = const_cast<char*>(Name.c_str());
		const auto namelength = Name.length();

		SerializationUtils::WriteToArray(ARRAY, Dest, Pointer);
		SerializationUtils::WriteToArray(namelength, Dest, Pointer);
		SerializationUtils::WriteToArray(name_array, Dest, Pointer, namelength);
		SerializationUtils::WriteToArray(NULL_TERMINATION, Dest, Pointer);
		SerializationUtils::WriteToArray(Type, Dest, Pointer);
		SerializationUtils::WriteToArray(Count, Dest, Pointer);
		if (Type == BOOLEAN_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, Count * sizeof(bool));
		}
		else if (Type == UCHAR_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, Count * sizeof(unsigned char));
		}
		else if (Type == CHAR_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, Count * sizeof(char));
		}
		else if (Type == USHORT_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, Count * sizeof(unsigned short));
		}
		else if (Type == SHORT_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, Count * sizeof(short));
		}
		else if (Type == UINT_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, Count * sizeof(unsigned int));
		}
		else if (Type == INT_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, Count * sizeof(int));
		}
		else if (Type == ULONG_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, Count * sizeof(unsigned long long));
		}
		else if (Type == LONG_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, Count * sizeof(long long));
		}
		else if (Type == FLOAT_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, Count * sizeof(float));
		}
		else if (Type == DOUBLE_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, Count * sizeof(long double));
		}
	}

	void SerializationArray::Deserialize(unsigned char* Src, SerializationArray& Dest, size_t& Pointer)
	{
		size_t namelength;
		SerializationUtils::ReadFromArray(Src, namelength, Pointer);
		const auto name = static_cast<char*>(malloc(namelength + 1));
		memset(name, 0, namelength + 1);
		SerializationUtils::ReadFromArray(Src, name, Pointer, namelength + 1);
		Dest.Name = name;
		free(name);
		SerializationUtils::ReadFromArray(Src, Dest.Type, Pointer);
		SerializationUtils::ReadFromArray(Src, Dest.Count, Pointer);
		if (Dest.Type == BOOLEAN_FIELD)
		{
			Dest.Value = new unsigned char[Dest.Count * sizeof(bool)];
			SerializationUtils::ReadFromArray(Src, reinterpret_cast<bool*>(Dest.Value), Pointer, Dest.Count);
		}
		else if (Dest.Type == UCHAR_FIELD)
		{
			Dest.Value = new unsigned char[Dest.Count * sizeof(unsigned char)];
			SerializationUtils::ReadFromArray(Src, static_cast<unsigned char*>(Dest.Value), Pointer, Dest.Count);
		}
		else if (Dest.Type == CHAR_FIELD)
		{
			Dest.Value = new unsigned char[Dest.Count * sizeof(char)];
			SerializationUtils::ReadFromArray(Src, reinterpret_cast<char*>(Dest.Value), Pointer, Dest.Count);
		}
		else if (Dest.Type == USHORT_FIELD)
		{
			Dest.Value = new unsigned char[Dest.Count * sizeof(unsigned short)];
			SerializationUtils::ReadFromArray(Src, reinterpret_cast<unsigned short*>(Dest.Value), Pointer, Dest.Count);
		}
		else if (Dest.Type == SHORT_FIELD)
		{
			Dest.Value = new unsigned char[Dest.Count * sizeof(short)];
			SerializationUtils::ReadFromArray(Src, reinterpret_cast<short*>(Dest.Value), Pointer, Dest.Count);
		}
		else if (Dest.Type == UINT_FIELD)
		{
			Dest.Value = new unsigned char[Dest.Count * sizeof(unsigned int)];
			SerializationUtils::ReadFromArray(Src, reinterpret_cast<unsigned int*>(Dest.Value), Pointer, Dest.Count);
		}
		else if (Dest.Type == INT_FIELD)
		{
			Dest.Value = new unsigned char[Dest.Count * sizeof(int)];
			SerializationUtils::ReadFromArray(Src, reinterpret_cast<int*>(Dest.Value), Pointer, Dest.Count * sizeof(int));
		}
		else if (Dest.Type == ULONG_FIELD)
		{
			Dest.Value = new unsigned char[Dest.Count * sizeof(unsigned long long)];
			SerializationUtils::ReadFromArray(Src, reinterpret_cast<unsigned long long*>(Dest.Value), Pointer, Dest.Count);
		}
		else if (Dest.Type == LONG_FIELD)
		{
			Dest.Value = new unsigned char[Dest.Count * sizeof(long long)];
			SerializationUtils::ReadFromArray(Src, reinterpret_cast<long long*>(Dest.Value), Pointer, Dest.Count);
		}
		else if (Dest.Type == FLOAT_FIELD)
		{
			Dest.Value = new unsigned char[Dest.Count * sizeof(float)];
			SerializationUtils::ReadFromArray(Src, reinterpret_cast<float*>(Dest.Value), Pointer, Dest.Count);
		}
		else if (Dest.Type == DOUBLE_FIELD)
		{
			Dest.Value = new unsigned char[Dest.Count * sizeof(long double)];
			SerializationUtils::ReadFromArray(Src, reinterpret_cast<long double*>(Dest.Value), Pointer, Dest.Count);
		}
	}

	size_t SerializationArray::Size() const
	{
		size_t result = 0;
		result += sizeof(uint32_t); // value type
		result += sizeof(size_t); // name length
		result += Name.length(); // name
		result += sizeof(NULL_TERMINATION); // null terminator at end of name
		result += sizeof(Type); // data type
		result += sizeof(Count); // number of values

		switch (Type) // bytes in array, Count * sizeof(type)
		{
		case BOOLEAN_FIELD:
		{
			result += Count * sizeof(bool);
			break;
		}
		case CHAR_FIELD:
		case UCHAR_FIELD:
		{
			result += Count * sizeof(char);
			break;
		}
		case SHORT_FIELD:
		case USHORT_FIELD:
		{
			result += Count * sizeof(short);
			break;
		}
		case INT_FIELD:
		case UINT_FIELD:
		{
			result += Count * sizeof(int);
			break;
		}
		case LONG_FIELD:
		case ULONG_FIELD:
		{
			result += Count * sizeof(long long);
			break;
		}
		case FLOAT_FIELD:
		{
			result += Count * sizeof(float);
			break;
		}
		case DOUBLE_FIELD:
		{
			result += Count * sizeof(long double);
			break;
		}
		default:
		{
			break;
		}
		}
		return result;
	}

	void SerializationArray::Free()
	{
		if (Value != nullptr)
		{
			delete[] Value;
			Value = nullptr;
		}
	}
} }