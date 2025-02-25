#pragma once

#include "vector.h"

typedef struct {
	int a;
	int b;
	int c;
	vec3_t normal;
} face;

typedef struct {
	vec2_t points[3];
} triangle;