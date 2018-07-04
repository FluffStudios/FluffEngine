#pragma once

#include <common.h>
#include <core/beans/property.h>
#include <vector>
#include <memory>

namespace fluff
{
	class FLUFF_API Window
	{
		const char* Name_;
		Property<unsigned int> Width_;
		Property<unsigned int> Height_;
		Property<unsigned int> FramebufferWidth_;
		Property<unsigned int> FramebufferHeight_;
		bool Vsync_;
		bool Fullscreen_;

		static std::vector<Window*> Windows_;
		static Window* CurrentHandle_;
		static bool GlewInit_;

		Window(const char* Name, unsigned int Width, unsigned int Height, bool Vsync, bool Fullscreen);

		struct WindowImpl;
		std::unique_ptr<WindowImpl> Impl_;
	public:
		/*
			Deconstructs window
			
			Releases GLFW if it is the last window open
		 */
		~Window();

		/*
			Updates window and inputs
			
			Returns if window was successfully updated
		 */
		bool Update() const;

		/*
			Kills window
		 */
		void CloseWindow();

		/*
			Gets the window's width
			
			Returns width
		 */
		inline Property<unsigned int>& GetWidth() { return Width_; }

		/*
			Gets the window's height
			
			Returns height
		 */
		inline Property<unsigned int>& GetHeight() { return Height_; }

		/*
			Resizes window
			
			Width - new width
			Height - new height
		 */
		void SetDimensions(unsigned int Width, unsigned int Height);
		inline void SetWidth(unsigned int Width) { Width_ = Width; }
		inline void SetHeight(unsigned int Height) { Height_ = Height; }

		/*
			Creates a new window
		 
			Name - Name of window
			Width - Width of window
			Height - Height of window
			Vsync - Should window be vertically synchronized
			Fullscreen - Should window be fullscreen
			Returns window handle
		 */
		static Window* CreateWindow(const char * Name, unsigned int Width, unsigned int Height, bool Vsync, bool Fullscreen);
		
		/*
			Gets the handle to the current window
			
			Returns current window's handle
		 */
		static Window* GetCurrentWindow();

		/*
			Sets the current window
			
			WindowHandle - window's handle
		 */
		static void SetCurrentWindow(Window *WindowHandle);

		/*
			Gets the handle pointer
			
			Returns Handle Pointer
		 */
		void * GetHandle() const;

		/*
			Set window framebuffer size

			Width - buffer width
			Height - buffer height
		*/
		void SetBufferSize(uint32_t Width, uint32_t Height)
		{
			this->FramebufferWidth_ = Width;
			this->FramebufferHeight_ = Height;
		}

		/*
			Gets the border size of the window
		*/
		void GetWindowBorders(int * left, int * top, int * right, int * bottom);

		Property<unsigned int>& GetBufferWidth()
		{
			return FramebufferWidth_;
		}

		Property<unsigned int>& GetBufferHeight()
		{
			return FramebufferHeight_;
		}
	private:
		void Initialize();
	};
}
