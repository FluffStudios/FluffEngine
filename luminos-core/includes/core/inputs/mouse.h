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

namespace luminos
{
	class LUMINOS_API Mouse
	{
	private:
		static bool keys[MAX_BUTTON_COUNT];
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
				LUMINOS_ASSERT()
				LUMINOS_ERROR_FUNC("Invalid KeyID!")
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
				LUMINOS_ASSERT()
					LUMINOS_ERROR_FUNC("Invalid KeyID!")
			}
#endif
			keys[KeyID] = Status;
		}
	};
}