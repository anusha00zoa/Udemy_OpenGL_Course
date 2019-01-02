// OpenGLConsoleApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <cmath>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "MyWindow.h"

const float degreesToRadians = 3.141529265f / 180.0f;

MyWindow mainWindow;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

bool	direction = true,
		sizeDirection = true;

float	triOffset = 0.0f,
		triMaxOffset = 0.7f,
		triIncrement = 0.005f,
		curAngle = 0.0f,
		curSize = 0.3f,
		maxSize = 1.0f,
		minSize = 0.1f;


// VERTEX SHADER
static const char* vertexShader = "Shaders/vs.vert";

// FRAGMENT SHADER
static const char* fragmentShader = "Shaders/fs.frag";

void CreateShaders() {
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vertexShader, fragmentShader);
	shaderList.push_back(shader1);
}

void CreateObjects() {
	// DEFINE VERTICES FOR OBJECT TO BE DRAWN
	
	// USE THESE VERTICES TO DRAW A FULL SPECTRUM COLORED TRIANGLE
	/*GLfloat vertices[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};*/ 

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,		// 0
		0.0f, -1.0f, 1.0f,		// 1
		1.0f, -1.0f, 0.0f,		// 2
		0.0f, 1.0f, 0.0f		// 3
	};

	// FOR INDEXED DRAW
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2);
}

int main() {
	// CREATE WINDOW
	mainWindow = MyWindow(800, 600);
	mainWindow.Initialize();

	// CALL FUNCTION TO CREATE OBJECT DATA
	CreateObjects();
	
	// ADD AND COMPILE SHADERS AND SHADER PROGRAM
	CreateShaders();

	// CALCULATE PROJECTION MATRIX ONCE AND REUSE IN THE APPLICATION
	glm::mat4 projectionMatrix = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	GLuint uniformModel = 0, uniformProjection = 0;
	// LOOP UNTIL WINDOW CLOSES
	while (!mainWindow.GetShouldClose()) {
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// USE SHADER PROGRAM
		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();

		// BEGIN OBJECT 1 SECTION
		// INITIALIZE IDENTITY MATRIX
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		// APPLY TRANSLATION
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -5.0f));
		// APPLY ROTATION
		modelMatrix = glm::rotate(modelMatrix, curAngle * degreesToRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//// APPLY TRANSLATION
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(triOffset, 0.0f, 0.0f));
		// APPLY SCALING
		modelMatrix = glm::scale(modelMatrix, glm::vec3(curSize, curSize, 1.0f));
		
		//// DEBUG PRINT glm::mat4
		//std::cout << glm::to_string(modelMatrix) << std::endl;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		meshList[0]->RenderMesh();
		// END OBJECT 1 SECTION
		
		// BEGIN OBJECT 2 SECTION
		// INITIALIZE IDENTITY MATRIX
		modelMatrix = glm::mat4(1.0f);
		// APPLY TRANSLATION
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 1.0f, -5.0f));
		// APPLY ROTATION
		modelMatrix = glm::rotate(modelMatrix, curAngle * degreesToRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		// APPLY SCALING
		modelMatrix = glm::scale(modelMatrix, glm::vec3(curSize, curSize, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		meshList[1]->RenderMesh();
		// END OBJECT 2 SECTION

		glUseProgram(0);

		// SWAP FRAMEBUFFER TO BE DRAWN
		mainWindow.SwapBuffers();
	}

	return 0;
}