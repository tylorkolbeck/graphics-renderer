#ifndef CAMERA_H
#define CAMERA_H
#include "vector.h"

typedef struct {
	vec3 location;
	vec3 rotation;
	float fovAngle;
} camera;

#endif
