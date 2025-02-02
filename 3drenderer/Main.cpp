#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>

// Window settings
int window_width = 800;
int window_height = 600;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool is_running = false;

// Color buffer is copied to texture for rendering
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

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

void create_color_buffer_32(uint32_t** bufferP, int size) {
	*bufferP = (uint32_t*)malloc(sizeof uint32_t * size);
}

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
		break;
	}
}

void update() {
	// TODO:
}

void clear_color_buffer(uint32_t color) {
	// TODO: pass size of color buffer and buffer and set with out nested loop?
	for (int y = 0; y < window_height; y++) {
		for (int x = 0; x < window_width; x++) {
			color_buffer[(window_width * y) + x] = color;
		}
	}
}

// Render on the back buffer, render buffer to texture
void render_color_buffer(void) {
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL, 
		color_buffer,
		(int)window_width * sizeof uint32_t);
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void render(void) {
	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	render_color_buffer(); // Render the color buffer to the texture

	clear_color_buffer(0xFFFFFF00); // Set each pixel to yellow color in buffer

	SDL_RenderPresent(renderer);
}

void setBufferPixel(int row, int col, uint32_t color) {
	color_buffer[(window_width * row) + col] = color;
}

void cleanup(void) {
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char* args[]) {
	is_running = initialize_window();

	setup();

	while (is_running) {
		process_input();
		update();
		render();
	}

	cleanup();

	return 0;
}