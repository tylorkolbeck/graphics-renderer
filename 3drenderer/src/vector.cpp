#include <math.h>
#include "vector.h"

vec3_t vec3_rotate_x(vec3_t v, float angle)
{
	vec3_t rotated = {
		v.x,
		v.y * cos(angle) - v.z * sin(angle),
		v.y * sin(angle) + v.z * cos(angle)};

	return rotated;
}

vec3_t vec3_rotate_y(vec3_t v, float angle)
{
	vec3_t rotated = {
		v.x * cos(angle) - v.z * sin(angle),
		v.y,
		v.x * sin(angle) + v.z * cos(angle)};

	return rotated;
}

vec3_t vec3_rotate_z(vec3_t v, float angle)
{
	vec3_t rotated = {
		v.x * cos(angle) - v.y * sin(angle),
		v.x * sin(angle) + v.y * cos(angle),
		v.z};

	return rotated;
}

float vec2_length(vec2_t v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

float vec3_mag(vec3_t v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec2_t vec2_sub(vec2_t a, vec2_t b)
{
	return {
		a.x - b.x,
		a.y - b.y};
}

vec3_t vec3_sub(vec3_t a, vec3_t b)
{
	return {
		a.x - b.x,
		a.y - b.y,
		a.z - b.z};
}

vec2_t vec2_add(vec2_t a, vec2_t b)
{
	return {
		a.x + b.x,
		a.y + b.y};
}

vec3_t vec3_add(vec3_t a, vec3_t b)
{
	return {
		a.x + b.x,
		a.y + b.y,
		a.z + b.z};
}

vec2_t vec2_mult(vec2_t v, float factor)
{
	return {
		v.x * factor,
		v.y * factor};
}

vec3_t vec3_mult(vec3_t v, float factor)
{
	return {
		v.x * factor,
		v.y * factor,
		v.z * factor};
}

vec2_t vec2_div(vec2_t v, float factor)
{
	if (factor == 0)
		return v;

	return {
		v.x / factor,
		v.y / factor};
}

vec3_t vec3_div(vec3_t v, float factor)
{
	if (factor == 0)
		return v;

	return {
		v.x / factor,
		v.y / factor,
		v.z / factor};
}

vec3_t vec3_cross(vec3_t a, vec3_t b)
{
	return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x};
}

float vec2_dot(vec2_t a, vec2_t b)
{
	return (a.x * b.x) + (a.y * b.y);
}

float vec3_dot(vec3_t a, vec3_t b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

void vec3_normalize(vec3_t *v)
{
	float magnitude = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);

	if (magnitude == 0)
	{
		v->x = 0;
		v->y = 0;
		v->z = 0;
	}
	else
	{
		v->x /= magnitude;
		v->y /= magnitude;
		v->z /= magnitude;
	}
}

void vec2_normalize(vec2_t *v)
{
	float magnitude = sqrt(v->x * v->x + v->y * v->y);
	if (magnitude == 0)
	{
		v->x = 0;
		v->y = 0;
	}
	else
	{
		v->x /= magnitude;
		v->y /= magnitude;
	}
}

