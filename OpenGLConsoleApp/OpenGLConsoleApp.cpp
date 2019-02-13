// OpenGLConsoleApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
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
#include "Camera.h"
#include "Texture.h"
#include "Light.h"

const float degreesToRadians = 3.141529265f / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

MyWindow mainWindow;

Camera camera;

Texture brickTexture, dirtTexture;

Light mainLight;

GLfloat deltaTime = 0.0f, lastTime = 0.0f;

static const char* vertexShader = "Shaders/vs.vert";                                                            // VERTEX SHADER
static const char* fragmentShader = "Shaders/fs.frag";                                                          // FRAGMENT SHADER

void CreateShaders() {
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vertexShader, fragmentShader);
	shaderList.push_back(shader1);
}

void CreateObjects() {
	// // DEFINE VERTICES FOR OBJECT TO BE DRAWN
	
	// USE THESE VERTICES TO DRAW A FULL SPECTRUM COLORED TRIANGLE
	/*GLfloat vertices[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};*/ 

	GLfloat vertices[] = {
    // x, y, z - VERTEX COORDINATES, u, v -  TEXTURE COORDINATES
    // LINE FORMAT: x, y, z, u, v 
		-1.0f,  -1.0f,  0.0f,     0.0f, 0.0f,		// 0
		0.0f,   -1.0f,  1.0f,     0.5f, 0.0f,		// 1
		1.0f,   -1.0f,  0.0f,     1.0f, 0.0f,		// 2
		0.0f,   1.0f,   0.0f,     0.5f, 1.0f	  // 3
	};

	unsigned int indices[] = {                                                                                  	// FOR INDEXED DRAW
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(obj2);
}

int main() {
	mainWindow = MyWindow(800, 600);                                                                        	    // CREATE WINDOW
	mainWindow.Initialize();

	CreateObjects();                                                                                          	  // CALL FUNCTION TO CREATE OBJECT DATA
	
	CreateShaders();                                                                                            	// ADD AND COMPILE SHADERS AND SHADER PROGRAM

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);        	// INITIALIZE CAMERA

  brickTexture = Texture((char*)"Textures/brick.png");                                                          // INITIALIZE TEXTURES - PASS TEXTURE LOCATIONS AND LOAD TEXTURES
  brickTexture.LoadTexture();
  dirtTexture = Texture((char*)"Textures/dirt.png");
  dirtTexture.LoadTexture();

  mainLight = Light(1.0f, 1.0f, 1.0f, 1.0f);                                                                    // INITIALIZE AMBIENT LIGHT

	glm::mat4 projectionMatrix = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);    	// CALCULATE PROJECTION MATRIX ONCE AND REUSE IN THE APPLICATION

	GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0, uniformAmbientIntensity = 0, uniformAmbientColor = 0;
	
	while (!mainWindow.GetShouldClose()) {                                                                         // LOOP UNTIL WINDOW CLOSES
		GLfloat now = glfwGetTime();                                                                                 // CALCULATE DELTA TIME
		deltaTime = now - lastTime;
		lastTime = now;

		glfwPollEvents();                                                                                       		// GET AND HANDLE USER INPUT EVENTS

		camera.KeyControl(mainWindow.getKeys(), deltaTime);
		camera.MouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                                                                    		// CLEAR WINDOW
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0]->UseShader();                                                                             		// USE SHADER PROGRAM
		uniformModel = shaderList[0]->GetModelLocation();                                                           // GET UNIFORMS LOCATIONS
		uniformProjection = shaderList[0]->GetProjectionLocation();
		uniformView = shaderList[0]->GetViewLocation();
    uniformAmbientIntensity = shaderList[0]->GetAmbientIntensityLocation();
    uniformAmbientColor = shaderList[0]->GetAmbientColorLocation();

    mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColor);                                           // USE AMBIENT LIGHT

		// // BEGIN OBJECT 1 SECTION
		glm::mat4 modelMatrix = glm::mat4(1.0f);                                                                		// INITIALIZE MODEL MATRIX AS IDENTITY AND TRANSFORM
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -2.5f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.3f, 1.0f));
		// // DEBUG PRINT glm::mat4
		//std::cout << glm::to_string(modelMatrix) << std::endl;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
		brickTexture.UseTexture();
		meshList[0]->RenderMesh();
		// // END OBJECT 1 SECTION
		
		// // BEGIN OBJECT 2 SECTION
		modelMatrix = glm::mat4(1.0f);                                                                              // INITIALIZE MODEL MATRIX AS IDENTITY AND TRANSFORM
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 1.0f, -2.5f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    dirtTexture.UseTexture();
		meshList[1]->RenderMesh();
		// // END OBJECT 2 SECTION

		glUseProgram(0);                                                                                            // DETACH SHADER PROGRAM

		mainWindow.SwapBuffers();                                                                               		// SWAP FRAMEBUFFER TO BE DRAWN
	}

	return 0;
}