#pragma once

#include <functional>
#include <unordered_map>

namespace fluff
{
	template<typename T, typename ... Packing>
	class Registry
	{
	public:
		using constructor = std::function<T*(Packing...)>;
		using constructor_map = std::unordered_map<std::string, constructor>;

		template <typename ... Arguments>
		static T* Create(const std::string & ClassName, Arguments && ... Args)
		{
			if (Constructors().count(ClassName) == 1) return Constructors[ClassName](std::forward<Arguments>(Args)...);
			return nullptr;
		}

		static bool Register(const std::string & ClassName, const constructor & Constructor)
		{
			Constructors()[ClassName] = Constructor;
			return Registered(ClassName);
		}

		static bool Unregister(const std::string & ClassName)
		{
			Constructors().erase(ClassName);
			return Registered(ClassName);
		}

		static bool Registered(const std::string & ClassName)
		{
			return Constructors().count(ClassName) == 1;
		}
	private:
		static constructor_map & Constructors()
		{
			static constructor_map map;
			return map;
		}
	};
}

#define REGISTRY_CTOR(Base, Derived, params, param_names) \
	[](params) -> Base* { return new Derived(param_names); }