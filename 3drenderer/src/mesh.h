#pragma once

#include "vector.h"
#include "triangles.h"
#include <vector>

extern std::vector<vec3> cube_mesh_vertices;
extern std::vector<face> cube_mesh_faces;

extern std::vector<vec3> mesh_vertices;
extern std::vector<face> mesh_faces;

typedef struct
{
    std::vector<vec3> vertices;
    std::vector<face> faces;
    vec3 rotation = {0, 0, 0};
} mesh_t;

extern mesh_t mesh;

void load_cube_mesh(void);