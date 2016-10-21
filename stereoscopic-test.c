#include <GL/glew.h> // Order matters
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "config.h"


/// Loads a shader source
GLchar *load_shader_source(const char *filePath)
{
	FILE *file = fopen(filePath, "r");
	if (file == NULL) { // Handle error
		fprintf(stderr, "Could not open shader file: %s\n", filePath);
	}
	// Calculate the length
	fseek(file, 0L, SEEK_END);
	size_t length = ftell(file);
	rewind(file);
	// Read the file
	GLchar *source = malloc(length);
	size_t nread = fread(source, 1, length, file);
	if (ferror(file)) {
		fprintf(stderr, "Could not read shader file: %s\n", filePath);
	}
	source[length - 1] = 0;
	fclose(file);
	return source;
}


/// Link compiled shaders to a shader program object
GLuint link_program(const GLuint vertexShader, const GLuint fragmentShader)
{
	GLuint program = glCreateProgram(); // Create a program object
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	GLint success; // Handle error
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		fprintf(stderr, "Error linking shader program: %s\n", infoLog);
		glDeleteProgram(program);
	}
	glDetachShader(program, vertexShader); // Detach shader objects
	glDetachShader(program, fragmentShader);
	return program;
}


/// Dynamically compile a shader
GLuint compile_shader(const GLenum type, const GLchar *source)
{
	GLuint shader = glCreateShader(type); // Create a shader object
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint success; // Handle error
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		fprintf(stderr, "Error compiling %s shader: %s\n",
			type == GL_VERTEX_SHADER ? "vertex" : "fragment", infoLog);
		glDeleteShader(shader);
		return EXIT_FAILURE;
	}
	return shader;
}


/// Loads shader sources, compile shaders and link them into a program
bool create_programs(GLuint *baseProgram, GLuint *depthProgram) {
	GLchar *vertexSource = load_shader_source("shader/base.vert");
	GLuint vertexShader = compile_shader(GL_VERTEX_SHADER, vertexSource);
	GLchar *fragmentSource = load_shader_source("shader/base.frag");
	GLuint fragmentShader = compile_shader(GL_FRAGMENT_SHADER, fragmentSource);
	*baseProgram = link_program(vertexShader, fragmentShader);

	GLchar *depthSource = load_shader_source("shader/depth.frag");
	GLuint depthShader = compile_shader(GL_FRAGMENT_SHADER, depthSource);
	*depthProgram = link_program(vertexShader, depthShader);

	glDeleteShader(vertexShader); // Delete shader objects
	glDeleteShader(fragmentShader);
	glDeleteShader(depthShader);
	free(vertexSource); // Free shader sources
	free(fragmentSource);

	return vertexShader != 0 && fragmentShader != 0 && baseProgram != 0 && depthProgram != 0;
}


/// Returns a vertex array object of a triangle
GLuint create_triangle()
{
	GLfloat vertices[] = {
		// Positions        // Colors
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, // Bottom left
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom right
		 0.0f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f // Top
	};
	GLuint vbo; // Generate a vertex buffer object
	glGenBuffers(1, &vbo);
	GLuint vao; // Create a vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// Copy the vertices in the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
	// Specify the vertex shader input in the form of vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	return vao;
}


/// Instantiates the GLFW window
GLFWwindow *init_window()
{
	glfwInit(); // Initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a window object
	GLFWwindow *window = glfwCreateWindow(960 * 2, 570, "StereoscopicTest", NULL, NULL);
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

	return window;
}


/// Keeps drawing and handling input until explicitly stopped
void game_loop(GLFWwindow *window, const GLuint baseProgram, const GLuint depthProgram, const GLuint triangle)
{
	int width, height; // Take dimensions from GLFW such that it also works on high DPI screens

	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);// Clear the color buffer

		glBindVertexArray(triangle);
		
		glViewport(0, 0, width / 2, height);
		glUseProgram(baseProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3); // Draw triangle
		
		glViewport(width / 2, 0, width / 2, height);
		glUseProgram(depthProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3); // Draw triangle depth
		
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
}


int main(void)
{
	GLFWwindow *window = init_window();
	GLuint triangle = create_triangle();

	GLuint baseProgram;
	GLuint depthProgram;
	if (create_programs(&baseProgram, &depthProgram)) {
		game_loop(window, baseProgram, depthProgram, triangle);
	}

	glfwTerminate();
	printf("Test version %d.%d succesfull\n", SST_VERSION_MAJOR, SST_VERSION_MINOR);
	return EXIT_SUCCESS;
}
