#include <core/window.h>
#include <core/inputs/cursor.h>
#include <core/inputs/keyboard.h>
#include <core/inputs/mouse.h>
#include <gfx/error_callback.h>
#include "gfx/context.h"
#include <core/timer.h>
#include <glew.h>
#include <glfw3.h>

#include <imgui/imgui.h>
#include <rendering/ui/backend_imgui_wrapper.h>

static void key_callback(GLFWwindow *Window, int Key, int Scancode, int Action, int Mods)
{
	fluff::Keyboard::Update(Key, Action == GLFW_PRESS || Action == GLFW_REPEAT);

    ImGuiIO& io = ImGui::GetIO();
    if (Action == GLFW_PRESS)
        io.KeysDown[Key] = true;
    if (Action == GLFW_RELEASE)
        io.KeysDown[Key] = false;

    (void)Mods; // Modifiers are not reliable across systems
    io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
    io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
}

 static void char_callback(GLFWwindow *window, unsigned int codepoint)
{
}

static void cursor_pos_callback(GLFWwindow *Window, double Xpos, double Ypos)
{
	fluff::Cursor::Update(static_cast<float>(Xpos), static_cast<float>(Ypos));
}

static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	fluff::Mouse::Update(button, action == GLFW_PRESS);
}

static void scroll_input(GLFWwindow *window, double xoff, double yoff)
{
	fluff::Mouse::Update(xoff, yoff);
}

static void window_resize_callback(GLFWwindow *window, int width, int height)
{
	auto win = static_cast<fluff::Window*>(glfwGetWindowUserPointer(window));

	int l, r, t, b;
	glfwGetWindowFrameSize(window, &l, &t, &r, &b);
	
	win->SetWidth(width + l - r);
	win->SetHeight(height + t - b);
	glViewport(0, 0, width + l - r, height + t - b);
}

static void framebuffer_resize_callback(GLFWwindow * window, int width, int height)
{
	auto win = static_cast<fluff::Window*>(glfwGetWindowUserPointer(window));
	win->SetBufferSize(width, height);
}

namespace fluff
{
	struct Window::WindowImpl
	{
		GLFWwindow *Handle_;
	};

	std::vector<Window*> Window::Windows_;
	Window* Window::CurrentHandle_;
	bool Window::GlewInit_ = false;

	Window::Window(const char* Name, unsigned int Width, unsigned int Height, bool VSYNC, bool Fullscreen)
		:Name_(Name), Width_(Width), Height_(Height), Vsync_(VSYNC), Fullscreen_(Fullscreen), Impl_(new Window::WindowImpl)
	{
		Initialize();
	}

	Window::~Window()
	{
		if (Impl_->Handle_ != nullptr)
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
		Mouse::Update(0.0, 0.0);
        glfwPollEvents();

		glfwSwapBuffers(Impl_->Handle_);
		return glfwWindowShouldClose(Impl_->Handle_) == 0;
	}

	void Window::CloseWindow()
	{
		glfwDestroyWindow(Impl_->Handle_);
		Impl_->Handle_ = nullptr;
	}

	void Window::SetDimensions(unsigned int Width, unsigned int Height)
	{
		Width_.SetValue(Width);
		Height_.SetValue(Height);
		glfwSetWindowSize(Impl_->Handle_, Width, Height);
		glViewport(0, 0, Width, Height);
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
		glfwFocusWindow(CurrentHandle_->Impl_->Handle_);
	}

	void * Window::GetHandle() const
	{
		return Impl_->Handle_;
	}

	void Window::GetWindowBorders(int * left, int * top, int * right, int * bottom)
	{
		glfwGetWindowFrameSize(Impl_->Handle_, left, top, right, bottom);
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
			FLUFF_ERROR_FUNC("Could not initialize GLFW!")
			FLUFF_ASSERT(false)
			GlewInit_ = true;
		}

		if (Fullscreen_)
		{
			auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
			Impl_->Handle_ = glfwCreateWindow(Width_.GetValue(), Height_.GetValue(), Name_, NULL, NULL);
			glfwSetWindowMonitor(Impl_->Handle_, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else
		{
			Impl_->Handle_ = glfwCreateWindow(Width_.GetValue(), Height_.GetValue(), Name_, NULL, NULL);
		}

		int x, y;
		glfwGetFramebufferSize(Impl_->Handle_, &x, &y);
		this->FramebufferWidth_ = x;
		this->FramebufferHeight_ = y;

		FLUFF_ASSERT(Impl_->Handle_);

		glfwSetKeyCallback(Impl_->Handle_, key_callback);
		glfwSetMouseButtonCallback(Impl_->Handle_, mouse_button_callback);
		glfwSetCursorPosCallback(Impl_->Handle_, cursor_pos_callback);
		glfwSetWindowUserPointer(Impl_->Handle_, this);
		glfwSetFramebufferSizeCallback(Impl_->Handle_, framebuffer_resize_callback);
		glfwSetScrollCallback(Impl_->Handle_, scroll_input);
		glfwMakeContextCurrent(Impl_->Handle_);
		glfwSwapInterval(Vsync_ ? 1 : 0);

		glfwSetWindowSizeCallback(Impl_->Handle_, window_resize_callback);
		glfwSetCharCallback(Impl_->Handle_, char_callback);
		const int result = glewInit();
		if (result != GLEW_OK)
		{
			CloseWindow();
			glfwTerminate();
			FLUFF_ERROR_FUNC("Could not initialize GLEW!")
			FLUFF_ASSERT(false)
		}
#if defined(DEBUG) || defined(_DEBUG)
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(gfx::glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif
		glClearDepth(0);
		glClearColor(0, 0, 0, 1);

        // Initialize IMGUI
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& imGuiIo = ImGui::GetIO(); 
	    (void)imGuiIo;

        const char* GLSL_VERSION = "#version 130";

        ImGui_ImplGlfw_InitForOpenGL(Impl_->Handle_, true);
        ImGui_ImplOpenGL3_Init(GLSL_VERSION);
        ImGui::StyleColorsClassic();
	}

}