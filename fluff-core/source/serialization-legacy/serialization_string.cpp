#include <serialization-legacy/serialization_string.h>
#include <serialization-legacy/serialization_utilities.h>

namespace fluff { namespace utilities
{
	size_t SerializationString::Size() const
	{
		size_t size = 0;
		size += sizeof STRING;
		size += sizeof(size_t);
		size += sizeof(Name.length());
		size += sizeof NULL_TERMINATION;
		size += sizeof(size_t);
		size += sizeof(Value.length());
		size += sizeof NULL_TERMINATION;
		return size;
	}

	void SerializationString::Serialize(unsigned char* Dest, size_t& Pointer) const
	{
		const auto nameArray = const_cast<char*>(Name.c_str());
		const auto valueArray = const_cast<char*>(Value.c_str());
		const auto namelength = Name.length();
		const auto valuelength = Value.length();

		SerializationUtils::WriteToArray(STRING, Dest, Pointer);
		SerializationUtils::WriteToArray(namelength, Dest, Pointer);
		SerializationUtils::WriteToArray(nameArray, Dest, Pointer, namelength);
		SerializationUtils::WriteToArray(NULL_TERMINATION, Dest, Pointer);
		SerializationUtils::WriteToArray(valuelength, Dest, Pointer);
		SerializationUtils::WriteToArray(valueArray, Dest, Pointer, valuelength);
		SerializationUtils::WriteToArray(NULL_TERMINATION, Dest, Pointer);
	}

	void SerializationString::Deserialize(unsigned char* Src, SerializationString& Dest, size_t& Pointer)
	{
		size_t name_length;
		SerializationUtils::ReadFromArray(Src, name_length, Pointer);
		const auto name = new char[name_length + 1];
		memset(name, 0, name_length * sizeof(char) + 1);
		SerializationUtils::ReadFromArray(Src, name, Pointer, name_length + 1);
		uint32_t value_length;
		SerializationUtils::ReadFromArray(Src, value_length, Pointer);
		const auto value = new char[value_length + 1];
		memset(value, 0, value_length * sizeof(char) + 1);
		SerializationUtils::ReadFromArray(Src, value, Pointer, value_length + 1);
		Dest.Name = std::string(name);
		Dest.Value = std::string(value);
	}

	void SerializationString::Free()
	{
		Name = Value = "";
	}
} }