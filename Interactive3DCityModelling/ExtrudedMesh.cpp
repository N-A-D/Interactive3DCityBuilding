#include "ExtrudedMesh.h"
#include <iostream>

icm::ExtrudedMesh::ExtrudedMesh(const std::vector<float>& ambient,
								const std::vector<float>& diffuse,
								const std::vector<float>& specular,
								const std::vector<float>& shininess,
								bool is_street)
	: ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
{
	auto width = icm::cube_scale_factors.x * 2.0f;
	auto height = icm::cube_scale_factors.y * 2.0f;
	auto depth = icm::cube_scale_factors.z * 2.0f;

	Vector3D bottom_left = NewVector3D(current_cube_position.x - width / 2.0f, current_cube_position.y - height / 2.0f,
		current_cube_position.z + depth / 2.0f);

	Vector3D top_left = NewVector3D(current_cube_position.x - width / 2.0f, current_cube_position.y - height / 2.0f,
		current_cube_position.z - depth / 2.0f);

	Vector3D top_right = NewVector3D(current_cube_position.x + width / 2.0f, current_cube_position.y - height / 2.0f,
		current_cube_position.z - depth / 2.0f);

	Vector3D bottom_right = NewVector3D(current_cube_position.x + width / 2.0f, current_cube_position.y - height / 2.0f,
		current_cube_position.z + depth / 2.0f);

	vertices.push_back(bottom_left);
	vertices.push_back(top_left);
	vertices.push_back(top_right);
	vertices.push_back(bottom_right);

	if (!is_street) {
		int floors = static_cast<int>(height / icm::floor_height);
		for (int i = 1; i <= floors; i++) {

			vertices.push_back(NewVector3D(bottom_left.x, bottom_left.y + i * icm::floor_height, bottom_left.z));
			vertices.push_back(NewVector3D(top_left.x, top_left.y + i * icm::floor_height, top_left.z));
			vertices.push_back(NewVector3D(top_right.x, top_right.y + i * icm::floor_height, top_right.z));
			vertices.push_back(NewVector3D(bottom_right.x, bottom_right.y + i * icm::floor_height, bottom_right.z));

		}
	}
	else {
		vertices.push_back(NewVector3D(bottom_left.x, bottom_left.y + icm::street_offset.y, bottom_left.z));
		vertices.push_back(NewVector3D(top_left.x, top_left.y + icm::street_offset.y, top_left.z));
		vertices.push_back(NewVector3D(top_right.x, top_right.y + icm::street_offset.y, top_right.z));
		vertices.push_back(NewVector3D(bottom_right.x, bottom_right.y + icm::street_offset.y, bottom_right.z));
	}
}

Vector3D icm::ExtrudedMesh::calculate_side_normal(const Vector3D & a, const Vector3D & b, const Vector3D & c, const Vector3D & d) const noexcept
{
	Vector3D u, w, x;
	Subtract(&b, &a, &u);
	Subtract(&d, &c, &w);
	CrossProduct(&u, &w, &x);
	Normalize(&x);
	return x;
}

void icm::ExtrudedMesh::draw() const noexcept
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient.data());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse.data());
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular.data());
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess.data());

	glPushMatrix();
	glBegin(GL_QUADS);

	Vector3D prev_bl = vertices[0];
	Vector3D prev_tl = vertices[1];
	Vector3D prev_tr = vertices[2];
	Vector3D prev_br = vertices[3];
	Vector3D normal;

	for (std::size_t i = 4; i < vertices.size(); i += 4) {
		Vector3D curr_bl = vertices[i];
		Vector3D curr_tl = vertices[i + 1];
		Vector3D curr_tr = vertices[i + 2];
		Vector3D curr_br = vertices[i + 3];

		normal = calculate_side_normal(prev_bl, curr_br, prev_br, curr_bl);
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(curr_bl.x, curr_bl.y, curr_bl.z);
		glVertex3f(curr_br.x, curr_br.y, curr_br.z);
		glVertex3f(prev_br.x, prev_br.y, prev_br.z);
		glVertex3f(prev_bl.x, prev_bl.y, prev_bl.z);

		normal = calculate_side_normal(prev_br, curr_tr, prev_tr, curr_br);
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(curr_br.x, curr_br.y, curr_br.z);
		glVertex3f(curr_tr.x, curr_tr.y, curr_tr.z);
		glVertex3f(prev_tr.x, prev_tr.y, prev_tr.z);
		glVertex3f(prev_br.x, prev_br.y, prev_br.z);

		normal = calculate_side_normal(prev_bl, curr_tl, prev_tl, curr_tl);
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(curr_bl.x, curr_bl.y, curr_bl.z);
		glVertex3f(curr_tl.x, curr_tl.y, curr_tl.z);
		glVertex3f(prev_tl.x, prev_tl.y, prev_tl.z);
		glVertex3f(prev_bl.x, prev_bl.y, prev_bl.z);

		normal = calculate_side_normal(prev_tl, curr_tr, prev_tr, curr_tl);
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(curr_tl.x, curr_tl.y, curr_tl.z);
		glVertex3f(curr_tr.x, curr_tr.y, curr_tr.z);
		glVertex3f(prev_tr.x, prev_tr.y, prev_tr.z);
		glVertex3f(prev_tl.x, prev_tl.y, prev_tl.z);

		prev_bl = curr_bl;
		prev_br = curr_br;
		prev_tl = curr_tl;
		prev_tr = curr_tr;
	}

	normal = calculate_side_normal(prev_bl, prev_tr, prev_br, prev_tl);
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(prev_tl.x, prev_tl.y, prev_tl.z);
	glVertex3f(prev_tr.x, prev_tr.y, prev_tr.z);
	glVertex3f(prev_br.x, prev_br.y, prev_br.z);
	glVertex3f(prev_bl.x, prev_bl.y, prev_bl.z);

	glEnd();
	glPopMatrix();
}