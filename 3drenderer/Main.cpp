#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>

#include "display.h"
#include "vector.h"

bool is_running = false;

uint32_t C_TEAL = 0x82f2fa;
uint32_t C_BLACK = 0xFF000000;
uint32_t C_GREY = 0xFF333333;
uint32_t C_WHITE = 0xFFFFFFFF;
uint32_t C_RED = 0xFFFF0000;
uint32_t C_PINK = 0xfc60fc;

const int N_POINTS = 9 * 9 * 9;
vec3 cube_points[N_POINTS];
vec2 projected_points[N_POINTS];


int objX = 200;
int objY = 200;

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

	// Load cube points
	// From -1 to 1 (length 2)

	int point_count = 0;
	for (float x = -1; x <= 1; x += 0.25) {
		for (float y = -1; y <= 1; y += 0.25) {
			for (float z = -1; z <= 1; z += 0.25) {
				vec3 new_point = { x, y, z };
				cube_points[point_count++] = new_point;
			}
		}
	}
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
	cube_rotation.y += 0.01;
	cube_rotation.x += 0.01;
	cube_rotation.z += 0.01;

	for (int i = 0; i < N_POINTS; i++) {
		vec3 point = cube_points[i];

		// Apply rotations
		vec3 transformedP = vec3_rotate_y(point, cube_rotation.y);
		transformedP = vec3_rotate_x(transformedP, cube_rotation.x);
		transformedP = vec3_rotate_z(transformedP, cube_rotation.z);

		// Translate the points away from the camera
		transformedP.z -= camera.z;

		vec2 projected = project_point(transformedP);
		projected_points[i] = projected;
	}
}

void render(void) {
	draw_grid(color_buffer, 10, C_GREY);
	//draw_rect(color_buffer, objX, objY, 300, 200, C_TEAL);
	for (int i = 0; i < N_POINTS - 1; i++) {
		draw_rect(color_buffer, projected_points[i].x + (window_width / 2), projected_points[i].y + (window_height / 2), 4, 4, C_TEAL);
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