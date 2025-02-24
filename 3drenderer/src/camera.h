#ifndef CAMERA_H
#define CAMERA_H
#include "vector.h"

typedef struct {
	vec3_t location;
	vec3_t rotation;
	float fovAngle;
} camera;

#endif
