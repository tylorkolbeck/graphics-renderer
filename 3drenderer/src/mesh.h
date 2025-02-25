#pragma once

#include "vector.h"
#include "triangles.h"
#include <vector>

extern std::vector<vec3_t> cube_mesh_vertices;
extern std::vector<face> cube_mesh_faces;

extern std::vector<vec3_t> mesh_vertices;
extern std::vector<face> mesh_faces;
// extern mesh_t mesh;

typedef struct
{
    std::vector<vec3_t> vertices;
    std::vector<face> faces;
    vec3_t rotation = {0, 0, 0};
} mesh_t;

void load_cube_mesh(mesh_t& meshObj);