#include <vector>
#include "mesh.h"
#include "color.h"

// mesh_t mesh;	

std::vector<vec3_t> mesh_vertices = {
	{-1, -1, -1},
	{-1, 1, -1},
	{1, 1, -1},
	{1, -1, -1},
	{1, 1, 1},
	{1, -1, 1},
	{-1, 1, 1},
	{-1, -1, 1}};

std::vector<face> mesh_faces = {
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

std::vector<face> cube_mesh_faces = {
	// front
	{1, 2, 3, DODGER_BLUE},
	{1, 3, 4, DODGER_BLUE},
	// right
	{4, 3, 5, C_BLUE},
	{4, 5, 6, C_BLUE},
	// back
	{6, 5, 7, C_TEAL},
	{6, 7, 8, C_TEAL},
	// left
	{8, 7, 2, C_PINK},
	{8, 2, 1, C_PINK},
	// top
	{2, 7, 5, C_GREEN},
	{2, 5, 3, C_GREEN},
	// bottom
	{6, 8, 1, C_PINK},
	{6, 1, 4, C_PINK}};

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