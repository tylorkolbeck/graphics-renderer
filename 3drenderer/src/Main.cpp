#include <stdio.h>
#include <vector>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <iostream>
#include <algorithm>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "mesh_loader.h"
#include "color.h"

#include <filesystem>

bool is_running = false;

int previous_frame_time = 0;

std::vector<triangle_t> triangles_to_render = {};

mesh_t mesh = {};

void load_meshes(mesh_t &mesh)
{
	std::string file_path = "assets/f22.obj";
	parse_obj_file(file_path, mesh);
}

void setup(void)
{
	render_method = RENDER_WIRE;
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

	load_meshes(mesh);
	// load_cube_mesh(mesh);

	// Flip model
	mesh.rotation.x = 3.14 / 2;
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
		if (event.key.keysym.sym == SDLK_w)
			camera.z += 0.1;
		if (event.key.keysym.sym == SDLK_s)
			camera.z -= 0.1;
		if (event.key.keysym.sym == SDLK_g)
			show_grid = !show_grid;
		break;
	}
}

bool compareAvg(const triangle_t& a, const triangle_t& b)
{
	return a.avg_depth > b.avg_depth;
}

void update()
{
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
		SDL_Delay(time_to_wait);

	// Increment the rotation of the cube
	mesh.rotation.y += 0.01;
	mesh.rotation.x += 0.01;
	mesh.rotation.z += 0.01;

	int num_faces = mesh.faces.size();

	// Reset faces array
	triangles_to_render.resize(num_faces);
	triangles_to_render.clear();

	// Loop all the triangle faces of the mesh
	for (int i = 0; i < num_faces; i++)
	{
		face mesh_face = mesh.faces[i];
		vec3_t face_vertices[3];

		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		vec3_t transformed_vertices[3];
		// Loop all three vertices of this current face and apply transformations
		for (int j = 0; j < 3; j++)
		{
			// Apply transformation to put in correct position in the world
			vec3_t transformed_vertex = face_vertices[j];
			transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
			transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
			transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

			// Translate the camera away from the camera
			transformed_vertex.z += 5;

			// Save transformed vertext in the array of transformed vertices
			transformed_vertices[j] = transformed_vertex;
		}

		// Check backface culling
		vec3_t vector_a = transformed_vertices[0]; /* 	A	*/
		vec3_t vector_b = transformed_vertices[1]; /*  / \  */
		vec3_t vector_c = transformed_vertices[2]; /* C___B */

		// // If dot product is less than 0 then do not render
		if (cull_method == CULL_BACKFACE && is_culled(vector_a, vector_b, vector_c))
		{
			continue;
		}

		// Loop all three transformed vertices to perform projection
		vec2_t projected_points[3];
		for (int j = 0; j < 3; j++)
		{
			// Apply projection
			projected_points[j] = project(transformed_vertices[j]);

			// Scale and translate the projected point to the middle of the screen
			projected_points[j].x += window_width / 2;
			projected_points[j].y += window_height / 2;
		}

		float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3;

		triangle_t projected_triangle = {
			{
				projected_points[0],
				projected_points[1],
				projected_points[2],
			},
			mesh_face.color,
			avg_depth
		};

		triangles_to_render.push_back(projected_triangle);
	}
	// Sort the triangles ascending by their avg_depth for painters algorithm
	std::sort(triangles_to_render.begin(), triangles_to_render.end(), compareAvg);
}

void render(void)
{
	if (show_grid)
		draw_grid(color_buffer, 10, C_GREY);

	// Draw the model
	for (int i = 0; i < triangles_to_render.size(); i++)
	{
		triangle_t tri = triangles_to_render[i];

		// Draw filled triangles
		if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE)
		{
			draw_filled_triangle(
				color_buffer,
				tri.points[0].x, tri.points[0].y, // vertex A
				tri.points[1].x, tri.points[1].y, // vertex B
				tri.points[2].x, tri.points[2].y, // vertex C
				tri.color);
		}

		// Draw triangle wireframe
		if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE)
		{
			draw_triangle(
				color_buffer,
				tri,
				C_WHITE);
		}

		if (render_method == RENDER_WIRE_VERTEX)
		{
			draw_rect(color_buffer, tri.points[0].x - 3, tri.points[0].y - 3, 6, 6, C_RED);
			draw_rect(color_buffer, tri.points[1].x - 3, tri.points[1].y - 3, 6, 6, C_RED);
			draw_rect(color_buffer, tri.points[2].x - 3, tri.points[2].y - 3, 6, 6, C_RED);
		}
	}

	render_color_buffer();			// Render the color buffer to the texture
	clear_color_buffer(0xFF000000); // Set each pixel to yellow color in buffer

	SDL_RenderPresent(renderer);
}

void free_resources(void)
{
	// free(color_buffer);
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