#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool is_running = false;

bool initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL. \n");
		return false;
	}

	// Create SDL window
	window = SDL_CreateWindow(NULL, 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		800, 
		600, 
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
	return true;
}

void setup(void) {
	// TODO:
	is_running = initialize_window();
}

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		is_running = false;
		break;
	}
	
}

void update() {
	// TODO:
}

void render(void) {
	// TODO:
}

int main(int argc, char* args[]) {
	setup();

	while (is_running) {
		process_input();
		update();
		render();
	}

	return 0;
}