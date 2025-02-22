#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "vector.h"
#include "triangles.h"

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)


extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;
extern int window_width;
extern int window_height;
extern float fov_factor;
extern vec3 camera;
extern vec3 cube_rotation;



bool initialize_window(void);
void destroy_window(void);
void draw_grid(uint32_t* buffer_p, int cellSize, uint32_t color);
void draw_dot(uint32_t* buffer_p, int cellSize, uint32_t color);
void draw_pixel(uint32_t* buffer_p, int x, int y, uint32_t color);
void draw_rect(uint32_t* buffer_p, int xloc, int yloc, int width, int height, uint32_t color);
void create_color_buffer_32(uint32_t** bufferP, int size);
void clear_color_buffer(uint32_t color);
void setBufferPixel(int row, int col, uint32_t color);
void render_color_buffer(void);
int loc_1d(int x, int y);
vec2 project_point(vec3 vert);
void draw_line(uint32_t* buffer_p, vec2 a, vec2 b, uint32_t color);
void draw_triangle(uint32_t* buffer_p, triangle tri, uint32_t color);


#endif
