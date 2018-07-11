#pragma once

#include <serial/types/arrays.h>

#include <string>
#include <vector>

namespace fluff { namespace serial {

	class ISerialVector 
	{
	public:
		virtual IArray * GetArray(std::string Name) = 0;
	};

	class UCharVector : public ISerialVector
	{
		std::vector<unsigned char> Data_;
	public:
		UCharVector(std::vector<unsigned char> Data);
		IArray * GetArray(std::string Name);
	};

	class CharVector : public ISerialVector
	{
		std::vector<char> Data_;
	public:
		CharVector(std::vector<char> Data);
		IArray * GetArray(std::string Name);
	};

	class UShortVector : public ISerialVector
	{
		std::vector<unsigned short> Data_;
	public:
		UShortVector(std::vector<unsigned short> Data);
		IArray * GetArray(std::string Name);
	};

	class ShortVector : public ISerialVector
	{
		std::vector<short> Data_;
	public:
		ShortVector(std::vector<short> Data);
		IArray * GetArray(std::string Name);
	};

	class UIntVector : public ISerialVector
	{
		std::vector<unsigned int> Data_;
	public:
		UIntVector(std::vector<unsigned int> Data);
		IArray * GetArray(std::string Name);
	};

	class IntVector : public ISerialVector
	{
		std::vector<int> Data_;
	public:
		IntVector(std::vector<int> Data);
		IArray * GetArray(std::string Name);
	};

	class ULongVector : public ISerialVector
	{
		std::vector<unsigned long long> Data_;
	public:
		ULongVector(std::vector<unsigned long long> Data);
		IArray * GetArray(std::string Name);
	};

	class LongVector : public ISerialVector
	{
		std::vector<long long> Data_;
	public:
		LongVector(std::vector<long long> Data);
		IArray * GetArray(std::string Name);
	};

	class FloatVector : public ISerialVector
	{
		std::vector<float> Data_;
	public:
		FloatVector(std::vector<float> Data);
		IArray * GetArray(std::string Name);
	};

	class DoubleVector : public ISerialVector
	{
		std::vector<double> Data_;
	public:
		DoubleVector(std::vector<double> Data);
		IArray * GetArray(std::string Name);
	};

} }