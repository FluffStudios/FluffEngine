#include <serialization-legacy/serialization_field.h>
#include <serialization-legacy/serialization_utilities.h>

namespace fluff { namespace utilities
{
	void SerializationField::Free()
	{
		if (Value != nullptr)
		{
			free(Value);
			Value = nullptr;
		}
	}

	void SerializationField::Serialize(unsigned char* Dest, size_t& Pointer) const
	{
		const auto name_array = const_cast<char*>(Name.c_str());
		const auto namelength = Name.length();

		SerializationUtils::WriteToArray(FIELD, Dest, Pointer);
		SerializationUtils::WriteToArray(namelength, Dest, Pointer);
		SerializationUtils::WriteToArray(name_array, Dest, Pointer, namelength);
		SerializationUtils::WriteToArray(NULL_TERMINATION, Dest, Pointer);
		SerializationUtils::WriteToArray(Type, Dest, Pointer);
		if (Type == BOOLEAN_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, sizeof(bool));
		}
		else if (Type == UCHAR_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, sizeof(unsigned char));
		}
		else if (Type == CHAR_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, sizeof(char));
		}
		else if (Type == USHORT_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, sizeof(unsigned short));
		}
		else if (Type == SHORT_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, sizeof(short));
		}
		else if (Type == UINT_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, sizeof(unsigned int));
		}
		else if (Type == INT_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, sizeof(int));
		}
		else if (Type == ULONG_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, sizeof(unsigned long long));
		}
		else if (Type == LONG_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, sizeof(long long));
		}
		else if (Type == FLOAT_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, sizeof(float));
		}
		else if (Type == DOUBLE_FIELD)
		{
			SerializationUtils::WriteToArray(Value, Dest, Pointer, sizeof(long double));
		}
	}

	void SerializationField::Deserialize(unsigned char* Src, SerializationField& Dest, size_t& Pointer)
	{
		size_t namelength;
		SerializationUtils::ReadFromArray(Src, namelength, Pointer);
		char* Name = new char[namelength + 1];
		SerializationUtils::ReadFromArray(Src, Name, Pointer, namelength + 1);
		Dest.Name = Name;
		delete[] Name;
		SerializationUtils::ReadFromArray(Src, Dest.Type, Pointer);
		if (Dest.Type == BOOLEAN_FIELD)
		{
			Dest.Value = reinterpret_cast<unsigned char*>(new bool);
			SerializationUtils::ReadFromArray(Src, Dest.Value, Pointer, sizeof(bool));
		}
		else if (Dest.Type == UCHAR_FIELD)
		{
			Dest.Value = static_cast<unsigned char*>(new unsigned char);
			SerializationUtils::ReadFromArray(Src, Dest.Value, Pointer, sizeof(unsigned char));
		}
		else if (Dest.Type == CHAR_FIELD)
		{
			Dest.Value = reinterpret_cast<unsigned char*>(new char);
			SerializationUtils::ReadFromArray(Src, Dest.Value, Pointer, sizeof(char));
		}
		else if (Dest.Type == USHORT_FIELD)
		{
			Dest.Value = reinterpret_cast<unsigned char*>(new unsigned short);
			SerializationUtils::ReadFromArray(Src, Dest.Value, Pointer, sizeof(unsigned short));
		}
		else if (Dest.Type == SHORT_FIELD)
		{
			Dest.Value = reinterpret_cast<unsigned char*>(new short);
			SerializationUtils::ReadFromArray(Src, Dest.Value, Pointer, sizeof(short));
		}
		else if (Dest.Type == UINT_FIELD)
		{
			Dest.Value = reinterpret_cast<unsigned char*>(new unsigned int);
			SerializationUtils::ReadFromArray(Src, Dest.Value, Pointer, sizeof(unsigned int));
		}
		else if (Dest.Type == INT_FIELD)
		{
			Dest.Value = reinterpret_cast<unsigned char*>(new int);
			SerializationUtils::ReadFromArray(Src, Dest.Value, Pointer, sizeof(int));
		}
		else if (Dest.Type == ULONG_FIELD)
		{
			Dest.Value = reinterpret_cast<unsigned char*>(new unsigned long long);
			SerializationUtils::ReadFromArray(Src, Dest.Value, Pointer, sizeof(unsigned long long));
		}
		else if (Dest.Type == LONG_FIELD)
		{
			Dest.Value = reinterpret_cast<unsigned char*>(new long long);
			SerializationUtils::ReadFromArray(Src, Dest.Value, Pointer, sizeof(long long));
		}
		else if (Dest.Type == FLOAT_FIELD)
		{
			Dest.Value = Dest.Value = reinterpret_cast<unsigned char*>(new float);
			SerializationUtils::ReadFromArray(Src, Dest.Value, Pointer, sizeof(float));
		}
		else if (Dest.Type == DOUBLE_FIELD)
		{
			Dest.Value = Dest.Value = reinterpret_cast<unsigned char*>(new long double);
			SerializationUtils::ReadFromArray(Src, Dest.Value, Pointer, sizeof(long double));
		}
	}

	SerializationField SerializationField::Create(std::string Name, bool Value)
	{
		SerializationField field;
		field.Name = Name;
		field.Type = BOOLEAN_FIELD;
		field.Value = static_cast<unsigned char*>(malloc(sizeof(bool)));
		size_t pointer = 0;
		SerializationUtils::WriteToArray(Value, field.Value, pointer);
		return field;
	}

	SerializationField SerializationField::Create(std::string Name, unsigned char Value)
	{
		SerializationField field;
		field.Name = Name;
		field.Type = UCHAR_FIELD;
		field.Value = static_cast<unsigned char*>(malloc(sizeof(unsigned char)));
		size_t pointer = 0;
		SerializationUtils::WriteToArray(Value, field.Value, pointer);
		return field;
	}

	SerializationField SerializationField::Create(std::string Name, char Value)
	{
		SerializationField field;
		field.Name = Name;
		field.Type = CHAR_FIELD;
		field.Value = static_cast<unsigned char*>(malloc(sizeof(char)));
		size_t pointer = 0;
		SerializationUtils::WriteToArray(Value, field.Value, pointer);
		return field;
	}

	SerializationField SerializationField::Create(std::string Name, unsigned short Value)
	{
		SerializationField field;
		field.Name = Name;
		field.Type = USHORT_FIELD;
		field.Value = static_cast<unsigned char*>(malloc(sizeof(unsigned short)));
		size_t pointer = 0;
		SerializationUtils::WriteToArray(Value, field.Value, pointer);
		return field;
	}

	SerializationField SerializationField::Create(std::string Name, short Value)
	{
		SerializationField field;
		field.Name = Name;
		field.Type = SHORT_FIELD;
		field.Value = static_cast<unsigned char*>(malloc(sizeof(short)));
		size_t pointer = 0;
		SerializationUtils::WriteToArray(Value, field.Value, pointer);
		return field;
	}

	SerializationField SerializationField::Create(std::string Name, unsigned int Value)
	{
		SerializationField field;
		field.Name = Name;
		field.Type = UINT_FIELD;
		field.Value = static_cast<unsigned char*>(malloc(sizeof(unsigned int)));
		size_t pointer = 0;
		SerializationUtils::WriteToArray(Value, field.Value, pointer);
		return field;
	}

	SerializationField SerializationField::Create(std::string Name, int Value)
	{
		SerializationField field;
		field.Name = Name;
		field.Type = INT_FIELD;
		field.Value = static_cast<unsigned char*>(malloc(sizeof(int)));
		size_t pointer = 0;
		SerializationUtils::WriteToArray(Value, field.Value, pointer);
		return field;
	}

	SerializationField SerializationField::Create(std::string Name, unsigned long long Value)
	{
		SerializationField field;
		field.Name = Name;
		field.Type = ULONG_FIELD;
		field.Value = static_cast<unsigned char*>(malloc(sizeof(unsigned long long)));
		size_t pointer = 0;
		SerializationUtils::WriteToArray(Value, field.Value, pointer);
		return field;
	}

	SerializationField SerializationField::Create(std::string Name, long long Value)
	{
		SerializationField field;
		field.Name = Name;
		field.Type = LONG_FIELD;
		field.Value = static_cast<unsigned char*>(malloc(sizeof(long long)));
		size_t pointer = 0;
		SerializationUtils::WriteToArray(Value, field.Value, pointer);
		return field;
	}

	SerializationField SerializationField::Create(std::string Name, float Value)
	{
		SerializationField field;
		field.Name = Name;
		field.Type = FLOAT_FIELD;
		field.Value = static_cast<unsigned char*>(malloc(sizeof(float)));
		size_t pointer = 0;
		SerializationUtils::WriteToArray(Value, field.Value, pointer);
		return field;
	}

	SerializationField SerializationField::Create(std::string Name, long double Value)
	{
		SerializationField field;
		field.Name = Name;
		field.Type = DOUBLE_FIELD;
		field.Value = static_cast<unsigned char*>(malloc(sizeof(long double)));
		size_t pointer = 0;
		SerializationUtils::WriteToArray(Value, field.Value, pointer);
		return field;
	}

	void SerializationField::ToBool(SerializationField& Src, bool& Dest)
	{
		size_t pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, pointer);
	}

	void SerializationField::ToUnsignedChar(SerializationField& Src, unsigned char& Dest)
	{
		size_t pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, pointer);
	}

	void SerializationField::ToChar(SerializationField& Src, char& Dest)
	{
		size_t pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, pointer);
	}

	void SerializationField::ToUnsignedShort(SerializationField& Src, unsigned short& Dest)
	{
		size_t pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, pointer);
	}

	void SerializationField::ToShort(SerializationField& Src, short& Dest)
	{
		size_t pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, pointer);
	}

	void SerializationField::ToUnsignedInt(SerializationField& Src, unsigned int& Dest)
	{
		size_t pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, pointer);
	}

	void SerializationField::ToInt(SerializationField& Src, int& Dest)
	{
		size_t pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, pointer);
	}

	void SerializationField::ToUnsignedLong(SerializationField& Src, unsigned long long& Dest)
	{
		size_t pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, pointer);
	}

	void SerializationField::ToLong(SerializationField& Src, long long& Dest)
	{
		size_t pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, pointer);
	}

	void SerializationField::ToFloat(SerializationField& Src, float& Dest)
	{
		size_t pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, pointer);
	}

	void SerializationField::ToDouble(SerializationField& Src, long double& Dest)
	{
		size_t pointer = 0;
		SerializationUtils::ReadFromArray(Src.Value, Dest, pointer);
	}

	size_t SerializationField::Size() const
	{
		size_t result = 0;
		result += sizeof(uint32_t); // value type
		result += sizeof(size_t); //namelength
		result += Name.length(); // name
		result += sizeof(unsigned char); // null termination of name
		result += sizeof(unsigned int); // type
		switch (Type) // value
		{
		case BOOLEAN_FIELD: return result + sizeof(bool);
		case UCHAR_FIELD:
		case CHAR_FIELD: return result + sizeof(char);
		case USHORT_FIELD:
		case SHORT_FIELD: return result + sizeof(short);
		case UINT_FIELD:
		case INT_FIELD: return result + sizeof(int);
		case ULONG_FIELD:
		case LONG_FIELD: return result + sizeof(long long);
		case FLOAT_FIELD: return result + sizeof(float);
		case DOUBLE_FIELD: return result + sizeof(long double);
		default:
			break;
		}
		return result + 1;
	}
} }