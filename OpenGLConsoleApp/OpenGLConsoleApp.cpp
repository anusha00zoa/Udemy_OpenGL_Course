// OpenGLConsoleApp.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

// WINDOW DIMENSIONS
const GLint WIDTH = 800,	// WINDOW WIDTH
			HEIGHT = 600;	// WINDOW HEIGHT

GLuint	vao,				// VERTEX ARRAY OBJECT
		vbo,				// VERTEX BUFFER OBJECT
		shaderProgram,		// SHADER PROGRAM
		uniformModel;

bool	direction = true,
		sizeDirection = true;

float	triOffset = 0.0f,
		triMaxOffset = 0.7f,
		triIncrement = 0.005f,
		curAngle = 0.0f,
		curSize = 0.3f,
		maxSize = 1.0f,
		minSize = 0.1f;

const float degreesToRadians = 3.141529265f / 180.0f;


// VERTEX SHADER
static const char* vertexShader = "								\n\
#version 330													\n\
																\n\
layout (location = 0) in vec3 pos;								\n\
																\n\
uniform mat4 model;												\n\
																\n\
void main() {													\n\
	gl_Position = model * vec4(pos, 1.0);						\n\
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

	uniformModel = glGetUniformLocation(shaderProgram, "model");
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

		// TRANSLATION
		if (direction) {
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset) {
			direction = !direction;
		}

		// ROTATION
		curAngle += 0.1f;
		if (curAngle >= 360) {
			curAngle -= 360;
		}

		// SCALING
		if (sizeDirection) {
			curSize += 0.001f;
		}
		else {
			curSize -= 0.001f;
		}

		if (curSize >= maxSize || curSize <= minSize) {
			sizeDirection = !sizeDirection;
		}

		// CLEAR WINDOW
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// USE SHADER PROGRAM
		glUseProgram(shaderProgram);

		// INITIALIZE IDENTITY MATRIX
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		// APPLY ROTATION
		modelMatrix = glm::rotate(modelMatrix, curAngle * degreesToRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		// APPLY TRANSLATION
		modelMatrix = glm::translate(modelMatrix, glm::vec3(triOffset, 0.0f, 0.0f));
		// APPLY SCALING
		modelMatrix = glm::scale(modelMatrix, glm::vec3(curSize, curSize, 1.0f));
		
		//// DEBUG PRINT glm::mat4
		//std::cout << glm::to_string(modelMatrix) << std::endl;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));

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