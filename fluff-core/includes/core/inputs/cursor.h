#pragma once

#include <common.h>

namespace luminos
{
	class LUMINOS_API Cursor
	{
		static float X_; 
		static float Y_;
		static float Px_;
		static float Py_;
	public:
		
		/*
			Gets the mouse's X position

			Returns mouse x
		*/
		inline static double X()
		{
			return X_;
		}

		/*
			Gets the mouse's Y position

			Returns mouse y
		*/
		inline static double Y()
		{
			return Y_;
		}

		/*
			Gets the change in the mouse's X position between this and last update

			Returns mouse's change in x position
		*/
		inline static double DeltaX()
		{
			return X_ - Px_;
		}

		/*
			Gets the change in the mouse's Y position between this and last update

			Returns mouse's change in y position
		*/
		inline static double DeltaY()
		{
			return Y_ - Py_;
		}

		/*
			Updates the mouse's information
		*/
		inline static void Update(float X, float Y)
		{
			Px_ = X_;
			Py_ = Y_;

			X_ = X;
			Y_ = Y;

		}
	};
}