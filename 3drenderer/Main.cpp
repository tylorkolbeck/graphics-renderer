#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"

bool is_running = false;

uint32_t C_TEAL = 0x82f2fa;
uint32_t C_BLACK = 0xFF000000;
uint32_t C_GREY = 0xFF333333;
uint32_t C_WHITE = 0xFFFFFFFF;
uint32_t C_RED = 0xFFFF0000;
uint32_t C_PINK = 0xfc60fc;

int previous_frame_time= 0;

int objX = 200;
int objY = 200;

triangle triangles_to_render[N_MESH_FACES];


void setup(void) {
	// Allocate the color buffer in memory
	create_color_buffer_32(&color_buffer, window_width * window_height);
	if (!color_buffer) {
		fprintf(stderr, "Error allocating color buffer \n");
	}

	// Creating SDL texture used to display color buffer
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);
}

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			is_running = false;
		}
		if (event.key.keysym.sym == SDLK_RIGHT) {
			objX += 2;
		}
		if (event.key.keysym.sym == SDLK_LEFT) {
			objX -= 2;
		}
		if (event.key.keysym.sym == SDLK_DOWN) {
			objY += 2;
		}
		if (event.key.keysym.sym == SDLK_UP) {
			objY -= 2;
		}
		break;
	}
}

void update() {
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), previous_frame_time + FRAME_TARGET_TIME));
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	cube_rotation.y += 0.00;
	cube_rotation.x += 0.00;
	cube_rotation.z += 0.00;

	for (int i = 0; i < N_MESH_FACES; i++) {
		face mesh_face = mesh_faces[i];

		vec3 face_vertices[3];

		face_vertices[0] = mesh_vertices[mesh_face.a - 1];
		face_vertices[1] = mesh_vertices[mesh_face.b - 1];
		face_vertices[2] = mesh_vertices[mesh_face.c - 1];

		triangle projected_triangle;

		for (int j = 0; j < 3; j++) {
			vec3 transformed_vertex = face_vertices[j];
			transformed_vertex = vec3_rotate_y(transformed_vertex, cube_rotation.y);
			transformed_vertex = vec3_rotate_x(transformed_vertex, cube_rotation.x);
			transformed_vertex = vec3_rotate_z(transformed_vertex, cube_rotation.z);

			transformed_vertex.z -= camera.z;

			vec2 projected_point = project_point(transformed_vertex);
			
			// Scale and translate the projected point
			projected_point.x += window_width / 2;
			projected_point.y += window_height / 2;

			projected_triangle.points[j] = projected_point;
		}

		triangles_to_render[i] = projected_triangle;

	}
}

void render(void) {
	// draw_grid(color_buffer, 10, C_GREY);
	//draw_rect(color_buffer, objX, objY, 300, 200, C_TEAL);
	/*
	for (int i = 0; i < N_POINTS - 1; i++) {
		draw_rect(color_buffer, projected_points[i].x + (window_width / 2), projected_points[i].y + (window_height / 2), 4, 4, C_TEAL);
	}
	*/

	for (int i = 0; i < N_MESH_FACES; i++) {
		triangle tri = triangles_to_render[i];

		draw_triangle(color_buffer, tri, C_TEAL);

		//vec2 point1 = tri.points[0];
		//vec2 point2 = tri.points[1];
		//vec2 point3 = tri.points[2];

		//draw_rect(color_buffer, point1.x, point1.y, 4, 4, C_TEAL);
		//draw_rect(color_buffer, point2.x, point2.y, 4, 4, C_TEAL);
		//draw_rect(color_buffer, point3.x, point3.y, 4, 4, C_TEAL);
	}

	render_color_buffer(); // Render the color buffer to the texture
	clear_color_buffer(0xFF000000); // Set each pixel to yellow color in buffer

	SDL_RenderPresent(renderer);
}

int main(int argc, char* args[]) {
	is_running = initialize_window();

	setup();

	while (is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}