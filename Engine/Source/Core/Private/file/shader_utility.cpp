#include <file/shader_utility.h>

#include <stdexcept>
#include <fstream>
#include <string>
#include <file/filewriter.h>
#include <unordered_map>

namespace fluff { namespace utilities
{
	std::string ShaderUtility::LoadShader(std::string FileName, std::unordered_map<std::string, unsigned> Uniforms)
	{
		std::string result;

		std::ifstream input(FileName.c_str());

		if (input.is_open())
		{
			std::string temp;
			input >> temp;
			
			unsigned long line = 0;

			while(input.good())
			{
				line++;
				if (strcmp(temp.c_str(), "#include") == 0)
				{
					if (input.good())
					{
						input >> temp;
						const auto begin = temp[0];
						const auto end = temp[temp.size() - 1];

						if (begin == '<' && end == '>')
						{
							auto included_file = temp.substr(1, temp.size() - 2);
							result.append(LoadShader(included_file.c_str()));
							result.append("\n");
							input >> temp;
						}
						else
						{
							if (begin != '<')
								FLUFF_SHADER_ERROR_FUNC(FileName.c_str(), line, "Invalid Syntax.  Expected \"<\" before file name.")
							if (end != '>')
								FLUFF_SHADER_ERROR_FUNC(FileName.c_str(), line, "Invalid Syntax.  Expected \">\" after file name.")
						}
					}
					else
					{
						FLUFF_SHADER_ERROR_FUNC(FileName.c_str(), line, "File ended before #include could complete.")
					}
				}
				else
				{
					result.append(temp.c_str());
					getline(input, temp);
					result.append(temp.c_str());
					result.append("\n");
					input >> temp;
					if (!input.good()) result.append(temp.c_str());
				}

			}
		}
		else
		{
			FLUFF_ERROR_FUNC("Could not open file.");
		}

		input.close();

		return result;
	}

	bool ShaderUtility::SaveShader(std::string FileName, std::string Data)
	{
		FileWriter::WriteToFile(FileName, Data);
		auto result = false;
		FILE * file;
		result = fopen_s(&file ,FileName.c_str(), "r");
		if (result) fclose(file);
		return result;
	}
} }
