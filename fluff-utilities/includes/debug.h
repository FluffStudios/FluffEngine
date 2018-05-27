#pragma once
#define DEBUG
#include <iostream>

#define LUMINOS_API_EXPORT

#ifdef LUMINOS_API_EXPORT
#define LUMINOS_API __declspec(dllexport)
#endif

inline void LuminosErrorMsg(const char * File, unsigned int Line, const char * Description)
{
#if defined(_DEBUG) || defined(DEBUG)
	printf("Error in line %s on line %d\n", File, Line);
	printf("Error Description: %s", Description);
	printf("\n\n");
#endif
}

inline void LuminosAssert(bool Condition = false)
{
#if defined(_DEBUG) || defined (DEBUG)
	if (!Condition) __debugbreak();
#endif
}

#define LUMINOS_ERROR_FUNC(Description) \
	LuminosErrorMsg(__FILE__, __LINE__, Description);

#define LUMINOS_SHADER_ERROR_FUNC(File, Line, Description) \
	LuminosErrorMsg(File, Line, Description);

#define LUMINOS_ASSERT(Condition) \
	LuminosAssert(Condition);


typedef unsigned char		uint8_t;
typedef signed char			int8_t;
typedef unsigned short		uint16_t;
typedef signed short		int16_t;
typedef unsigned int		uint32_t;
typedef signed int			int32_t;
typedef unsigned long long	uint64_t;
typedef signed long long	int64_t;
typedef float				float32_t;
typedef double				double32_t;
typedef long double			double64_t;

#define POSITION_ATTRIB		0
#define UV_ATTRIB			1
#define NORMAL_ATTRIB		2
#define TANGENT_ATTRIB		3
#define BITANGENT_ATTRIB	4
#define DIFFUSE_ATTRIB		5
#define SPECULAR_ATTRIB		6

#define SHADER_BUFFER_OFFSET	256
#define TEXTURE_BUFFER_OFFSET	512

#ifndef LUMINOS_ASSERTION
#define LUMINOS_ASSERTION
inline void luminos_assertion(bool condition, const char * sig)
{
	static auto failed = false;
	std::cout << sig;
	if (condition)
	{
		if (!failed)
			system("Color 02");
		std::cout << " Test Passed" << std::endl;
	}
	else
	{
		system("Color 04");
		std::cout << " Test Failed" << std::endl;
		failed = true;
	}
}
#endif

#if defined(_MSC_VER) && !defined(METHOD)

#define METHOD __FUNCSIG__

#endif

#ifndef ASSERT_TRUE
#define ASSERT_TRUE(condition) \
	luminos_assertion(condition, METHOD);
#endif