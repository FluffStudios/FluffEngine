#pragma once

#include <string>
#include <fstream>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

template <class Type>
static void SendToFile(std::string FileName, Type & Serial)
{
	std::ofstream out(FileName.c_str(), std::ios::binary);
	cereal::BinaryOutputArchive arch(out);
	arch(Serial);
	out.close();
}

template <class Type>
static void SendToJSON(std::string FileName, Type & Serial)
{
	std::ofstream out(FileName.c_str(), std::ios::out);
	cereal::JSONOutputArchive arch(out);
	arch(Serial);
	out.close();
}

template <class Type>
static void ReadFromFile(std::string FileName, Type & Serial)
{
	std::ifstream in(FileName.c_str(), std::ios::binary);
	if (!in.is_open())
	{
		LUMINOS_ERROR_FUNC("File does not exist!")
		LUMINOS_ASSERT(false)
		return;
	}
	cereal::BinaryInputArchive arch(in);
	arch(Serial);
	in.close();
}

template <class Type>
static void ReadFromJSON(std::string FileName, Type & Serial)
{
	std::ifstream in(FileName.c_str());
	if (!in.is_open())
	{
		LUMINOS_ERROR_FUNC("File does not exist!")
		LUMINOS_ASSERT(false)
		return;
	}
	cereal::JSONInputArchive arch(in);
	arch(Serial);
	in.close();
}