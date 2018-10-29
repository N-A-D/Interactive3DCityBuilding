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

	class ExtrudedMesh {
	public:

		ExtrudedMesh(const std::vector<float>& ambient, 
					const std::vector<float>& diffuse, 
					const std::vector<float>& specular, 
					const std::vector<float>& shininess, 
					bool is_street_mesh = false);

		Vector3D calculate_side_normal(const Vector3D& a, 
									   const Vector3D& b, 
			                           const Vector3D& c, 
			                           const Vector3D& d) const noexcept;

		void draw() const noexcept;

	private:
		//std::function<float(float)> scaling_function;
		std::vector<Vector3D> vertices;
		std::vector<float> ambient;
		std::vector<float> diffuse;
		std::vector<float> specular;
		std::vector<float> shininess;
		bool is_street_mesh;
	};
}