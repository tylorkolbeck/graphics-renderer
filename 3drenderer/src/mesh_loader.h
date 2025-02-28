#pragma once
#include <string>
#include <fstream>
#include <optional>
#include "mesh_old.h"

void parse_obj_file(std::string& path, mesh_t& meshObj);
void parseFace(const std::string& line, std::vector<face_t>& mesh_faces);