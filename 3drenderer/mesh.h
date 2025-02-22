#pragma once

#include "vector.h"
#include "triangles.h"

constexpr int N_MESH_VERTICES = 8;
constexpr int N_MESH_FACES = 6 * 2;

extern vec3 mesh_vertices[N_MESH_VERTICES];
extern face mesh_faces[N_MESH_FACES];

