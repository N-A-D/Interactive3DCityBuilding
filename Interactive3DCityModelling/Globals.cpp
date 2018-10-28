#include "Globals.h"
#include "Constants.h"

int icm::frame_count = 0;

Vector3D icm::camera_position = { 0.0f, 70.0f, 90.0f };
double icm::FOV = 45.0;

QuadMesh icm::ground_mesh;
std::vector<icm::ExtrudedMesh> icm::extruded_meshes;

CubeMesh icm::current_cube;
bool icm::is_current_cube_active = false;
bool icm::is_street = false;
bool icm::translate_cube = false;
bool icm::scale_horizontal = false;
bool icm::scale_vertical = false;
Vector3D icm::current_cube_position = icm::cube_starting_pos;
Vector3D icm::cube_scale_factors = icm::cube_starting_scale_factors;