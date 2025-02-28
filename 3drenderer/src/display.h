#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "vector.h"
#include "triangles.h"

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

typedef enum cull_method {
    CULL_NONE,
    CULL_BACKFACE
} cull_method_t;

typedef enum render_method {
    RENDER_WIRE,
    RENDER_WIRE_VERTEX,
    RENDER_FILL_TRIANGLE,
    RENDER_FILL_TRIANGLE_WIRE
} render_method_t;

struct Window {
    int width;
    int height;
};

extern cull_method_t cull_method;
extern render_method_t render_method;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;
extern int window_width;
extern int window_height;
extern vec3_t camera;
extern bool show_grid;
extern bool cull_faces;

bool initialize_window(void);
void destroy_window(void);
void draw_grid(uint32_t* buffer_p, int cellSize, uint32_t color);
void draw_dot(uint32_t* buffer_p, int cellSize, uint32_t color);
void draw_pixel(uint32_t* buffer_p, int x, int y, uint32_t color);
void draw_rect(uint32_t* buffer_p, int xloc, int yloc, int width, int height, uint32_t color);
void draw_rect(uint32_t *buffer_p, int xloc, int yloc, int width, int height);
void create_color_buffer_32(uint32_t** bufferP, int size);
void clear_color_buffer(uint32_t color);
void setBufferPixel(int row, int col, uint32_t color);
void render_color_buffer(void);
int loc_1d(int x, int y);
vec2_t project(vec3_t vert);
void draw_line(uint32_t* buffer_p, vec2_t a, vec2_t b, uint32_t color);
void draw_line(uint32_t *buffer_p, int x1, int y1, int x2, int y2, uint32_t color);
void draw_triangle(uint32_t* buffer_p, triangle_t tri, uint32_t color);
bool is_culled(vec3_t a, vec3_t b, vec3_t c);
bool is_culled(vec4_t vertices[]);


#endif
