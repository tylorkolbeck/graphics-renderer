#pragma once
#include <string>
#include <fstream>
#include <optional>
#include "mesh.h"

void parse_obj_file(std::string& path, mesh_t& meshObj);
void parseFace(const std::string& line, std::vector<face>& mesh_faces);