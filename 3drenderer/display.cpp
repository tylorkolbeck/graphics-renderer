#include "display.h"

extern SDL_Window* window = NULL;
extern SDL_Renderer* renderer = NULL;
extern uint32_t* color_buffer = NULL;
extern SDL_Texture* color_buffer_texture = NULL;
extern int window_width = 800;
extern int window_height = 600;
float fov_factor = 640;

vec3 camera = { 0, 0, -5 };
vec3 cube_rotation = { 0, 0, 0 };

bool initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL. \n");
		return false;
	}

	// Query SDL to get fullscreen max width and height
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	window_width = display_mode.w;
	window_height = display_mode.h;


	// Create SDL window
	window = SDL_CreateWindow(NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_BORDERLESS);

	if (!window) {
		fprintf(stderr, "Error creating SDL window. \n");
		return false;
	}

	// TODO: Create SDL renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Error creating renderer. \n");
		return false;
	}

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void destroy_window(void) {
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void draw_grid(uint32_t* buffer_p, int cellSize, uint32_t color) {
	// Draw vertical lines
	for (int x = 0; x < window_width; x += cellSize) {
		for (int y = 0; y < window_height; y++) {
			buffer_p[loc_1d(x, y)] = color;
		}
	}

	// Draw horizontal lines
	for (int y = 0; y < window_height; y += cellSize) {
		for (int x = 0; x < window_width; x++) {
			buffer_p[loc_1d(x, y)] = color;
		}
	}
}

void draw_dot(uint32_t* buffer_p, int cellSize, uint32_t color) {
	for (int x = 0; x < window_width; x += cellSize) {
		for (int y = 0; y < window_height; y += cellSize) {
			buffer_p[(window_width * y) + x] = color;
		}
	}
}

void draw_rect(uint32_t* buffer_p, int xloc, int yloc, int width, int height, uint32_t color) {
	for (int x = xloc; x <= xloc + width - 1; x++) {
		for (int y = yloc; y <= yloc + height - 1; y++) {
			draw_pixel(buffer_p, x, y, color);
		}
	}
}

void draw_pixel(uint32_t* buffer_p, int x, int y, uint32_t color) {
	if (x >= 0 && x < window_width && y >= 0 && y < window_height) {
		buffer_p[loc_1d(x, y)] = color;
	}
}

void create_color_buffer_32(uint32_t** bufferP, int size) {
	*bufferP = (uint32_t*)malloc(sizeof(uint32_t) * size);
}

void clear_color_buffer(uint32_t color) {
	// TODO: pass size of color buffer and buffer and set with out nested loop?
	for (int y = 0; y < window_height; y++) {
		for (int x = 0; x < window_width; x++) {
			color_buffer[(window_width * y) + x] = color;
		}
	}
}

void setBufferPixel(int row, int col, uint32_t color) {
	color_buffer[(window_width * row) + col] = color;
}

// Render on the back buffer, render buffer to texture
void render_color_buffer(void) {
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int)window_width * sizeof(uint32_t));
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

// Formula to get index in 1d array based on x and y
int loc_1d(int x, int y) {
	return (window_width * y) + x;
}

vec2 project_point(vec3 vert) {
	vec2 projected = {
		vert.x * fov_factor / vert.z,
		vert.y * fov_factor / vert.z
	};

	return projected;
}