#pragma once
#include "vector.h"
#include <cstdint>

typedef struct {
	int a;
	int b;
	int c;
	uint32_t color;
} face;

typedef struct {
	vec2_t points[3];
	uint32_t color;
} triangle;

// void draw_filled_triangle(uint32_t* buffer_p, triangle tri, uint32_t color);
void draw_filled_triangle(uint32_t* buffer_p, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void fill_flat_bottom_triangle(uint32_t* buffer_p, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void fill_flat_top_triangle(uint32_t* buffer_p, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);