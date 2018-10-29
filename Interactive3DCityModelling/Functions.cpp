#include "Globals.h"
#include "Constants.h"
#include "Functions.h"
#include "CubeMesh.h"
#include <string>
#include <sstream>
#include <iostream>
#include <gl/glew.h>
#include <gl/GLU.h>
#include <gl/freeglut.h>
#include <fstream>
#include <cctype>

void icm::initialize(int argc, char ** argv) noexcept
{

	// Initialize glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(icm::WINDOW_WIDTH, icm::WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	if (glutCreateWindow(icm::WINDOW_TITLE.c_str()) < 1) {
		std::cerr << "ERROR: Could not create a new rendering window" << std::endl;
		exit(EXIT_FAILURE);
	}
	glutReshapeFunc(resize_func);
	glutDisplayFunc(render_func);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(function_keys);
	glutTimerFunc(0, timer_func, 0);
	glutIdleFunc(idle_func);
	glutMotionFunc(mouse_motion_func);

	// Initialize OpenGL
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.53f, 0.81f, 0.98f, 0.0f);
	glClearDepth(1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Set up & enable lighting
	//float light_position0[] = { -6.0f, 12.0f, 0.0f, 1.0f };
	float light_position0[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	//float light_position1[] = { 6.0f, 12.0f, 0.0f, 1.0f };
	float light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Initialize the ground mesh
	icm::ground_mesh = NewQuadMesh(icm::MESH_SIZE);
	auto ground_mesh_org = NewVector3D(-icm::MESH_SIZE / 2, 0.0f, icm::MESH_SIZE / 2);
	auto right = NewVector3D(1.0f, 0.0f, 0.0f);
	auto front = NewVector3D(0.0f, 0.0f, -1.0f);
	InitMeshQM(&icm::ground_mesh, icm::MESH_SIZE, ground_mesh_org, icm::MESH_SIZE, icm::MESH_SIZE, right, front);

	// Set the lighting/shading for the ground mesh
	auto ground_mesh_ambient = NewVector3D(0.6f, 0.6f, 0.6f);
	auto ground_mesh_diffuse = NewVector3D(0.6f, 0.6f, 0.6f);
	auto ground_mesh_specular = NewVector3D(0.04f, 0.04f, 0.04f);
	SetMaterialQM(&icm::ground_mesh, ground_mesh_ambient, ground_mesh_diffuse, ground_mesh_specular, 0.2);
}

void icm::run() noexcept
{
	glutMainLoop();
}

void icm::resize_func(int w, int h) noexcept
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(icm::FOV, static_cast<double>(icm::WINDOW_WIDTH) / static_cast<double>(icm::WINDOW_HEIGHT), icm::near_plane, icm::far_plane);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camera_position.x, camera_position.y, camera_position.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void icm::render_func() noexcept
{
	icm::frame_count++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw the ground mesh
	DrawMeshQM(&icm::ground_mesh, icm::MESH_SIZE);

	if (icm::is_current_cube_active) {
		glPushMatrix();
		glTranslatef(icm::current_cube_position.x, icm::current_cube_position.y, icm::current_cube_position.z);
		glScalef(icm::cube_scale_factors.x, icm::cube_scale_factors.y, icm::cube_scale_factors.z);
		drawCube(&icm::current_cube);
		glPopMatrix();
	}

	for (auto extruded_mesh : icm::extruded_meshes) {
		extruded_mesh.draw();
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

void icm::idle_func() noexcept
{
	glutPostRedisplay();
}

void icm::timer_func(int value) noexcept
{
	if (value != 0) {
		std::stringstream ss;
		ss  << icm::WINDOW_TITLE
			<< ": "
			<< (icm::frame_count * 4)
			<< " FPS"
			<< " @ "
			<< icm::WINDOW_WIDTH
			<< " x "
			<< icm::WINDOW_HEIGHT;
		glutSetWindowTitle(ss.str().c_str());
	}
	icm::frame_count = 0;
	glutTimerFunc(250, timer_func, 1);
}

void icm::keyboard(unsigned char key, int x, int y) noexcept
{
	icm::scale_vertical = false;
	icm::scale_horizontal = false;
	icm::translate_cube = false;

	if (key == 't') {
		icm::translate_cube = icm::is_current_cube_active ? true : false;
	}

	else if (key == 'h') {
		icm::scale_vertical = icm::is_current_cube_active ? true : false;
	}

	else if (key == 's') {
		icm::scale_horizontal = icm::is_current_cube_active ? true : false;
	}

	// Save street
	if (key == 'e') {
		if (icm::is_current_cube_active && icm::is_street) {
			icm::extruded_meshes.push_back(icm::ExtrudedMesh(icm::street_mesh_ambient, icm::street_mesh_diffuse, icm::street_mesh_specular, icm::street_mesh_shininess, icm::BuildingType::NONE, true));
			icm::is_current_cube_active = false;
			icm::is_street = false;
			icm::translate_cube = false;
			icm::scale_vertical = false;
			icm::scale_horizontal = false;
			// Reset the cube starting position and scaling factors
			icm::current_cube_position = icm::cube_starting_pos;
			icm::cube_scale_factors = icm::cube_starting_scale_factors;
		}
	}

	if (key == 'i') {
		icm::FOV -= .25;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(icm::FOV,
			static_cast<double>(icm::WINDOW_WIDTH) / static_cast<double>(icm::WINDOW_HEIGHT),
			icm::near_plane, icm::far_plane);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		gluLookAt(camera_position.x, camera_position.y, camera_position.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

		glutPostRedisplay();

	}
	else if (key == 'k') {
		icm::FOV += .25;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(icm::FOV,
			static_cast<double>(icm::WINDOW_WIDTH) / static_cast<double>(icm::WINDOW_HEIGHT),
			icm::near_plane, icm::far_plane);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		gluLookAt(camera_position.x, camera_position.y, camera_position.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

		glutPostRedisplay();
	}

	if (key == 'z') {
		icm::clear_scene();
	}

	if (key == 'l') {
		load_from_file();
	}

	if (key == 'o') {
		save_to_file();
	}
}

void icm::function_keys(int key, int x, int y) noexcept
{

	// Create a cube mesh for a building
	if (key == GLUT_KEY_F1) {
		if (!icm::is_current_cube_active) {
			icm::is_current_cube_active = true;
			icm::is_street = false;
			icm::translate_cube = false;
			icm::scale_vertical = false;
			icm::scale_horizontal = false;
			icm::current_cube = newCube(icm::cube_ambient, icm::cube_diffuse, icm::cube_specular, icm::cube_shininess);
		}
	}
	
	// Create a cube mesh for a street
	if (key == GLUT_KEY_F2) {
		
		if (!icm::is_current_cube_active) {
			icm::is_current_cube_active = true;
			icm::is_street = true;
			icm::translate_cube = false;
			icm::scale_vertical = false;
			icm::scale_horizontal = false;
			icm::current_cube = newCube(icm::street_ambient, icm::street_diffuse, icm::street_specular, icm::street_shininess);
			icm::cube_scale_factors.y = 0.2f;
			icm::current_cube_position.y = 0.3f;
		}
	}

	// Create random building shape
	if (key == GLUT_KEY_F5) {
		if (icm::is_current_cube_active && !icm::is_street) {

			icm::extruded_meshes.push_back(icm::ExtrudedMesh(icm::building_ambient, icm::building_diffuse, icm::building_specular, icm::building_shininess, icm::BuildingType::RANDOM));

			icm::is_current_cube_active = false;
			icm::is_street = false;
			icm::translate_cube = false;
			icm::scale_vertical = false;
			icm::scale_horizontal = false;

			// Reset the cube starting position and scaling factors
			icm::current_cube_position = icm::cube_starting_pos;
			icm::cube_scale_factors = icm::cube_starting_scale_factors;
		}
	}

	// Create sine curved shape
	if (key == GLUT_KEY_F6) {
		if (icm::is_current_cube_active && !icm::is_street) {

			icm::extruded_meshes.push_back(icm::ExtrudedMesh(icm::building_ambient, icm::building_diffuse, icm::building_specular, icm::building_shininess, icm::BuildingType::SINE));
			
			icm::is_current_cube_active = false;
			icm::is_street = false;
			icm::translate_cube = false;
			icm::scale_vertical = false;
			icm::scale_horizontal = false;

			// Reset the cube starting position and scaling factors
			icm::current_cube_position = icm::cube_starting_pos;
			icm::cube_scale_factors = icm::cube_starting_scale_factors;
		}
	}

	// Create normal building shape
	if (key == GLUT_KEY_F7) {
		if (icm::is_current_cube_active && !icm::is_street) {

			icm::extruded_meshes.push_back(icm::ExtrudedMesh(icm::building_ambient, icm::building_diffuse, icm::building_specular, icm::building_shininess, icm::BuildingType::NONE));

			icm::is_current_cube_active = false;
			icm::is_street = false;
			icm::translate_cube = false;
			icm::scale_vertical = false;
			icm::scale_horizontal = false;

			// Reset the cube starting position and scaling factors
			icm::current_cube_position = icm::cube_starting_pos;
			icm::cube_scale_factors = icm::cube_starting_scale_factors;
		}
	}

	// Movement of the cube mesh

	// positive z axis movement
	if (key == GLUT_KEY_DOWN) {
		if (icm::is_current_cube_active) {
			if (icm::translate_cube)
				icm::current_cube_position.z += icm::translation_amount;
		}
	}
	// negative z axis movement
	else if (key == GLUT_KEY_UP) {
		if (icm::is_current_cube_active) {
			if (icm::translate_cube)
				icm::current_cube_position.z -= icm::translation_amount;
		}
	}

	// positive x axis movement and scaling
	if (key == GLUT_KEY_RIGHT) {
		if (icm::is_current_cube_active) {
			if (icm::translate_cube)
				icm::current_cube_position.x += icm::translation_amount;
			if (icm::scale_horizontal) {
				icm::cube_scale_factors.x += icm::scale_amount;
			}
		}
	}
	// negative x axis movement and scaling
	else if (key == GLUT_KEY_LEFT) {
		if (icm::is_current_cube_active) {
			if (icm::translate_cube)
				icm::current_cube_position.x -= icm::translation_amount;
			if (icm::scale_horizontal) {
				icm::cube_scale_factors.x -= icm::scale_amount;
			}
		}
	}

	// positive z axis scaling
	 if (key == GLUT_KEY_HOME) {
		if (icm::is_current_cube_active) {
			if (icm::scale_horizontal) {
				icm::cube_scale_factors.z += icm::scale_amount;
			}
		}
	}
	// negative z axis scaling
	else if (key == GLUT_KEY_END) {
		if (icm::is_current_cube_active) {
			if (icm::scale_horizontal) {
				icm::cube_scale_factors.z -= icm::scale_amount;
			}
		}
	}

	// Vertical scaling
	// positive y axis scaling
	if (key == GLUT_KEY_PAGE_UP) {
		if (icm::is_current_cube_active) {
			if (icm::scale_vertical && !icm::is_street) {
				icm::cube_scale_factors.y += icm::scale_amount;
				icm::current_cube_position.y += icm::scale_amount;
			}
		}
	}
	// negative y axis scaling
	else if (key == GLUT_KEY_INSERT) {
		if (icm::is_current_cube_active) {
			if (icm::scale_vertical && !icm::is_street) {
				icm::cube_scale_factors.y -= icm::scale_amount;
				icm::current_cube_position.y -= icm::scale_amount;
			}
		}
	}
}

void icm::mouse_func(int button, int state, int x, int y) noexcept
{
	
}

void icm::mouse_motion_func(int x, int y) noexcept
{
	
}

void icm::clear_scene() noexcept
{
	extruded_meshes.clear();
	is_current_cube_active = false;
	is_street = false;
	translate_cube = false;
	scale_vertical = false;
	scale_horizontal = false;
	current_cube_position = cube_starting_pos;
	cube_scale_factors = cube_starting_scale_factors;
}

void icm::save_to_file() noexcept
{
	if (extruded_meshes.empty())
		return;
	std::ofstream os("city_data.txt", std::ios::out | std::ios::trunc);
	std::cout << "Saving to file!" << std::endl;
	for (auto extruded_mesh : extruded_meshes) {
		if (extruded_mesh.is_street_mesh) {
			os << "street";
		}
		else {
			os << "building";
		}
		for (auto vertex : extruded_mesh.vertices) {
			os << " " << vertex.x << " " << vertex.y << " " << vertex.z;
		}
		os << std::endl;
	}
	os.close();
}

void icm::load_from_file() noexcept
{
	clear_scene();
	std::cout << "Reading from file!" << std::endl;
	std::ifstream in("city_data.txt");
	std::string line;
	while (std::getline(in, line)) {

		std::istringstream iss(line);

		std::string mesh_type;
		iss >> mesh_type;

		std::vector<Vector3D> vertices;
		if (mesh_type == "street") {
			Vector3D vertex;
			while (iss >> vertex.x >> vertex.y >> vertex.z) {
				vertices.push_back(vertex);
			}
			ExtrudedMesh em;
			em.ambient = icm::street_mesh_ambient;
			em.diffuse = icm::street_mesh_diffuse;
			em.specular = icm::street_mesh_specular;
			em.shininess = icm::street_mesh_shininess;
			em.vertices = vertices;
			em.is_street_mesh = true;
			icm::extruded_meshes.push_back(em);
		}
		else {
			Vector3D vertex;
			while (iss >> vertex.x >> vertex.y >> vertex.z) {
				vertices.push_back(vertex);
			}
			ExtrudedMesh em;
			em.ambient = icm::building_ambient;
			em.diffuse = icm::building_diffuse;
			em.specular = icm::building_specular;
			em.shininess = icm::building_shininess;
			em.vertices = vertices;
			em.is_street_mesh = false;
			icm::extruded_meshes.push_back(em);
		}

	}
	in.close();
}

