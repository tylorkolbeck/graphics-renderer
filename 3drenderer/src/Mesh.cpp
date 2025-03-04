#include "Mesh.h"
#include "light.h"
#include "Window.h"
#include "Renderer.h"

Mesh::Mesh()
	: m_rotation{0, 0, 0},
	  m_scale{1, 1, 1},
	  m_position{0, 0, 0}
{
}

Mesh::Mesh(std::vector<vec3_t> vertices, std::vector<face_t> faces) : m_vertices{vertices},
																	  m_faces{faces}
{
}

Mesh::Mesh(const std::string &path)
{
	parse_obj_file(path);
	m_render_queue.resize(faceCount());
}

void Mesh::rotate(vec3_t rotation)
{
	m_rotation = rotation;
}

void Mesh::scale(vec3_t scale)
{
	m_scale = scale;
}

void Mesh::translate(vec3_t translation)
{
	m_position = translation;
}

// TODO: Refactor this method
void Mesh::update(vec3_t camera, mat4_t proj_matrix, light_t light, bool cull, Window *window)
{
	m_render_queue.clear();
	// 1️. Apply transformations to the model matrix

	mat4_t scale_matrix = mat4_make_scale(scale().x, scale().y, scale().z);
	mat4_t rotation_matrix = mat4_make_rotation(rotation());
	mat4_t translation_matrix = mat4_make_translation(translation().x, translation().y, translation().z);

	mat4_t world_matrix = mat4_identity() * translation_matrix * rotation_matrix * scale_matrix;

	// Preallocate the memory required for the faces
	m_render_queue.reserve(faceCount());

	for (size_t i = 0; i < faceCount(); i++)
	{
		const face_t &mesh_face = faces()[i];
		vec3_t face_vertices[3];

		face_vertices[0] = vertices()[mesh_face.a - 1];
		face_vertices[1] = vertices()[mesh_face.b - 1];
		face_vertices[2] = vertices()[mesh_face.c - 1];

		vec4_t transformed_vertices[3];
		// Loop all three vertices of this current face and apply transformations
		for (int j = 0; j < 3; j++)
		{
			// Apply transformation to put in correct position in the world
			vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);
			// Multiple the world matrix by original vector
			transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);
			// Save transformed vertex in the array of transformed vertices
			transformed_vertices[j] = transformed_vertex;
		}

		if (cull)
		{
			// Check backface culling
			vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]); /* 	A	*/
			vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]); /*  / \  */
			vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]); /* C___B */
			// 1. Find vectors B-A and C-A
			vec3_t vector_ab = vec3_sub(vector_b, vector_a);
			vec3_t vector_ac = vec3_sub(vector_c, vector_a);
			vec3_normalize(&vector_ab);
			vec3_normalize(&vector_ac);

			// 2. Compute face normal using cross product of ab and ac - Order matters based on winding direction
			vec3_t normal = vec3_cross(vector_ab, vector_ac);
			vec3_normalize(&normal);

			// 3. Find the camera ray vector by subtracting the camera position from point A
			vec3_t camera_ray = vec3_sub({camera.x, camera.y, camera.z}, vector_a);
			vec3_normalize(&camera_ray);
			// 4. Take the dot product between the normal N and the camera ray
			float dot_normal_camera = vec3_dot(normal, camera_ray);
			// 5. If this dot product is less than zero, then do not display the face
			if (dot_normal_camera < 0)
				continue;

			// Loop all three transformed vertices to perform projection
			vec4_t projected_points[3];
			for (int j = 0; j < 3; j++)
			{
				// Apply projection
				projected_points[j] = mat4_mul_vec4_project(proj_matrix, transformed_vertices[j]);

				projected_points[j].x = (1 - projected_points[j].x) * (window->getWidth() / 2.0);
				projected_points[j].y = (projected_points[j].y + 1) * (window->getHeight() / 2.0);
			}

			float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3;

			// Calculate the shade intensity based on how aligned the face normal and light are
			float light_intensity_factor = std::max(0.0f, vec3_dot(normal, vec3_inverse(light.direction)));

			uint32_t triangle_color = light_apply_intensity(mesh_face.color, light_intensity_factor);

			triangle_t projected_triangle = {
				.points = {
					{projected_points[0].x, projected_points[0].y},
					{projected_points[1].x, projected_points[1].y},
					{projected_points[2].x, projected_points[2].y},
				},
				.color = triangle_color,
				.avg_depth = avg_depth};

			m_render_queue.push_back(projected_triangle);
		}
	}

	std::sort(m_render_queue.begin(), m_render_queue.end(), compareAvg);
}

