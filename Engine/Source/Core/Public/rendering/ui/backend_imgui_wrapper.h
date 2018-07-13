#pragma once
#include <imgui/imgui.h>

struct GLFWwindow;

bool IMGUI_IMPL_API ImGui_ImplGlfw_InitForOpenGL(GLFWwindow* window, bool install_callbacks);
bool IMGUI_IMPL_API ImGui_ImplGlfw_InitForVulkan(GLFWwindow* window, bool install_callbacks);
void IMGUI_IMPL_API ImGui_ImplGlfw_Shutdown();
void IMGUI_IMPL_API ImGui_ImplGlfw_NewFrame();

// GLFW callbacks (installed by default if you enable 'install_callbacks' during initialization)
// Provided here if you want to chain callbacks.
// You can also handle inputs yourself and use those as a reference.
void IMGUI_IMPL_API ImGui_ImplGlfw_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void IMGUI_IMPL_API ImGui_ImplGlfw_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void IMGUI_IMPL_API ImGui_ImplGlfw_CharCallback(GLFWwindow* window, unsigned int c);

bool IMGUI_IMPL_API ImGui_ImplOpenGL3_Init(const char* glsl_version = NULL);
void IMGUI_IMPL_API ImGui_ImplOpenGL3_Shutdown();
void IMGUI_IMPL_API ImGui_ImplOpenGL3_NewFrame();
void IMGUI_IMPL_API ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data);

// Called by Init/NewFrame/Shutdown
bool IMGUI_IMPL_API ImGui_ImplOpenGL3_CreateFontsTexture();
void IMGUI_IMPL_API ImGui_ImplOpenGL3_DestroyFontsTexture();
bool IMGUI_IMPL_API ImGui_ImplOpenGL3_CreateDeviceObjects();
void IMGUI_IMPL_API ImGui_ImplOpenGL3_DestroyDeviceObjects();