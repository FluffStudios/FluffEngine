#pragma once

#include <common.h>

namespace fluff { namespace utilities
{
	
#define NULL_TERMINATION '\0'

#pragma region COMPONENTS

#define STRING		0x0
#define FIELD		0x1
#define ARRAY		0x2
#define OBJECT		0x3
#define MODEL		0x4
#define TEXTURE		0x5
#define DATABASE	0x6

#pragma endregion

#pragma region MODELS

#define MESH		0x1
#define BONE		0x2
#define ANIMBEHAVE	0x3
#define MESHANIM	0x4
#define ANIMATION	0x5
#define NODEANIM	0x6

#pragma endregion

#pragma region FIELD_TYPES

#define BOOLEAN_FIELD 0x0
#define   UCHAR_FIELD 0x1
#define    CHAR_FIELD 0x2
#define  USHORT_FIELD 0x3
#define   SHORT_FIELD 0x4
#define    UINT_FIELD 0x5
#define     INT_FIELD 0x6
#define   ULONG_FIELD 0x7
#define    LONG_FIELD 0x8
#define   FLOAT_FIELD 0x9 
#define  DOUBLE_FIELD 0xA
#define   UNDEF_FIELD 0xB

#pragma endregion

	class FLUFF_API SerializationUtils
	{
	public:
		static void WriteToArray(bool Src, unsigned char* Dest, size_t& Pointer);
		static void WriteToArray(bool *Src, unsigned char* Dest, size_t& Pointer, size_t Count);
		static void WriteToArray(unsigned char Src, unsigned char* Dest, size_t& Pointer);
		static void WriteToArray(unsigned char *Src, unsigned char* Dest, size_t& Pointer, size_t Count);
		static void WriteToArray(char Src, unsigned char* Dest, size_t& Pointer);
		static void WriteToArray(char *Src, unsigned char* Dest, size_t& Pointer, size_t Count);
		static void WriteToArray(unsigned short Src, unsigned char* Dest, size_t& Pointer);
		static void WriteToArray(unsigned short *Src, unsigned char* Dest, size_t& Pointer, size_t Count);
		static void WriteToArray(short Src, unsigned char* Dest, size_t& Pointer);
		static void WriteToArray(short *Src, unsigned char* Dest, size_t& Pointer, size_t Count);
		static void WriteToArray(unsigned int Src, unsigned char* Dest, size_t& Pointer);
		static void WriteToArray(unsigned int *Src, unsigned char* Dest, size_t& Pointer, size_t Count);
		static void WriteToArray(int Src, unsigned char* Dest, size_t& Pointer);
		static void WriteToArray(int* Src, unsigned char* Dest, size_t& Pointer, size_t Count);
		static void WriteToArray(unsigned long long Src, unsigned char* Dest, size_t& Pointer);
		static void WriteToArray(unsigned long long* Src, unsigned char* Dest, size_t& Pointer, size_t Count);
		static void WriteToArray(long long Src, unsigned char* Dest, size_t& Pointer);
		static void WriteToArray(long long* Src, unsigned char* Dest, size_t& Pointer, size_t Count);
		static void WriteToArray(float Src, unsigned char* Dest, size_t& Pointer);
		static void WriteToArray(float* Src, unsigned char* Dest, size_t& Pointer, size_t Count);
		static void WriteToArray(long double Src, unsigned char* Dest, size_t& Pointer);
		static void WriteToArray(long double* Src, unsigned char* Dest, size_t& Pointer, size_t Count);
		static void ReadFromArray(unsigned char* Src, bool& Dest, size_t& Pointer);
		static void ReadFromArray(unsigned char* Src, bool* Dest, size_t& Pointer, size_t Count);
		static void ReadFromArray(unsigned char* Src, unsigned char& Dest, size_t& Pointer);
		static void ReadFromArray(unsigned char* Src, unsigned char* Dest, size_t& Pointer, size_t Count);
		static void ReadFromArray(unsigned char* Src, char& Dest, size_t& Pointer);
		static void ReadFromArray(unsigned char* Src, char* Dest, size_t& Pointer, size_t Count);
		static void ReadFromArray(unsigned char* Src, unsigned short& Dest, size_t& Pointer);
		static void ReadFromArray(unsigned char* Src, unsigned short* Dest, size_t& Pointer, size_t Count);
		static void ReadFromArray(unsigned char* Src, short& Dest, size_t& Pointer);
		static void ReadFromArray(unsigned char* Src, short* Dest, size_t& Pointer, size_t Count);
		static void ReadFromArray(unsigned char* Src, unsigned int& Dest, size_t& Pointer);
		static void ReadFromArray(unsigned char* Src, unsigned int* Dest, size_t& Pointer, size_t Count);
		static void ReadFromArray(unsigned char* Src, int& Dest, size_t& Pointer);
		static void ReadFromArray(unsigned char* Src, int* Dest, size_t& Pointer, size_t Count);
		static void ReadFromArray(unsigned char* Src, unsigned long long& Dest, size_t& Pointer);
		static void ReadFromArray(unsigned char* Src, unsigned long long* Dest, size_t& Pointer, size_t Count);
		static void ReadFromArray(unsigned char* Src, long long& Dest, size_t& Pointer);
		static void ReadFromArray(unsigned char* Src, long long* Dest, size_t& Pointer, size_t Count);
		static void ReadFromArray(unsigned char* Src, float& Dest, size_t& Pointer);
		static void ReadFromArray(unsigned char* Src, float* Dest, size_t& Pointer, size_t Count);
		static void ReadFromArray(unsigned char* Src, long double& Dest, size_t& Pointer);
		static void ReadFromArray(unsigned char* Src, long double* Dest, size_t& Pointer, size_t Count);
	};

} }