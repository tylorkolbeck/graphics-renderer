#include "mesh_loader.h"
#include <optional>
#include <iostream>
#include <cerrno>
#include <fstream>
#include <cstring>
#include <filesystem>
#include <sstream>
#include "mesh.h"

void parse_obj_file(std::string& path, mesh_t& meshObj)
{
    std::ifstream file;
    file.open(path);


    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        if (line[0] == 'v' && line[1] == ' ')
        {
            vec3_t vertex;
            sscanf(line.c_str(), "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            meshObj.vertices.push_back(vertex);
        }
        else if (line[0] == 'f' && line[1] == ' ')
        {
            parseFace(line, meshObj.faces);
        }
    }
    std::cout << "Number of vertices: " << meshObj.vertices.size() << " Number of faces: " << meshObj.faces.size() << std::endl;
}

void parseFace(const std::string& line, std::vector<face>& mesh_faces) 
{
    std::istringstream ss(line);
    std::string prefix;
    face face;
    
    ss >> prefix; // Read "f"
    
    char slash; // To consume the '/' characters
    int v; // Temporary variable for vertex index

    // Read up to 4 vertices per face
    if (ss >> face.a >> slash >> v >> slash >> v &&
        ss >> face.b >> slash >> v >> slash >> v &&
        ss >> face.c >> slash >> v >> slash >> v) {

        mesh_faces.push_back(face);
    } else {
        std::cerr << "Error parsing face: " << line << std::endl;
    }
}