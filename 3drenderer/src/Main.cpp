#include <stdio.h>
#include <vector>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <iostream>
#include "display.h"
#include "vector.h"
#include "mesh_old.h"
#include "mesh_loader.h"
#include "color.h"
#include "matrix.h"
#include <filesystem>
#include "light.h"
#include "Mesh.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

bool is_running = false;
int previous_frame_time = 0;
mat4_t proj_matrix;

std::vector<triangle_t> triangles_to_render = {};

vec3_t light_direction{0, 0, 0};

Mesh f22Mesh{};

void load_meshes(mesh_t &mesh)
{
	std::string file_path = "assets/f22.obj";
	parse_obj_file(file_path, mesh);
}

void setup(void)
{
	SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
	SDL_SetHint(SDL_HINT_MOUSE_AUTO_CAPTURE, "0");

	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);
	// Set up ImGui with SDL2 and SDL_Renderer
	SDL_SetRelativeMouseMode(SDL_FALSE); // Disable SDL from hijacking mouse input
	SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
	SDL_SetHint(SDL_HINT_MOUSE_AUTO_CAPTURE, "0");
	SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0"); // Prevent minimizing when clicking outside
	SDL_SetWindowGrab(window, SDL_FALSE);					 // Ensure SDL does not "trap" mouse inside window

	// Set default font to avoid rendering issues
	io.Fonts->AddFontDefault();

	render_method = RENDER_FILL_TRIANGLE;
	cull_method = CULL_BACKFACE;

	// Allocate the color buffer in memory
	create_color_buffer_32(&color_buffer, window_width * window_height);
	if (!color_buffer)
	{
		fprintf(stderr, "Error allocating color buffer \n");
	}

	// Creating SDL texture used to display color buffer
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height);

	float fov = M_PI / 3; // 60deg
	float aspect = (float)window_height / (float)window_width;
	float znear = 0.1;
	float zfar = 100.0;
	proj_matrix = mat4_make_perspective(fov, aspect, znear, zfar);

	std::string file_path = "assets/f22.obj";
	f22Mesh = Mesh(file_path);
}

void process_input(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event); // Send event to ImGui

		// Check if ImGui wants to capture the event (prevents SDL from handling ImGui clicks)
		ImGuiIO &io = ImGui::GetIO();
		if (io.WantCaptureMouse || io.WantCaptureKeyboard)
			continue;

		switch (event.type)
		{
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				is_running = false;
			if (event.key.keysym.sym == SDLK_1)
				render_method = RENDER_WIRE_VERTEX;
			if (event.key.keysym.sym == SDLK_2)
				render_method = RENDER_WIRE;
			if (event.key.keysym.sym == SDLK_3)
				render_method = RENDER_FILL_TRIANGLE;
			if (event.key.keysym.sym == SDLK_4)
				render_method = RENDER_FILL_TRIANGLE_WIRE;
			if (event.key.keysym.sym == SDLK_c)
				cull_method = CULL_BACKFACE;
			if (event.key.keysym.sym == SDLK_d)
				cull_method = CULL_NONE;
			// if (event.key.keysym.sym == SDLK_w)
			// 	camera.z += 0.1;
			// if (event.key.keysym.sym == SDLK_s)
			// 	camera.z -= 0.1;
			if (event.key.keysym.sym == SDLK_g)
				show_grid = !show_grid;
			break;
		}
	}
}

void update()
{
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
		SDL_Delay(time_to_wait);
	previous_frame_time - SDL_GetTicks();

	// Update the mesh
	f22Mesh.rotate({.x = f22Mesh.rotation().x + 0.005f, .y = 0.0, .z = 0.0});
	f22Mesh.translate({.x = 0.0, .y = 0.0f, .z = 5.0f});
	f22Mesh.update(camera, proj_matrix, light, true, {width : window_width, height : window_height});
}

void render(void)
{
	// Start a new ImGui frame
	ImGui_ImplSDL2_NewFrame();
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui::NewFrame();

	// Show a simple test window
	ImGui::Begin("Hello ImGui");
	ImGui::Text("This is a test window inside SDL2!");
	ImGui::End();

	if (show_grid)
		draw_grid(color_buffer, 10, Color::C_LIGHTGREY);

	// Draw the model
	f22Mesh.render(color_buffer);

	render_color_buffer();			// Render the color buffer to the texture
	clear_color_buffer(0xFF000000); // Set each pixel to yellow color in buffer

	// Render everything
	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

	SDL_RenderPresent(renderer);
}

int main(int argc, char *args[])
{
	is_running = initialize_window();

	setup();

	while (is_running)
	{
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}