#pragma once

#include <nuklear.h>

#include <glew.h>
#include <glfw3.h>
#include <common.h>
#include <core/ecs/ecs_manager.h>

#ifndef NK_GLFW_TEXT_MAX
#define NK_GLFW_TEXT_MAX 256
#endif

#ifndef NK_GLFW_DOUBLE_CLICK_LO
#define NK_GLFW_DOUBLE_CLICK_LO 0.02
#endif

#ifndef NK_GLFW_DOUBLE_CLICK_HI
#define NK_GLFW_DOBULE_CLICK_HI 0.2
#endif

#define MAX_VERTEX_SIZE 512 * 1024
#define MAX_ELEMENT_SIZE 128 * 1024

#ifdef __APPLE__
#define NK_SHADER_VERSION "#version 150\n";
#else
#define NK_SHADER_VERSION "#version 300 es\n";
#endif

namespace luminos { namespace ui
{

	/*
		Creates a new nk_context object

		Win - Window to create context for
		Returns new nk_context pointer
	*/
	LUMINOS_API struct nk_context * nk_glfw3_init(std::shared_ptr<ECSManager> Manager, GLFWwindow * win);

	/*
		Destroys the nk_context and releases context
	*/
	LUMINOS_API void				nk_glfw3_shutdown(void);

	/*
		Stashes a font for rendering with
		
		atlas - Pointer to Font atlas
	*/
	LUMINOS_API void				nk_glfw3_font_stash_begin(struct nk_font_atlas **atlas);

	/*
		Finished the stashing of a font
	*/
	LUMINOS_API void				nk_glfw3_font_stash_end(void);

	/*
		Prepares nuklear for a new frame
	*/
	LUMINOS_API void				nk_glfw3_new_frame(void);

	/*
		Renders the nuklear context

		max_vertex_buffer - Maximum size of vertex buffer
		max_element_buffer - Maximum size of element buffer
	*/
	LUMINOS_API void				nk_glfw3_render(int max_vertex_buffer, int max_element_buffer);

	/*
		Creates a new device
	*/
	LUMINOS_API void				nk_glfw3_device_create(std::shared_ptr<ECSManager> Manager);

	/*
		Destroys the device
	*/
	LUMINOS_API void				nk_glfw3_device_destroy(void);

	/*
		Gets the nk_window associated with the given name

		ctx - Context to search for window in
		name - Name of window
		Returns nk_window pointer
	*/
	LUMINOS_API struct nk_window *  nk_glfw3_get_window(nk_context * ctx, const char * name);

	/*
		Gets the active nk_context of the application

		Returns active context
	*/
	LUMINOS_API struct nk_context * nk_glfw3_get_active_context();

	/*
		Sets the provided character

		code - Character ID
	*/
	LUMINOS_API void				nk_glfw3_set_char(int code);

} }
