#include <gfx/ui.h>
#include "core/window.h"
#include "gfx/context.h"
#include <nuklear.h>
#include "core/beans/property.h"

#include <glm/vec3.hpp>

using namespace luminos::gfx;

void main(void)
{
	/* Platform */
	auto window = luminos::Window::CreateWindow("Luminos Engine Editor", 1920, 1080, false, false);

	nk_context *ctx = nk_glfw3_init(window->GetHandle());
	struct nk_font_atlas *atlas;
	nk_glfw3_font_stash_begin(&atlas);
	nk_glfw3_font_stash_end();

	struct nk_rect size;
	struct nk_command_buffer * canvas = nullptr;

	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 sca;

	char buffer[16];
	memset(buffer, 0, 16);
	int len = 0;

	while (window->Update())
	{
		Context::Clear();
		nk_glfw3_new_frame();
		if (nk_begin(ctx, "Editor", nk_rect(0, 0, window->GetWidth().GetValue(), window->GetHeight().GetValue()), NK_WINDOW_BORDER | NK_WINDOW_TITLE))
		{
			size = nk_layout_space_bounds(ctx);
			if (!canvas) canvas = nk_window_get_canvas(ctx);
				/* display grid */
			nk_layout_space_begin(ctx, NK_STATIC, window->GetWidth().GetValue(), 1);
			{
				float x, y;
				const float grid_size = 32.0f;
				const struct nk_color grid_color = nk_rgb(50, 50, 50);
				for (x = (float)fmod(size.x, grid_size); x < size.w; x += grid_size)
					nk_stroke_line(canvas, x + size.x, size.y, x + size.x, size.y + size.h, 1.0f, grid_color);
				for (y = (float)fmod(size.y, grid_size); y < size.h; y += grid_size)
					nk_stroke_line(canvas, size.x, y + size.y, size.x + size.w, y + size.y, 1.0f, grid_color);

				nk_layout_space_push(ctx, nk_rect(0, 0, 400, 150));
				if (nk_group_begin(ctx, "Transformation", NK_WINDOW_MOVABLE | NK_WINDOW_BORDER | NK_WINDOW_TITLE))
				{
					len = strlen(buffer);
					nk_layout_row_begin(ctx, NK_STATIC, 30, 7);
					nk_layout_row_push(ctx, 58);
					nk_label(ctx, "Position:", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
					nk_layout_row_push(ctx, 25);
					nk_label(ctx, "X", NK_TEXT_ALIGN_RIGHT | NK_TEXT_ALIGN_MIDDLE);
					nk_layout_row_push(ctx, 73);
					nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 15, nk_filter_float);
					nk_layout_row_push(ctx, 25);
					nk_label(ctx, "Y", NK_TEXT_ALIGN_RIGHT | NK_TEXT_ALIGN_MIDDLE);
					nk_layout_row_push(ctx, 73);
					nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 15, nk_filter_float);
					nk_layout_row_push(ctx, 25);
					nk_label(ctx, "Z", NK_TEXT_ALIGN_RIGHT | NK_TEXT_ALIGN_MIDDLE);
					nk_layout_row_push(ctx, 73);
					nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 15, nk_filter_float);

//					nk_label(ctx, "Rotation:", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
//					nk_propertyf(ctx, "X:", -std::numeric_limits<float>::max(), 0, std::numeric_limits<float>::max(), .1, .1);
//					nk_propertyf(ctx, "Y:", -std::numeric_limits<float>::max(), 0, std::numeric_limits<float>::max(), .1, .1);
//					nk_propertyf(ctx, "Z:", -std::numeric_limits<float>::max(), 0, std::numeric_limits<float>::max(), .1, .1);
//
//					nk_label(ctx, "Scale:", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
//					nk_propertyf(ctx, "X:", -std::numeric_limits<float>::max(), 0, std::numeric_limits<float>::max(), .1, .1);
//					nk_propertyf(ctx, "Y:", -std::numeric_limits<float>::max(), 0, std::numeric_limits<float>::max(), .1, .1);
//					nk_propertyf(ctx, "Z:", -std::numeric_limits<float>::max(), 0, std::numeric_limits<float>::max(), .1, .1);
					nk_group_end(ctx);
				}
				nk_layout_space_end(ctx);
			}
			nk_end(ctx);
			nk_glfw3_render(MAX_VERTEX_SIZE, MAX_ELEMENT_SIZE);
		}
	}

	window->CloseWindow();
}