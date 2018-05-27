#pragma once

namespace luminos {

	template<typename T>
	class Listenable
	{
	public:
		/*
			Gets the value of the listenable object

			Returns listenable value
		*/
		virtual T GetValue() const = 0;

		/*
			Sets the value of the listenable object

			Value - new value
		*/
		virtual void SetValue(const T& Value) = 0;

		/*
			Sets the value of the listenable object
		
			Value - new value
		*/
		virtual void SetValue(T&& Value) = 0;
	};

}