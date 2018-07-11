#include <serial/types/serialized_vector.h>

#include <serial/types/arrays.h>

namespace fluff { namespace serial {

	UCharVector::UCharVector(std::vector<unsigned char> Data)
		: Data_(Data)
	{	}

	IArray * UCharVector::GetArray(std::string Name)
	{
		IArray * arr = new UCharArray(Name.c_str(), Data_.data(), Data_.size());
		return arr;
	}

	CharVector::CharVector(std::vector<char> Data)
		: Data_(Data)
	{	}

	IArray * CharVector::GetArray(std::string Name)
	{
		IArray * arr = new CharArray(Name.c_str(), Data_.data(), Data_.size());
		return arr;
	}

	UShortVector::UShortVector(std::vector<unsigned short> Data)
		: Data_(Data)
	{	}

	IArray * UShortVector::GetArray(std::string Name)
	{
		IArray * arr = new UShortArray(Name.c_str(), Data_.data(), Data_.size());
		return arr;
	}

	ShortVector::ShortVector(std::vector<short> Data)
		: Data_(Data)
	{	}

	IArray * ShortVector::GetArray(std::string Name)
	{
		IArray * arr = new ShortArray(Name.c_str(), Data_.data(), Data_.size());
		return arr;
	}

	UIntVector::UIntVector(std::vector<unsigned int> Data)
		: Data_(Data)
	{	}

	IArray * UIntVector::GetArray(std::string Name)
	{
		IArray * arr = new UIntArray(Name.c_str(), Data_.data(), Data_.size());
		return arr;
	}

	IntVector::IntVector(std::vector<int> Data)
		: Data_(Data)
	{	}

	IArray * IntVector::GetArray(std::string Name)
	{
		IArray * arr = new IntArray(Name.c_str(), Data_.data(), Data_.size());
		return arr;
	}

	ULongVector::ULongVector(std::vector<unsigned long long> Data)
		: Data_(Data)
	{	}

	IArray * ULongVector::GetArray(std::string Name)
	{
		IArray * arr = new ULongArray(Name.c_str(), Data_.data(), Data_.size());
		return arr;
	}

	LongVector::LongVector(std::vector<long long> Data)
		: Data_(Data)
	{	}

	IArray * LongVector::GetArray(std::string Name)
	{
		IArray * arr = new LongArray(Name.c_str(), Data_.data(), Data_.size());
		return arr;
	}

	FloatVector::FloatVector(std::vector<float> Data)
		: Data_(Data)
	{	}

	IArray * FloatVector::GetArray(std::string Name)
	{
		IArray * arr = new FloatArray(Name.c_str(), Data_.data(), Data_.size());
		return arr;
	}

	DoubleVector::DoubleVector(std::vector<double> Data)
		: Data_(Data)
	{	}

	IArray * DoubleVector::GetArray(std::string Name)
	{
		IArray * arr = new DoubleArray(Name.c_str(), Data_.data(), Data_.size());
		return arr;
	}

}	}