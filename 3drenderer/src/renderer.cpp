// #include <cmath>
// #include "renderer.h"
// #include "triangles.h"
// #include "color.h"
// #include <imgui_impl_sdlrenderer2.h>
// #include <imgui_impl_sdl2.h>
// #include "Window.h"

// extern SDL_Window *m_window;
// extern SDL_Renderer *renderer = nullptr;

// extern uint32_t *color_buffer = nullptr;
// extern SDL_Texture *color_buffer_texture = nullptr;

// extern int window_width = 800;
// extern int window_height = 600;

// bool show_grid = false;
// cull_method_t cull_method = CULL_BACKFACE;
// render_method_t render_method = RENDER_WIRE;
// vec3_t camera = {0, 0, 0};

// bool initialize_renderer(Window *window)
// {
// 	window = window;
// 	window_width = window->getWidth();
// 	window_height = window->getHeight();

// 	renderer = SDL_CreateRenderer(window->getSDLWindow(), -1, 0);
// 	if (!renderer)
// 	{
// 		fprintf(stderr, "Error creating renderer. \n");
// 		return false;
// 	}

// 	// SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

// 	return true;
// }

// void destroy_renderer(void)
// {
// 	// Cleanup ImGui
// 	ImGui_ImplSDLRenderer2_Shutdown();
// 	ImGui_ImplSDL2_Shutdown();
// 	ImGui::DestroyContext();

// 	free(color_buffer);
// 	SDL_DestroyRenderer(renderer);
// 	// SDL_DestroyWindow(window);
// 	// SDL_Quit();

// 	// ImGui_ImplSDLRenderer2_Shutdown();
// 	// ImGui_ImplSDL2_Shutdown();
// 	// ImGui::DestroyContext();
// }

// void draw_grid(uint32_t *buffer_p, int cellSize, uint32_t color)
// {
// 	// Draw vertical lines
// 	for (int x = 0; x < window_width; x += cellSize)
// 	{
// 		for (int y = 0; y < window_height; y++)
// 		{
// 			buffer_p[loc_1d(x, y)] = color;
// 		}
// 	}

// 	// Draw horizontal lines
// 	for (int y = 0; y < window_height; y += cellSize)
// 	{
// 		for (int x = 0; x < window_width; x++)
// 		{
// 			buffer_p[loc_1d(x, y)] = color;
// 		}
// 	}
// }

// void draw_dot(uint32_t *buffer_p, int cellSize, uint32_t color)
// {
// 	for (int x = 0; x < window_width; x += cellSize)
// 	{
// 		for (int y = 0; y < window_height; y += cellSize)
// 		{
// 			buffer_p[(window_width * y) + x] = color;
// 		}
// 	}
// }

// void draw_rect(uint32_t *buffer_p, int xloc, int yloc, int width, int height, uint32_t color)
// {
// 	for (int x = xloc; x <= xloc + width - 1; x++)
// 	{
// 		for (int y = yloc; y <= yloc + height - 1; y++)
// 		{
// 			draw_pixel(buffer_p, x, y, color);
// 		}
// 	}
// }

// void draw_rect(uint32_t *buffer_p, int xloc, int yloc, int width, int height)
// {
// 	for (int x = xloc; x <= xloc + width - 1; x++)
// 	{
// 		for (int y = yloc; y <= yloc + height - 1; y++)
// 		{
// 			draw_pixel(buffer_p, x, y, Color::ORANGE);
// 		}
// 	}
// }

// void draw_pixel(uint32_t *buffer_p, int x, int y, uint32_t color)
// {
// 	if (x >= 0 && x < window_width && y >= 0 && y < window_height)
// 	{
// 		buffer_p[loc_1d(x, y)] = color;
// 	}
// }

// void create_color_buffer_32(uint32_t **bufferP, int size)
// {
// 	*bufferP = (uint32_t *)malloc(sizeof(uint32_t) * size);
// }

// void clear_color_buffer(uint32_t *buffer_p, uint32_t color)
// {
// 	// TODO: pass size of color buffer and buffer and set with out nested loop?
// 	for (int y = 0; y < window_height; y++)
// 	{
// 		for (int x = 0; x < window_width; x++)
// 		{
// 			buffer_p[(window_width * y) + x] = color;
// 		}
// 	}
// }

// void setBufferPixel(uint32_t *buffer_p, int row, int col, uint32_t color)
// {
// 	buffer_p[(window_width * row) + col] = color;
// }