void Mesh::render(Renderer *renderer)
{
	for (size_t i = 0; i < m_render_queue.size(); i++)
	{
		triangle_t triangles = m_render_queue[i];
		renderer->drawFilledTriangle(
			triangles.points[0].x, triangles.points[0].y, // vertex A
			triangles.points[1].x, triangles.points[1].y, // vertex B
			triangles.points[2].x, triangles.points[2].y, // vertex C
			triangles.color);

		// Draw filled triangles
		// if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE)
		// {
		// 	draw_filled_triangle(
		// 		color_buffer,
		// 		triangles.points[0].x, triangles.points[0].y, // vertex A
		// 		triangles.points[1].x, triangles.points[1].y, // vertex B
		// 		triangles.points[2].x, triangles.points[2].y, // vertex C
		// 		triangles.color);
		// }

		// // Draw triangle wireframe
		// if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE)
		// {
		// 	draw_triangle(
		// 		color_buffer,
		// 		triangles,
		// 		Color::C_WHITE);
		// }

		// if (render_method == RENDER_WIRE_VERTEX)
		// {
		// 	draw_rect(color_buffer, triangles.points[0].x - 3, triangles.points[0].y - 3, 6, 6);
		// 	draw_rect(color_buffer, triangles.points[1].x - 3, triangles.points[1].y - 3, 6, 6);
		// 	draw_rect(color_buffer, triangles.points[2].x - 3, triangles.points[2].y - 3, 6, 6);
		// }
	}
}

vec3_t &Mesh::rotation()
{
	return m_rotation;
}

vec3_t& Mesh::scale()
{
	return m_scale;
}

vec3_t& Mesh::translation()
{
	return m_position;
}

vec3_t& Mesh::position()
{
	return m_position;
}

size_t Mesh::faceCount()
{
	return m_faces.size();
}

const std::vector<vec3_t> &Mesh::vertices()
{
	return m_vertices;
}

const std::vector<face_t> &Mesh::faces()
{
	return m_faces;
}

bool Mesh::compareAvg(const triangle_t &a, const triangle_t &b)
{
	return a.avg_depth > b.avg_depth;
}

void Mesh::parse_obj_file(const std::string &path)
{
	std::ifstream file;
	file.open(path);

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty())
			continue;

		if (line[0] == 'v' && line[1] == ' ')
		{
			vec3_t vertex;
			sscanf(line.c_str(), "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
			m_vertices.push_back(vertex);
		}
		else if (line[0] == 'f' && line[1] == ' ')
		{
			parseFace(line);
		}
	}
	std::cout << "[Loaded Mesh]: " << path << " Vertices: " << m_vertices.size() << " Faces: " << m_faces.size() << std::endl;
}

void Mesh::parseFace(const std::string &line)
{
	std::istringstream ss(line);
	std::string prefix;
	face_t face;

	face.color = Color::C_LIGHTGREY; // Default light grey for faces

	ss >> prefix; // Read "f"

	char slash; // To consume the '/' characters
	int v;		// Temporary variable for vertex index

	// Read up to 4 vertices per face
	if (ss >> face.a >> slash >> v >> slash >> v &&
		ss >> face.b >> slash >> v >> slash >> v &&
		ss >> face.c >> slash >> v >> slash >> v)
	{

		m_faces.push_back(face);
	}
	else
	{
		std::cerr << "Error parsing face: " << line << std::endl;
	}
}

