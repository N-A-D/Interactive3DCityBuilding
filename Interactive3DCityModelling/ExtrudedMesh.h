#pragma once
#include "Vector3D.h"
#include "Constants.h"
#include "Globals.h"
#include <tuple>
#include <functional>
#include "CubeMesh.h"
#include <gl/glew.h>
#include <gl/freeglut.h>

namespace icm {

	enum class BuildingType {
		RANDOM,
		SINE, 
		NONE
	};

	// Contains the vertices of an extruded mesh
	class ExtrudedMesh {
	public:
		ExtrudedMesh() {}
		ExtrudedMesh(const std::vector<float>& ambient, 
					const std::vector<float>& diffuse, 
					const std::vector<float>& specular, 
					const std::vector<float>& shininess,
			        BuildingType building_type,
					bool is_street_mesh = false);

		// Calculates the normal vector for a side panel
		Vector3D calculate_side_normal(const Vector3D& a,
									   const Vector3D& b,
			                           const Vector3D& c, 
			                           const Vector3D& d) const noexcept;

		// Draws the extruded mesh
		void draw() const noexcept;

		// Sine function scaling
		void sine_scaling(int floors, Vector3D bottom_left, Vector3D top_left, Vector3D top_right, Vector3D bottom_right) noexcept;

		// Random number scaling
		void random_scaling(int floors, Vector3D bottom_left, Vector3D top_left, Vector3D top_right, Vector3D bottom_right) noexcept;

		// Linear scaling
		void linear_scaling(int floors, Vector3D bottom_left, Vector3D top_left, Vector3D top_right, Vector3D bottom_right) noexcept;
		
		// No scaling
		void no_scaling(int floors, Vector3D bottom_left, Vector3D top_left, Vector3D top_right, Vector3D bottom_right) noexcept;

		

		std::vector<Vector3D> vertices;
		std::vector<float> ambient;
		std::vector<float> diffuse;
		std::vector<float> specular;
		std::vector<float> shininess;
		bool is_street_mesh;
	};
}