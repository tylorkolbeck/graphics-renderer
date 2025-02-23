#pragma once

#include "vector.h"
#include "triangles.h"
#include <vector>

constexpr int N_MESH_VERTICES = 8;
constexpr int N_MESH_FACES = 6 * 2;

extern std::vector<vec3> mesh_vertices;
extern std::vector<face> mesh_faces;