// // Render on the back buffer, render buffer to texture
// void render_color_buffer(uint32_t *buffer_p)
// {
// 	SDL_UpdateTexture(
// 		color_buffer_texture,
// 		NULL,
// 		buffer_p,
// 		(int)window_width * sizeof(uint32_t));
// 	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
// }

// // Formula to get index in 1d array based on x and y
// int loc_1d(int x, int y)
// {
// 	return (window_width * y) + x;
// }

// void draw_line(uint32_t *buffer_p, int x1, int y1, int x2, int y2, uint32_t color)
// {
// 	int dx = x2 - x1;
// 	int dy = y2 - y1;

// 	int side_length = abs(dx) >= abs(dy) ? abs(dx) : abs(dy);

// 	float x_inc = dx / (float)side_length;
// 	float y_inc = dy / (float)side_length;

// 	float current_x = x1;
// 	float current_y = y1;

// 	for (int i = 0; i <= side_length; i++)
// 	{
// 		draw_pixel(buffer_p, round(current_x), round(current_y), color);
// 		current_x += x_inc;
// 		current_y += y_inc;
// 	}
// }

// void draw_line(uint32_t *buffer_p, vec2_t a, vec2_t b, uint32_t color)
// {
// 	int dx = b.x - a.x;
// 	int dy = b.y - a.y;

// 	int side_length = abs(dx) >= abs(dy) ? abs(dx) : abs(dy);

// 	float x_inc = dx / (float)side_length;
// 	float y_inc = dy / (float)side_length;

// 	float current_x = a.x;
// 	float current_y = a.y;

// 	for (int i = 0; i <= side_length; i++)
// 	{
// 		draw_pixel(buffer_p, round(current_x), round(current_y), color);
// 		current_x += x_inc;
// 		current_y += y_inc;
// 	}
// }

// void draw_triangle(uint32_t *buffer_p, triangle_t tri, uint32_t color)
// {
// 	vec2_t p1 = tri.points[0];
// 	vec2_t p2 = tri.points[1];
// 	vec2_t p3 = tri.points[2];

// 	draw_line(buffer_p, p1, p2, color);
// 	draw_line(buffer_p, p2, p3, color);
// 	draw_line(buffer_p, p3, p1, color);
// }

// bool is_culled(vec4_t vertices[])
// {
// 	vec3_t vector_a = vec3_from_vec4(vertices[0]); /* 	A	*/
// 	vec3_t vector_b = vec3_from_vec4(vertices[1]); /*  / \  */
// 	vec3_t vector_c = vec3_from_vec4(vertices[2]); /* C___B */
// 	// 1. Find vectors B-A and C-A
// 	vec3_t vector_ab = vec3_sub(vector_b, vector_a);
// 	vec3_t vector_ac = vec3_sub(vector_c, vector_a);
// 	vec3_normalize(&vector_ab);
// 	vec3_normalize(&vector_ac);

// 	// 2. Compute face normal using cross product of ab and ac - Order matters based on winding direction
// 	vec3_t normal = vec3_cross(vector_ab, vector_ac);
// 	vec3_normalize(&normal);

// 	// 3. Find the camera ray vector by subtracting the camera position from point A
// 	vec3_t camera_ray = vec3_sub({camera.x, camera.y, camera.z}, vector_a);
// 	// 4. Take the dot product between the normal N and the camera ray
// 	vec3_normalize(&camera_ray);
// 	float dot_prod = vec3_dot(normal, camera_ray);
// 	// 5. If this dot product is less than zero, then do not display the face

// 	return dot_prod < 0;
// }

// bool is_culled(vec3_t vector_a, vec3_t vector_b, vec3_t vector_c)
// {
// 	// 1. Find vectors B-A and C-A
// 	vec3_t vector_ab = vec3_sub(vector_b, vector_a);
// 	vec3_t vector_ac = vec3_sub(vector_c, vector_a);
// 	vec3_normalize(&vector_ab);
// 	vec3_normalize(&vector_ac);

// 	// 2. Compute face normal using cross product of ab and ac - Order matters based on winding direction
// 	vec3_t normal = vec3_cross(vector_ab, vector_ac);
// 	vec3_normalize(&normal);

// 	// 3. Find the camera ray vector by subtracting the camera position from point A
// 	vec3_t camera_ray = vec3_sub({camera.x, camera.y, camera.z}, vector_a);
// 	vec3_normalize(&camera_ray);
// 	// 4. Take the dot product between the normal N and the camera ray
// 	float dot_prod = vec3_dot(normal, camera_ray);
// 	// 5. If this dot product is less than zero, then do not display the face

// 	return dot_prod < 0;
// }