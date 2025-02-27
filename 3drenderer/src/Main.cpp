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
#include "matrix.h"
#include <filesystem>
#include "light.h"

bool is_running = false;
int previous_frame_time = 0;
mat4_t proj_matrix;

std::vector<triangle_t> triangles_to_render = {};

vec3_t light_direction{0, 0, 0};

mesh_t mesh = {};

void load_meshes(mesh_t &mesh)
{
	std::string file_path = "assets/f22.obj";
	parse_obj_file(file_path, mesh);
}

void setup(void)
{
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

	load_meshes(mesh);
	// load_cube_mesh(mesh);
	// load_point_mesh(mesh);

	// Flip model
	// mesh.rotation.x = 3.14 + .5;
	// mesh.rotation.z = 3.14 / 4;
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
		// if (event.key.keysym.sym == SDLK_w)
		// 	camera.z += 0.1;
		// if (event.key.keysym.sym == SDLK_s)
		// 	camera.z -= 0.1;
		if (event.key.keysym.sym == SDLK_g)
			show_grid = !show_grid;
		break;
	}
}

bool compareAvg(const triangle_t &a, const triangle_t &b)
{
	return a.avg_depth > b.avg_depth;
}

void update()
{
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
		// SDL_Delay(time_to_wait);
	previous_frame_time - SDL_GetTicks();

	// Increment the rotation of the cube0
	// mesh.rotation.y += 0.00;
	mesh.rotation.x += 0.005;
	// mesh.rotation.z += 0.00;

	// mesh.scale.z += 0.000;
	// mesh.translation.x += 0.002;
	mesh.translation.z = 5.0;

	mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
	mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
	mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);
	mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);

	int num_faces = mesh.faces.size();

	// Reset faces array
	triangles_to_render.resize(num_faces);
	triangles_to_render.clear();

	// Loop all the triangle faces of the mesh
	for (int i = 0; i < num_faces; i++)
	{
		face_t mesh_face = mesh.faces[i];
		vec3_t face_vertices[3];

		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		vec4_t transformed_vertices[3];
		// Loop all three vertices of this current face and apply transformations
		for (int j = 0; j < 3; j++)
		{
			// Apply transformation to put in correct position in the world
			vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

			// World matrix
			mat4_t world_matrix = mat4_identity();
			world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
			world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

			// Multiple the world matrix by original vector
			transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

			// Save transformed vertex in the array of transformed vertices
			transformed_vertices[j] = transformed_vertex;
		}

		if (cull_method == CULL_BACKFACE)
		{
			// Check backface culling
			vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]); /* 	A	*/
			vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]); /*  / \  */
			vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]); /* C___B */
			// 1. Find vectors B-A and C-A
			vec3_t vector_ab = vec3_sub(vector_b, vector_a);
			vec3_t vector_ac = vec3_sub(vector_c, vector_a);
			vec3_normalize(&vector_ab);
			vec3_normalize(&vector_ac);

			// 2. Compute face normal using cross product of ab and ac - Order matters based on winding direction
			vec3_t normal = vec3_cross(vector_ab, vector_ac);
			vec3_normalize(&normal);

			// 3. Find the camera ray vector by subtracting the camera position from point A
			vec3_t camera_ray = vec3_sub({camera.x, camera.y, camera.z}, vector_a);
			vec3_normalize(&camera_ray);
			// 4. Take the dot product between the normal N and the camera ray
			float dot_normal_camera = vec3_dot(normal, camera_ray);
			// 5. If this dot product is less than zero, then do not display the face
			if (dot_normal_camera < 0)
				continue;


			// Loop all three transformed vertices to perform projection
			vec4_t projected_points[3];
			for (int j = 0; j < 3; j++)
			{
				// Apply projection
				projected_points[j] = mat4_mul_vec4_project(proj_matrix, transformed_vertices[j]);

				projected_points[j].x *= (window_width / 2.0);
				projected_points[j].y *= (window_height / 2.0);
				projected_points[j].x *= -1;

				projected_points[j].x += (window_width / 2.0);
				projected_points[j].y += (window_height / 2.0);
			}

			float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3;

			// Calculate the shade intensity based on how aligned the face normal and light are
			float light_intensity_factor = -vec3_dot(normal, light.direction);
			// triangle_color = light_apply_intensity(triangle_color, light_intensity_factor);
			uint32_t triangle_color = light_apply_intensity(mesh_face.color, light_intensity_factor);

			// float new_color = light_apply_intensity(C_BLUE, dot_percentage);

			triangle_t projected_triangle = {
				.points = {
					{projected_points[0].x, projected_points[0].y},
					{projected_points[1].x, projected_points[1].y},
					{projected_points[2].x, projected_points[2].y},
				},
				.color = triangle_color,
				.avg_depth = avg_depth};

			triangles_to_render.push_back(projected_triangle);
		}
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
		auto width = window_width;
		triangle_t triangles = triangles_to_render[i];

		// Draw filled triangles
		if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE)
		{
			draw_filled_triangle(
				color_buffer,
				triangles.points[0].x, triangles.points[0].y, // vertex A
				triangles.points[1].x, triangles.points[1].y, // vertex B
				triangles.points[2].x, triangles.points[2].y, // vertex C
				triangles.color);
		}

		// Draw triangle wireframe
		if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE)
		{
			draw_triangle(
				color_buffer,
				triangles,
				C_WHITE);
		}

		if (render_method == RENDER_WIRE_VERTEX)
		{
			draw_rect(color_buffer, triangles.points[0].x - 3, triangles.points[0].y - 3, 6, 6, C_RED);
			draw_rect(color_buffer, triangles.points[1].x - 3, triangles.points[1].y - 3, 6, 6, C_RED);
			draw_rect(color_buffer, triangles.points[2].x - 3, triangles.points[2].y - 3, 6, 6, C_RED);
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