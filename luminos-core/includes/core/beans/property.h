#pragma once
#include <algorithm>
#include <functional>
#include <vector>
#include <core/beans/listenable.h>

namespace luminos
{
	template<class T>
	class Property : public Listenable<T>
	{
		T Value_;
		std::vector<std::function<void(T, T)>> Function_;
	public:
		/*
			Creates a new property
		*/
		Property() { }
		
		/*
			Creates a new property

			Value - default value
		*/
		explicit Property(T& Value) : Value_(Value) { }

		/*
			Creates a new property

			Value - default value
		*/
		explicit Property(T&& Value) : Value_(std::move(Value)) { }

		/*
			Sets the value of the property

			Value - new value
		*/
		void SetValue(const T& Value)
		{
			if (Value != Value_)
			{
				for (const auto func : Function_)
					func(Value_, Value);
			}
			Value_ = Value;
		}

		/*
			Sets the value of the property

			Value - new value
		*/
		void SetValue(T&& Value)
		{
			if (Value != Value_)
			{
				for (const auto func : Function_)
					func(Value_, Value);
			}
			Value_ = std::move(Value);
		}

		/*
			Sets the value of the property

			Value - new value
			Returns the property
		*/
		Property<T>& operator=(const T& Other)
		{
			SetValue(std::move(Other));
			return *this;
		}

		/*
			Gets the value stored by the property

			Returns property value
		*/
		T GetValue() const
		{
			return Value_;
		}

		/*
			Sets listener for property value change

			Function - std::function requiring two parameters, the old and new property values.  Called
			on value change
		*/
		inline void SetListener(const std::function<void(T, T)> &Function)
		{
			Function_.emplace_back(Function);
		}

		/*
			Removes listener for property value change

			Function - std::function to remove
		*/
		void RemoveListener(const std::function<void(T, T)> &Function)
		{
			size_t index = 0;
			for (auto func : Function_)
			{
				if (GetAddress(Function) == GetAddress(func)) break;
				index++;
			}
			Function_.erase(Function_.begin() + index);
		}
	private:
		template<typename T, typename... U>
		size_t GetAddress(std::function<T(U...)> F) {
			typedef T(fn_type)(U...);
			fn_type ** fn_pointer = F.template target<fn_type*>();
			return static_cast<size_t>(*fn_pointer);
		}
	};

}
