#pragma once

#include <vector>
#include "QuadMesh.h"
#include "CubeMesh.h"
#include "ExtrudedMesh.h"

namespace icm {
	extern int frame_count;                           // Current FPS
	extern QuadMesh ground_mesh;                      // The ground mesh
	extern CubeMesh current_cube;                     // The current cube mesh being translated and scaled
	extern std::vector<ExtrudedMesh> extruded_meshes; // Array of extruded meshes to save if prompted
	extern bool is_current_cube_active;                  // Signal to check if the current cube is being used to create a building
	extern bool is_street;                            // Signal to check if the current cube mesh is a street
	extern bool translate_cube;	                      // Signal to translate the current cube mesh
	extern bool scale_vertical;                       // Signal to scale the cube mesh vertically
	extern bool scale_horizontal;                     // Signal to scale the cube mesh horizontally
	extern Vector3D current_cube_position;            // Passed to ExtrudedMesh object
	extern Vector3D cube_scale_factors;               // Passed to ExtrudedMesh object
}