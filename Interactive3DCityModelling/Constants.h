#pragma once

#include <string>
#include "Vector3D.h"

namespace icm {

	static const int MESH_SIZE = 64;

	static const int WINDOW_WIDTH = 1200;
	static const int WINDOW_HEIGHT = 800;
	static const std::string WINDOW_TITLE = "Interactive 3D City Modelling";

	static const Vector3D street_ambient = { .45f, .45f, 0.45f };
	static const Vector3D street_diffuse = { .45f, .45f, 0.45f };
	static const Vector3D street_specular = { .1f, .1f, 0.1f };
	static const Vector3D street_offset = { 0.0f, 0.3f, 0.0f };
	static const double street_shininess = 0.1;

	static const Vector3D building_ambient = { 0.0f, 0.5f, 0.0f };
	static const Vector3D building_diffuse = { 0.0f, 0.5f, 0.0f };
	static const Vector3D building_specular = { 0.2f, 0.2f, 0.2f };
	static const double building_shininess = 0.1;

	static const Vector3D cube_starting_pos = { -30.0f, 1.0f, 25.0f };
	static const Vector3D cube_starting_scale_factors = { 1.0f, 1.0f, 1.0f };
	static const double translation_amount = 1.0f;
	static const double scale_amount = 0.1f;

	static const double near_plane = 0.2;
	static const double far_plane = 160.0;
	
}