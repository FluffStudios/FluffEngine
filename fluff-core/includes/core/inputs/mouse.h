#pragma once

#include <common.h>
#include <debug.h>

#define MAX_BUTTON_COUNT		32

#define MOUSE_BUTTON_1         0
#define MOUSE_BUTTON_2         1
#define MOUSE_BUTTON_3         2
#define MOUSE_BUTTON_4         3
#define MOUSE_BUTTON_5         4
#define MOUSE_BUTTON_6         5
#define MOUSE_BUTTON_7         6
#define MOUSE_BUTTON_8         7
#define MOUSE_BUTTON_LAST      MOUSE_BUTTON_8
#define MOUSE_BUTTON_LEFT      MOUSE_BUTTON_1
#define MOUSE_BUTTON_RIGHT     MOUSE_BUTTON_2
#define MOUSE_BUTTON_MIDDLE    MOUSE_BUTTON_3

namespace fluff
{
	class FLUFF_API Mouse
	{
	private:
		static bool keys[MAX_BUTTON_COUNT];
		static float scrollX;
		static float scrollY;
	public:
		/*
			Checks if key is pressed
			
			KeyID - Key button ID number
			Returns if key is pressed
		 */
		inline static bool IsKeyPressed(unsigned int KeyID)
		{
#if defined(DEBUG) || defined(_DEBUG)
			if (KeyID < 0 || KeyID > MAX_BUTTON_COUNT) 
			{
				FLUFF_ASSERT(false)
				FLUFF_ERROR_FUNC("Invalid KeyID!")
			}
#endif
			return keys[KeyID];
		}

		/*
			Updates key status
			
			KeyID - keycode
			Status - status of key
		 */
		inline static void Update(unsigned int KeyID, bool Status)
		{
#if defined(DEBUG) || defined(_DEBUG)
			if (KeyID < 0 || KeyID > MAX_BUTTON_COUNT)
			{
				FLUFF_ASSERT(false)
				FLUFF_ERROR_FUNC("Invalid KeyID!")
			}
#endif
			Mouse::keys[KeyID] = Status;
		}

		/*
			Updates scroll status

			ScrollX - change in x
			ScrollY - change in y
		*/
		inline static void Update(double ScrollX, double ScrollY)
		{
			Mouse::scrollX = ScrollX;
			Mouse::scrollY = ScrollY;
		}

		/*
			Gets the scroll x value

			Returns scroll x
		*/
		inline static float ScrollX()
		{
			return Mouse::scrollX;
		}

		/*
			Gets the scroll y value

			Returns scroll y
		*/
		inline static float ScrollY()
		{
			return Mouse::scrollY;
		}
	};
}