#include <GL/glew.h> // Order matters
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include "config.h"

GLFWwindow *window;

/// Instantiates the GLFW window
void init_window()
{
	glfwInit(); // Initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a window object
	window = glfwCreateWindow(960, 570, "StereoscopicTest", NULL, NULL);
	if (window == NULL) { // Handle error
		fprintf(stderr, "Could not create GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE; // Initialize GLEW and handle error
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Could not initialize GLEW\n");
		exit(EXIT_FAILURE);
	}

	int width, height; // Take dimensions from GLFW such that it also works on high DPI screens
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}


/// Keeps drawing and handling input until explicitly stopped
void game_loop()
{
	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);// Clear the color buffer
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}


int main(void)
{
	init_window();
	game_loop();
	printf("Test version %d.%d succesfull\n", SST_VERSION_MAJOR, SST_VERSION_MINOR);
	return EXIT_SUCCESS;
}
