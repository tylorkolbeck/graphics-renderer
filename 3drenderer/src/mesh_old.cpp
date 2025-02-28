#include <vector>
#include "mesh_old.h"
#include "color.h"

std::vector<vec3_t> mesh_vertices = {
	{-1, -1, -1},
	{-1, 1, -1},
	{1, 1, -1},
	{1, -1, -1},
	{1, 1, 1},
	{1, -1, 1},
	{-1, 1, 1},
	{-1, -1, 1}};

std::vector<face_t> mesh_faces = {
	// front
	{1, 2, 3},
	{1, 3, 4},
	// right
	{4, 3, 5},
	{4, 5, 6},
	// back
	{6, 5, 7},
	{6, 7, 8},
	// left
	{8, 7, 2},
	{8, 2, 1},
	// top
	{2, 7, 5},
	{2, 5, 3},
	// bottom
	{6, 8, 1},
	{6, 1, 4}};

std::vector<vec3_t> cube_mesh_vertices = {
	{-1, -1, -1},
	{-1, 1, -1},
	{1, 1, -1},
	{1, -1, -1},
	{1, 1, 1},
	{1, -1, 1},
	{-1, 1, 1},
	{-1, -1, 1}};

std::vector<vec3_t> point_mesh_vertices = {
	{0.0, 0.0, 0.0}
};

std::vector<face_t> point_mesh_faces = {
	{1, 1, 1, Color::DODGER_BLUE}
};

std::vector<face_t> cube_mesh_faces = {
	// front
	{1, 2, 3, Color::C_WHITE},
	{1, 3, 4, Color::C_WHITE},
	// right
	{4, 3, 5, Color::C_WHITE},
	{4, 5, 6, Color::C_WHITE},
	// back
	{6, 5, 7, Color::C_WHITE},
	{6, 7, 8, Color::C_WHITE},
	// left
	{8, 7, 2, Color::C_WHITE},
	{8, 2, 1, Color::C_WHITE},
	// top
	{2, 7, 5, Color::C_WHITE},
	{2, 5, 3, Color::C_WHITE},
	// bottom
	{6, 8, 1, Color::C_WHITE},
	{6, 1, 4, Color::C_WHITE}};

void load_point_mesh(mesh_t& meshObj)
{
	for (int i = 0; i < point_mesh_vertices.size(); i++)
	{
		meshObj.vertices.push_back(point_mesh_vertices[i]);
	}

	for (int i = 0; i < point_mesh_faces.size(); i++)
	{
		meshObj.faces.push_back(point_mesh_faces[i]);
	}
}

void load_cube_mesh(mesh_t& meshObj)
{
	for (int i = 0; i < cube_mesh_vertices.size(); i++)
	{
		meshObj.vertices.push_back(cube_mesh_vertices[i]);
	}

	for (int i = 0; i < cube_mesh_faces.size(); i++)
	{
		meshObj.faces.push_back(cube_mesh_faces[i]);
	}
}