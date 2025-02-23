#include <vector>
#include "mesh.h"

mesh_t mesh;	

std::vector<vec3> mesh_vertices = {
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

std::vector<vec3> cube_mesh_vertices = {
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

void load_cube_mesh(void)
{
	for (int i = 0; i < cube_mesh_vertices.size(); i++)
	{
		mesh.vertices.push_back(cube_mesh_vertices[i]);
	}

	for (int i = 0; i < cube_mesh_faces.size(); i++)
	{
		mesh.faces.push_back(cube_mesh_faces[i]);
	}
}