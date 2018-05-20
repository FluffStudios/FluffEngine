#include <core/lookup_table.h>

namespace luminos
{
	std::unordered_map<std::string, int64_t> ComponentLookupTable::IDMap_;

	std::unordered_map<std::string, void*(*)()> GeneratorTable::map;

	void * GeneratorTable::Generate(const std::string & TypeName)
	{
		if (map.find(TypeName) == map.end()) return nullptr;
		else
		{
			return map.find(TypeName)->second();
		}
	}
}