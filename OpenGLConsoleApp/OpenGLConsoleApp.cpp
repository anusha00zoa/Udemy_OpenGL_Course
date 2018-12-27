// OpenGLConsoleApp.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// WINDOW DIMENSIONS
const GLint WIDTH = 800,	// WINDOW WIDTH
			HEIGHT = 600;	// WINDOW HEIGHT

GLuint	vao,				// VERTEX ARRAY OBJECT
		vbo,				// VERTEX BUFFER OBJECT
		shaderProgram,		// SHADER PROGRAM
		uniformXMove;

bool	direction = true;

float	triOffset = 0.0f,
		triMaxOffset = 0.7f,
		triIncrement = 0.0005f;


// VERTEX SHADER
static const char* vertexShader = "								\n\
#version 330													\n\
																\n\
layout (location = 0) in vec3 pos;								\n\
																\n\
uniform float xMove;											\n\
																\n\
void main() {													\n\
	gl_Position = vec4(0.1 * pos.x + xMove, 0.1 * pos.y, pos.z, 1.0);		\n\
}";

// FRAGMENT SHADER
static const char* fragmentShader = "	\n\
#version 330							\n\
										\n\
out vec4 color;							\n\
										\n\
void main() {							\n\
	color = vec4(1.0, 0.0, 0.0, 1.0);	\n\
}";

void AddShader(GLuint program, const char* shaderCode, GLenum shaderType) {
	// CREATE SHADER, GIVEN SHADER TYPE
	GLuint shdr = glCreateShader(shaderType);

	const GLchar* code[1];
	code[0] = shaderCode;
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	// ASSIGN SHADER CODE SOURCE AND COMPILE SHADER
	glShaderSource(shdr, 1, code, codeLength);
	glCompileShader(shdr);

	// GET COMPILE STATUS AND PRINT ERRORS IF ANY
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glGetShaderiv(shdr, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shdr, 1024, NULL, eLog);
		printf("Error compiling %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	// ATTACH COMPILED SHADER TO THE GIVEN SHADER PROGRAM
	glAttachShader(program, shdr);

	return;
};

void CompileShaders() {
	// CREATE SHADER PROGRAM
	shaderProgram = glCreateProgram();
	if (!shaderProgram) {
		printf("Error creating shader program!");
		return;
	}

	// ADD VERTEX SHADER AND FRAGMENT SHADER TO THE PROGRAM
	AddShader(shaderProgram, vertexShader, GL_VERTEX_SHADER);
	AddShader(shaderProgram, fragmentShader, GL_FRAGMENT_SHADER);

	// LINK SHADER PROGRAM, GET LINK STATUS AND PRINT ERRORS IF ANY
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	// VALIDATE SHADER PROGRAM, GET VALIDATION STATUS AND PRINT ERRORS IF ANY
	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	uniformXMove = glGetUniformLocation(shaderProgram, "xMove");
}

void CreateTriangle() {
	// DEFINE VERTICES FOR OBJECT TO BE DRAWN
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	// GENERATE AND BIND VERTEX ARRAY OBJECT
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// GENERATE AND BIND VERTEX BUFFER OBJECT
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// DEFINE BUFFER DATA
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// DEFINE ATTRIBUTE POINTER
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	// UNBIND VERTEX ARRAY OBJECT AND VERTEX BUFFER OBJECT 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

int main() {
    // INITIALIZE GLFW
	if (!glfwInit()) {
		printf("GLFW Initialization failed!");
		glfwTerminate();
		return 1;
	}

	// // SETUP GLFW WINDOW PROPERTIES
	// OPENGL VERSION
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// CORE PROFILE = NO BACKWARDS COMPATIBILITY
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// ALLOW FORWARD COMPATIBILITY
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "OpenGLConsoleAppWindow", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// GET BUFFER SIZE INFORMATION
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// SET CONTEXT FOR GLEW TO USE
	glfwMakeContextCurrent(mainWindow);

	// ALLOW MODERN EXTENSION FEATURES
	glewExperimental = GL_TRUE;

	// INITIALIZE GLEW
	if (glewInit() != GLEW_OK) {
		printf("GLEW Initialization failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// SETUP VIEWPORT SIZE
	glViewport(0, 0, bufferWidth, bufferHeight);

	// CALL FUNCTION TO CREATE TRIANGLE DATA
	CreateTriangle();
	// ADD AND COMPILE SHADERS AND SHADER PROGRAM
	CompileShaders();

	// LOOP UNTIL WINDOW CLOSES
	while (!glfwWindowShouldClose(mainWindow)) {
		// GET AND HANDLE USER INPUT EVENTS
		glfwPollEvents();

		if (direction) {
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset) {
			direction = !direction;
		}

		// CLEAR WINDOW
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// USE SHADER PROGRAM
		glUseProgram(shaderProgram);

		glUniform1f(uniformXMove, triOffset);

		glBindVertexArray(vao);
		// CALL THE DRAW FUNCTION
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// UNBIND 
		glBindVertexArray(0);
		glUseProgram(0);

		// SWAP FRAMEBUFFER TO BE DRAWN
		glfwSwapBuffers(mainWindow);
	}

	return 0;
}