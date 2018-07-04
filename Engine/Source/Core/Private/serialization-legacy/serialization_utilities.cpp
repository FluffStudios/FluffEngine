#include <serialization-legacy/serialization_utilities.h>

#include <cstring>

namespace fluff { namespace utilities
{
	
	void SerializationUtils::WriteToArray(bool Src, unsigned char *Dest, size_t& Pointer)
	{
		const auto data = reinterpret_cast<unsigned char*>(&Src);
		for (size_t i = 0; i < sizeof Src; i++)
		{
			Dest[i + Pointer] = data[i];
		}

		Pointer += sizeof(bool);
	}

	void SerializationUtils::WriteToArray(bool *Src, unsigned char *Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) WriteToArray(Src[i], Dest, Pointer);
	}

	void SerializationUtils::WriteToArray(unsigned char Src, unsigned char* Dest, size_t& Pointer)
	{
		const auto data = reinterpret_cast<unsigned char*>(&Src);
		for (size_t i = 0; i < sizeof Src; i++)
		{
			Dest[i + Pointer] = data[i];
		}

		Pointer += sizeof(unsigned char);
	}

	void SerializationUtils::WriteToArray(unsigned char* Src, unsigned char* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) WriteToArray(Src[i], Dest, Pointer);
	}

	void SerializationUtils::WriteToArray(char Src, unsigned char* Dest, size_t& Pointer)
	{
		const auto data = reinterpret_cast<unsigned char*>(&Src);
		for (size_t i = 0; i < sizeof Src; i++)
		{
			Dest[i + Pointer] = data[i];
		}

		Pointer += sizeof(unsigned char);
	}

	void SerializationUtils::WriteToArray(char* Src, unsigned char* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) WriteToArray(Src[i], Dest, Pointer);
	}

	void SerializationUtils::WriteToArray(unsigned short Src, unsigned char* Dest, size_t& Pointer)
	{
		const auto data = reinterpret_cast<unsigned char*>(&Src);
		for (size_t i = 0; i < sizeof Src; i++)
		{
			Dest[i + Pointer] = data[i];
		}

		Pointer += sizeof(unsigned short);
	}

	void SerializationUtils::WriteToArray(unsigned short* Src, unsigned char* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) WriteToArray(Src[i], Dest, Pointer);
	}

	void SerializationUtils::WriteToArray(short Src, unsigned char* Dest, size_t& Pointer)
	{
		const auto data = reinterpret_cast<unsigned char*>(&Src);
		for (size_t i = 0; i < sizeof Src; i++)
		{
			Dest[i + Pointer] = data[i];
		}

		Pointer += sizeof(short);
	}

	void SerializationUtils::WriteToArray(short* Src, unsigned char* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) WriteToArray(Src[i], Dest, Pointer);
	}

	void SerializationUtils::WriteToArray(unsigned int Src, unsigned char* Dest, size_t& Pointer)
	{
		const auto data = reinterpret_cast<unsigned char*>(&Src);
		for (size_t i = 0; i < sizeof Src; i++)
		{
			Dest[i + Pointer] = data[i];
		}

		Pointer += sizeof(unsigned int);
	}

	void SerializationUtils::WriteToArray(unsigned int* Src, unsigned char* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) WriteToArray(Src[i], Dest, Pointer);
	}

	void SerializationUtils::WriteToArray(int Src, unsigned char* Dest, size_t& Pointer)
	{
		const auto data = reinterpret_cast<unsigned char*>(&Src);
		for (size_t i = 0; i < sizeof Src; i++)
		{
			Dest[i + Pointer] = data[i];
		}

		Pointer += sizeof(int);
	}

	void SerializationUtils::WriteToArray(int* Src, unsigned char* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) WriteToArray(Src[i], Dest, Pointer);
	}

	void SerializationUtils::WriteToArray(unsigned long long Src, unsigned char* Dest, size_t& Pointer)
	{
		const auto data = reinterpret_cast<unsigned char*>(&Src);
		for (size_t i = 0; i < sizeof Src; i++)
		{
			Dest[i + Pointer] = data[i];
		}

		Pointer += sizeof(unsigned long long);
	}

	void SerializationUtils::WriteToArray(unsigned long long* Src, unsigned char* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) WriteToArray(Src[i], Dest, Pointer);
	}

	void SerializationUtils::WriteToArray(long long Src, unsigned char* Dest, size_t& Pointer)
	{
		const auto data = reinterpret_cast<unsigned char*>(&Src);
		for (size_t i = 0; i < sizeof Src; i++)
		{
			Dest[i + Pointer] = data[i];
		}

		Pointer += sizeof(long long);
	}

	void SerializationUtils::WriteToArray(long long* Src, unsigned char* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) WriteToArray(Src[i], Dest, Pointer);
	}

	void SerializationUtils::WriteToArray(float Src, unsigned char* Dest, size_t& Pointer)
	{
		unsigned char temp[sizeof Src];
		memcpy(temp, &Src, sizeof(float));
		for (auto i = 0; i < sizeof Src; i++)
		{
			Dest[Pointer + i] = temp[i];
		}
		Pointer += sizeof(float);
	}

	void SerializationUtils::WriteToArray(float* Src, unsigned char* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) WriteToArray(Src[i], Dest, Pointer);
	}

	void SerializationUtils::WriteToArray(long double Src, unsigned char* Dest, size_t& Pointer)
	{
		unsigned char temp[sizeof Src];
		memcpy(temp, &Src, sizeof(long double));
		for (auto i = 0; i < sizeof Src; i++)
		{
			Dest[Pointer + i] = temp[i];
		}
		Pointer += sizeof(long double);
	}

	void SerializationUtils::WriteToArray(long double* Src, unsigned char* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) WriteToArray(Src[i], Dest, Pointer);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, bool& Dest, size_t& Pointer)
	{
		memcpy(&Dest, &Src[Pointer], sizeof(bool));
		Pointer += sizeof(bool);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, bool* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) ReadFromArray(Src, Dest[i], Pointer);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, unsigned char& Dest, size_t& Pointer)
	{
		memcpy(&Dest, &Src[Pointer], sizeof(unsigned char));
		Pointer += sizeof(unsigned char);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, unsigned char* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) ReadFromArray(Src, Dest[i], Pointer);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, char& Dest, size_t& Pointer)
	{
		memcpy(&Dest, &Src[Pointer], sizeof(char));
		Pointer += sizeof(char);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, char* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) ReadFromArray(Src, Dest[i], Pointer);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, unsigned short& Dest, size_t& Pointer)
	{
		memcpy(&Dest, &Src[Pointer], sizeof(unsigned short));
		Pointer += sizeof(unsigned short);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, unsigned short* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) ReadFromArray(Src, Dest[i], Pointer);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, short& Dest, size_t& Pointer)
	{
		memcpy(&Dest, &Src[Pointer], sizeof(short));
		Pointer += sizeof(short);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, short* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) ReadFromArray(Src, Dest[i], Pointer);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, unsigned int& Dest, size_t& Pointer)
	{
		memcpy(&Dest, &Src[Pointer], sizeof(unsigned int));
		Pointer += sizeof(unsigned int);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, unsigned int* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) ReadFromArray(Src, Dest[i], Pointer);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, int& Dest, size_t& Pointer)
	{
		memcpy(&Dest, &Src[Pointer], sizeof(int));
		Pointer += sizeof(int);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, int* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) ReadFromArray(Src, Dest[i], Pointer);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, unsigned long long& Dest, size_t& Pointer)
	{
		memcpy(&Dest, &Src[Pointer], sizeof(unsigned long long));
		Pointer += sizeof(unsigned long long);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, unsigned long long* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) ReadFromArray(Src, Dest[i], Pointer);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, long long& Dest, size_t& Pointer)
	{
		memcpy(&Dest, &Src[Pointer], sizeof(long long));
		Pointer += sizeof(long long);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, long long* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) ReadFromArray(Src, Dest[i], Pointer);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, float& Dest, size_t& Pointer)
	{
		memcpy(&Dest, &Src[Pointer], sizeof(float));
		Pointer += sizeof(float);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, float* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) ReadFromArray(Src, Dest[i], Pointer);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, long double& Dest, size_t& Pointer)
	{
		memcpy(&Dest, &Src[Pointer], sizeof(long double));
		Pointer += sizeof(long double);
	}

	void SerializationUtils::ReadFromArray(unsigned char* Src, long double* Dest, size_t& Pointer, size_t Count)
	{
		for (auto i = 0; i < Count; i++) ReadFromArray(Src, Dest[i], Pointer);
	}
} }