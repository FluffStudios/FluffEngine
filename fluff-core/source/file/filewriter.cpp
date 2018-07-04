#include <file/filewriter.h>

#include <stdio.h>

void fluff::utilities::FileWriter::WriteToFile(std::string FileName, std::string Contents)
{

	FILE * file;
	fopen_s(&file, FileName.c_str(), "wt+");
	fprintf_s(file, Contents.c_str());
	if (fclose(file)) __debugbreak();

}
