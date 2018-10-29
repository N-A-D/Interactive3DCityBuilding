#pragma once

#include <vector>
#include <string>
#include "Vector3D.h"

namespace icm {

	static const int MESH_SIZE = 64;

	static const int WINDOW_WIDTH = 1200;
	static const int WINDOW_HEIGHT = 800;
	static const std::string WINDOW_TITLE = "Interactive 3D City Modelling";

	static const Vector3D street_ambient = { .15f, .15f, 0.15f };
	static const Vector3D street_diffuse = { .15f, .15f, 0.15f };
	static const Vector3D street_specular = { .1f, .1f, 0.1f };
	static const Vector3D street_offset = { 0.0f, 0.3f, 0.0f };
	static const double street_shininess = 0.0;

	static const std::vector<float> street_mesh_ambient = { .15f, .15f, 0.15f, 1.0f };
	static const std::vector<float> street_mesh_diffuse = { .15f, .15f, 0.15f, 1.0f };
	static const std::vector<float> street_mesh_specular = { .1f, .1f, 0.1f, 1.0f };
	static const std::vector<float> street_mesh_shininess = { 0.0 };

	static const Vector3D cube_ambient = { 0.0f, 0.5f, 0.0f };
	static const Vector3D cube_diffuse = { 0.0f, 0.5f, 0.0f };
	static const Vector3D cube_specular = { 0.2f, 0.2f, 0.2f };
	static const double cube_shininess = 0.1;

	static const std::vector<float> building_ambient = { 0.0f, 0.5f, 0.0f, 1.0f };
	static const std::vector<float> building_diffuse = { 0.0f, 0.5f, 0.0f, 1.0f, };
	static const std::vector<float> building_specular = { 0.0f, 0.0f, 0.004f, 1.0f };
	static const std::vector<float> building_shininess = { 0.0f };

	static const Vector3D cube_starting_pos = { -30.0f, 1.0f, 25.0f };
	static const Vector3D cube_starting_scale_factors = { 1.0f, 1.0f, 1.0f };
	static const double translation_amount = 1.5f;
	static const double scale_amount = 0.2f;

	static const double near_plane = .2;
	static const double far_plane = 160.0;
	
	static const float floor_height = 1.0f;


	
}