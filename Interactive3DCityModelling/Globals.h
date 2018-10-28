#pragma once

#include <vector>
#include "QuadMesh.h"
#include "CubeMesh.h"
#include "ExtrudedMesh.h"

namespace icm {
	extern int frame_count;
	extern QuadMesh ground_mesh;
	extern CubeMesh current_cube;
	extern std::vector<ExtrudedMesh> extruded_meshes;
	extern bool translate_cube;
	extern bool scale_vertical;
	extern bool scale_horizontal;
	extern Vector3D current_cube_position;
	extern Vector3D cube_scale_factors;
}