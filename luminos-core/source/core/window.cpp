#include <core/window.h>
#include <core/inputs/cursor.h>
#include <core/inputs/keyboard.h>
#include <core/inputs/mouse.h>
#include <common.h>
#include <gfx/error_callback.h>
#include <ui/nk_ui.h>
#include "gfx/context.h"
#include <core/timer.h>

static void key_callback(GLFWwindow *Window, int Key, int Scancode, int Action, int Mods)
{
	luminos::Keyboard::Update(Key, Action == GLFW_PRESS || Action == GLFW_REPEAT);
}

 static void char_callback(GLFWwindow *window, unsigned int codepoint)
{
	(void) window;
	luminos::ui::nk_glfw3_set_char(codepoint);
}

static void cursor_pos_callback(GLFWwindow *Window, double Xpos, double Ypos)
{
	luminos::Cursor::Update(Xpos, Ypos);
}

static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	luminos::Mouse::Update(button, action == GLFW_PRESS);
}

static void scroll_input(GLFWwindow *window, float xoff, float yoff)
{
	
}

static void window_resize_callback(GLFWwindow *window, int width, int height)
{
	auto win = static_cast<luminos::Window*>(glfwGetWindowUserPointer(window));

	int l, r, t, b;
	glfwGetWindowFrameSize(window, &l, &t, &r, &b);
	
	win->SetWidth(width + l - r);
	win->SetHeight(height + t - b);
	glViewport(0, 0, width + l - r, height + t - b);
}

static void framebuffer_resize_callback(GLFWwindow * window, int width, int height)
{
	auto win = static_cast<luminos::Window*>(glfwGetWindowUserPointer(window));
	win->SetBufferSize(width, height);
}

namespace luminos
{
	std::vector<Window*> Window::Windows_;
	Window* Window::CurrentHandle_;
	bool Window::GlewInit_ = false;

	Window::Window(const char* Name, unsigned int Width, unsigned int Height, bool VSYNC, bool Fullscreen)
		:Name_(Name), Width_(Width), Height_(Height), Vsync_(VSYNC), Fullscreen_(Fullscreen)
	{
		Initialize();
	}

	Window::~Window()
	{
		if (Handle_ != nullptr)
		{
			CloseWindow();
		}

		if (Windows_.size() == 1)
		{
			
			glfwTerminate();
		}
		else
		{
			CurrentHandle_ = Windows_.front();
		}
	}

	bool Window::Update() const
	{
		glfwPollEvents();
		glfwSwapBuffers(Handle_);
		return glfwWindowShouldClose(Handle_) == 0;
	}

	void Window::CloseWindow()
	{
		ui::nk_glfw3_shutdown();
		glfwDestroyWindow(Handle_);
		Handle_ = nullptr;
	}

	Window* Window::CreateWindow(const char* Name, unsigned int Width, unsigned int Height, bool Vsync, bool Fullscreen)
	{
		const auto win = new Window(Name, Width, Height, Vsync, Fullscreen);
		Windows_.push_back(win);
		CurrentHandle_ = win;
		return win;
	}

	Window* Window::GetCurrentWindow()
	{
		return CurrentHandle_;
	}

	void Window::SetCurrentWindow(Window *WindowHandle)
	{
		CurrentHandle_ = WindowHandle;
		glfwFocusWindow(CurrentHandle_->Handle_);
	}

	void Window::GetWindowBorders(int * left, int * top, int * right, int * bottom)
	{
		glfwGetWindowFrameSize(Handle_, left, top, right, bottom);
	}

	void Window::Initialize()
	{
		glfwWindowHint(GLFW_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
#if defined(DEBUG) || defined(_DEBUG)
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
		if (!glfwInit() && !GlewInit_)
		{
			LUMINOS_ERROR_FUNC("Could not initialize GLFW!")
			LUMINOS_ASSERT()
			GlewInit_ = true;
		}

		if (Fullscreen_)
		{
			auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
			Handle_ = glfwCreateWindow(Width_.GetValue(), Height_.GetValue(), Name_, NULL, NULL);
			glfwSetWindowMonitor(Handle_, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else
		{
			Handle_ = glfwCreateWindow(Width_.GetValue(), Height_.GetValue(), Name_, NULL, NULL);
		}

		int x, y;
		glfwGetFramebufferSize(Handle_, &x, &y);
		this->FramebufferWidth_ = x;
		this->FramebufferHeight_ = y;

		LUMINOS_ASSERT(Handle_);

		glfwSetKeyCallback(Handle_, key_callback);
		glfwSetMouseButtonCallback(Handle_, mouse_button_callback);
		glfwSetCursorPosCallback(Handle_, cursor_pos_callback);
		glfwSetWindowUserPointer(Handle_, this);
		glfwSetFramebufferSizeCallback(Handle_, framebuffer_resize_callback);
		glfwMakeContextCurrent(Handle_);
		glfwSwapInterval(Vsync_ ? 1 : 0);

		glfwSetWindowSizeCallback(Handle_, window_resize_callback);
		glfwSetCharCallback(Handle_, char_callback);
		const int result = glewInit();
		if (result != GLEW_OK)
		{
			CloseWindow();
			glfwTerminate();
			LUMINOS_ERROR_FUNC("Could not initialize GLEW!")
			LUMINOS_ASSERT()
		}
#if defined(DEBUG) || defined(_DEBUG)
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(gfx::glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif
		glClearDepth(0);
		glClearColor(0, 0, 0, 1);

	}

}