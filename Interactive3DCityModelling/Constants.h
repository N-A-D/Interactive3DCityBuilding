#pragma once

#include <string>
#include "Vector3D.h"

namespace icm {
	static const float street_mesh_offset_from_ground_mesh = 0.15f;
	static const int MESH_SIZE = 128;
	static const int WINDOW_WIDTH = 1200;
	static const int WINDOW_HEIGHT = 800;
	static const std::string WINDOW_TITLE = "Interactive 3D City Modelling";
	static const Vector3D street_ambient = { .45f, .45f, 0.45f };
	static const Vector3D street_diffuse = { .45f, .45f, 0.45f };
	static const Vector3D street_specular = { .1f, .1f, 0.1f };
	static const Vector3D street_offset = { 0.0f, 0.3f, 0.0f };
	static const Vector3D cube_starting_pos = { -55.0f, 1.0f, 55.0f };
	static const Vector3D cube_starting_scale_factors = { 1.0f, 1.0f, 1.0f };
	static const double far_plane = 160.0;
	static const double near_plane = 0.2;
}