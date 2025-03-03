#pragma once

#include "vector.h"
#include "triangles.h"
#include "color.h"
#include "matrix.h"
#include "light.h"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "renderer.h"

// void load_cube_mesh(mesh_t &meshObj);
// void load_point_mesh(mesh_t &meshObj);

class Mesh
{
private:
    std::vector<vec3_t> m_vertices{};
    std::vector<face_t> m_faces{};
    vec3_t m_rotation{0.0f, 0.0f, 0.0f};
    vec3_t m_scale{1.0f, 1.0f, 1.0f};
    vec3_t m_position{0.0f, 0.0f, 0.0f};
    std::vector<triangle_t> m_render_queue;
    void parse_obj_file(const std::string &path);
    void parseFace(const std::string &line);

public:
    Mesh();
    Mesh(std::vector<vec3_t> vertices, std::vector<face_t> faces);
    Mesh(const std::string &path);
    void rotate(vec3_t rotation);
    void scale(vec3_t scale);
    void translate(vec3_t translation);
    vec3_t& rotation();
    vec3_t& scale();
    vec3_t& translation();
    vec3_t& position();
    size_t faceCount();
    const std::vector<vec3_t>& vertices();
    const std::vector<face_t>& faces();
    
    void update(vec3_t camera, mat4_t proj_matrix, light_t light, bool cull, Window_t window);
    void render(uint32_t* color_buffer);

};