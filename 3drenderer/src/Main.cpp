#include <stdio.h>
#include <vector>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"

bool is_running = false;

uint32_t C_TEAL = 0x82f2fa;
uint32_t C_BLACK = 0xFF000000;
uint32_t C_GREY = 0x2B2B2B;
uint32_t C_WHITE = 0xFFFFFFFF;
uint32_t C_RED = 0xFFFF0000;
uint32_t C_PINK = 0xfc60fc;

int previous_frame_time = 0;

int objX = 200;
int objY = 200;

std::vector<triangle> triangles_to_render = {};

void setup(void)
{
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

	load_cube_mesh();
}

void process_input(void)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
		{
			is_running = false;
		}
		if (event.key.keysym.sym == SDLK_RIGHT)
		{
			objX += 2;
		}
		if (event.key.keysym.sym == SDLK_LEFT)
		{
			objX -= 2;
		}
		if (event.key.keysym.sym == SDLK_DOWN)
		{
			objY += 2;
		}
		if (event.key.keysym.sym == SDLK_UP)
		{
			objY -= 2;
		}
		if (event.key.keysym.sym == SDLK_p)
		{
			is_perspective = !is_perspective;
		}
		if (event.key.keysym.sym == SDLK_w)
		{
			camera.z += 0.1;
		}
		if (event.key.keysym.sym == SDLK_s)
		{
			camera.z -= 0.1;
		}
		if (event.key.keysym.sym == SDLK_g) {
			show_grid = !show_grid;
		}
		break;
	}
}

void update()
{
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
	{
		SDL_Delay(time_to_wait);
	}

	// Increment the rotation of the cube
	mesh.rotation.y += 0.01;
	mesh.rotation.x += 0.01;
	mesh.rotation.z += 0.01;

	int num_faces = mesh.faces.size();

	// Reset faces array
	triangles_to_render.resize(num_faces);
	triangles_to_render.clear();

	for (int i = 0; i < num_faces; i++)
	{
		face mesh_face = mesh.faces[i];
		vec3 face_vertices[3];

		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		triangle projected_triangle;
		for (int j = 0; j < 3; j++)
		{
			// Apply transformation
			vec3 transformed_vertex = face_vertices[j];
			transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
			transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
			transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

			transformed_vertex.z -= camera.z;

			// Apply projection
			vec2 projected_point = project(transformed_vertex);

			// Scale and translate the projected point
			projected_point.x += window_width / 2;
			projected_point.y += window_height / 2;

			projected_triangle.points[j] = projected_point;
		}

		triangles_to_render.push_back(projected_triangle);
	}
}

void render(void)
{
	// Draw grid
	if (show_grid)
		draw_grid(color_buffer, 10, C_GREY);

	for (int i = 0; i < mesh_faces.size(); i++)
	{
		triangle tri = triangles_to_render[i];
		draw_triangle(color_buffer, tri, C_TEAL);

		vec2 point1 = tri.points[0];
		vec2 point2 = tri.points[1];
		vec2 point3 = tri.points[2];

		draw_rect(color_buffer, point1.x, point1.y, 4, 4, C_TEAL);
		draw_rect(color_buffer, point2.x, point2.y, 4, 4, C_TEAL);
		draw_rect(color_buffer, point3.x, point3.y, 4, 4, C_TEAL);
	}

	render_color_buffer();			// Render the color buffer to the texture
	clear_color_buffer(0xFF000000); // Set each pixel to yellow color in buffer

	SDL_RenderPresent(renderer);
}

void free_resources(void)
{
	free(color_buffer);
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
	free_resources();

	return 0;
}