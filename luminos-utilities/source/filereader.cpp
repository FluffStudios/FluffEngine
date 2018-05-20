#include <filereader.h>

#include <sstream>
#include <fstream>

namespace luminos
{
	std::string utilities::FileReader::ReadTextFile(const char* path)
	{

		std::ifstream stream(path);
		std::stringstream buffer;
		buffer << stream.rdbuf();
		return std::string(buffer.str().c_str());

	}
}
