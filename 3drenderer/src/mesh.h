#pragma once

#include "vector.h"
#include "triangles.h"
#include <vector>

extern std::vector<vec3_t> cube_mesh_vertices;
extern std::vector<face_t> cube_mesh_faces;

extern std::vector<vec3_t> mesh_vertices;
extern std::vector<face_t> mesh_faces;
// extern mesh_t mesh;

typedef struct
{
    std::vector<vec3_t> vertices;
    std::vector<face_t> faces;
    vec3_t rotation = {0, 0, 0};
    vec3_t scale = {1.0, 1.0, 1.0};
    vec3_t translation = {0.0, 0.0, 0.0};
} mesh_t;

void load_cube_mesh(mesh_t &meshObj);
void load_point_mesh(mesh_t& meshObj);