#include <math.h>
#include "vector.h"

vec3 vec3_rotate_x(vec3 v, float angle) {
	vec3 rotated = {
		v.x,
		v.y * cos(angle) - v.z * sin(angle),
		v.y * sin(angle) + v.z * cos(angle)
	};

	return rotated;
}

vec3 vec3_rotate_y(vec3 v, float angle) {
	vec3 rotated = {
		v.x * cos(angle) - v.z * sin(angle),
		v.y,
		v.x * sin(angle) + v.z * cos(angle)
	};

	return rotated;
}

vec3 vec3_rotate_z(vec3 v, float angle) {
	vec3 rotated = {
		v.x * cos(angle) - v.y * sin(angle),
		v.x * sin(angle) + v.y * cos(angle),
		v.z
	};

	return rotated;
}