#include "Globals.h"
#include "Constants.h"

int icm::frame_count = 0;
QuadMesh icm::ground_mesh;
CubeMesh icm::current_cube;
std::vector<icm::ExtrudedMesh> icm::extruded_meshes;
bool icm::translate_cube = false;
bool icm::scale_horizontal = false;
bool icm::scale_vertical = false;
Vector3D icm::current_cube_position = icm::cube_starting_pos;
Vector3D icm::cube_scale_factors = icm::cube_starting_scale_factors;