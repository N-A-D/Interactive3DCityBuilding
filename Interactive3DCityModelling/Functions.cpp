#include "Globals.h"
#include "Constants.h"
#include "Functions.h"
#include <string>
#include <sstream>
#include <iostream>
#include <gl/glew.h>
#include <gl/GLU.h>
#include <gl/freeglut.h>

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

	// Initialize OpenGL
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
	glClearDepth(1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Set up & enable lighting
	//float light_position0[] = { -6.0f, 12.0f, 0.0f, 1.0f };
	float light_position0[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	//float light_position1[] = { 6.0f, 12.0f, 0.0f, 1.0f };
	float light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
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
	auto ground_mesh_org = NewVector3D(-16.0f, 0.0f, 16.0f);
	auto right = NewVector3D(1.0f, 0.0f, 0.0f);
	auto front = NewVector3D(0.0f, 0.0f, -1.0f);
	InitMeshQM(&icm::ground_mesh, icm::MESH_SIZE, ground_mesh_org, icm::MESH_SIZE, icm::MESH_SIZE, right, front);

	// Set the lighting/shading for the ground mesh
	auto ground_mesh_ambient = NewVector3D(0.87f, 0.72f, 0.53f);
	auto ground_mesh_diffuse = NewVector3D(0.87f, 0.72f, 0.53f);
	auto ground_mesh_specular = NewVector3D(0.04f, 0.04f, 0.04f);
	SetMaterialQM(&icm::ground_mesh, ground_mesh_ambient, ground_mesh_diffuse, ground_mesh_specular, 0.2);

	// Initialize the street mesh
	icm::street_mesh = NewQuadMesh(icm::MESH_SIZE);
	Vector3D street_mesh_org = { ground_mesh_org.x, ground_mesh_org.y + street_mesh_offset_from_ground_mesh, ground_mesh_org.z };
	InitMeshQM(&icm::street_mesh, icm::MESH_SIZE, street_mesh_org, icm::MESH_SIZE, icm::MESH_SIZE, right, front);

	// Set the lighting/shading for the street mesh
	auto street_mesh_ambient = NewVector3D(0.35f, 0.35f, 0.35f);
	auto street_mesh_diffuse = NewVector3D(0.35f, 0.35f, 0.35f);
	auto street_mesh_specular = NewVector3D(0.04f, 0.04f, 0.04f);
	SetMaterialQM(&icm::street_mesh, street_mesh_ambient, street_mesh_diffuse, street_mesh_specular, 0.15);
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
	gluPerspective(45.0, static_cast<double>(icm::WINDOW_WIDTH) / static_cast<double>(icm::WINDOW_HEIGHT), 0.2, 80.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 15.0, 40, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void icm::render_func() noexcept
{
	icm::frame_count++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Draw the ground mesh
	DrawMeshQM(&icm::ground_mesh, icm::MESH_SIZE);

	// Draw the street mesh
	DrawMeshQM(&icm::street_mesh, icm::MESH_SIZE);

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
	// TO DO keyboard functionality
}

void icm::function_keys(int key, int x, int y) noexcept
{
	// TO DO function key functionality
}
