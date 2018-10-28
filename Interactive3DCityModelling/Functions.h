#pragma once

namespace icm {

	// Initializes glut and OpenGL
	void initialize(int argc, char** argv) noexcept;

	void run() noexcept;

	// Resize callback function for glut
	void resize_func(int w, int h) noexcept;

	// Render callback function for glut
	void render_func() noexcept;

	// Idling callback function for glut
	void idle_func() noexcept;

	// Timer callback function for glut
	void timer_func(int value) noexcept;

	// Keyboard callback function for glut
	void keyboard(unsigned char key, int x, int y) noexcept;

	// Function key callback function for glut
	void function_keys(int key, int x, int y) noexcept;

}