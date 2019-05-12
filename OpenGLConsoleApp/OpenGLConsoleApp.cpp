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
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "MyModel.h"
#include "CommonValues.h"


const float degreesToRadians = 3.141529265f / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

MyWindow mainWindow;

Camera camera;

Texture brickTexture, dirtTexture, floorTexture;

MyModel pottedPlant;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Material shinyMat, dullMat;

GLfloat deltaTime = 0.0f, lastTime = 0.0f;

// VERTEX SHADER
static const char* vertexShader = "Shaders/vs.vert";
// FRAGMENT SHADER
static const char* fragmentShader = "Shaders/fs.frag";                          


void CalculateAverageNormals(unsigned int* indices, unsigned int indicesCount, GLfloat* vertices, unsigned int verticesCount, unsigned int vertexLength, unsigned int normalOffset) {
  for (size_t i = 0; i < indicesCount; i += 3) {
    unsigned int index0 = indices[i] * vertexLength;
    unsigned int index1 = indices[i + 1] * vertexLength;
    unsigned int index2 = indices[i + 2] * vertexLength;

    glm::vec3 v1(vertices[index1] - vertices[index0], vertices[index1 + 1] - vertices[index0 + 1], vertices[index1 + 2] - vertices[index0 + 2]);
    glm::vec3 v2(vertices[index2] - vertices[index0], vertices[index2 + 1] - vertices[index0 + 1], vertices[index2 + 2] - vertices[index0 + 2]);

    glm::vec3 normal = glm::cross(v1, v2);
    normal = glm::normalize(normal);

    index0 += normalOffset;
    vertices[index0] += normal.x;
    vertices[index0 + 1] += normal.y;
    vertices[index0 + 2] += normal.z;

    index1 += normalOffset;
    vertices[index1] += normal.x;
    vertices[index1 + 1] += normal.y;
    vertices[index1 + 2] += normal.z;

    index2 += normalOffset;
    vertices[index2] += normal.x;
    vertices[index2 + 1] += normal.y;
    vertices[index2 + 2] += normal.z;
  }

  for (size_t i = 0; i < verticesCount / vertexLength; i++) {
    unsigned int nOffset = i * vertexLength + normalOffset;
    glm::vec3 v(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
    v = glm::normalize(v);

    vertices[nOffset] = v.x;
    vertices[nOffset + 1] = v.y;
    vertices[nOffset + 2] = v.z;
  }
}

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
    // LINE FORMAT: x, y, z, u, v, normalX, normalY, normalZ
    -1.0f,  -1.0f,  0.0f,     0.0f, 0.0f,   0.0f, 0.0f, 0.0f,	  // 0
    0.0f,   -1.0f,  1.0f,     0.5f, 0.0f,   0.0f, 0.0f, 0.0f,		// 1
    1.0f,   -1.0f,  0.0f,     1.0f, 0.0f,   0.0f, 0.0f, 0.0f,		// 2
    0.0f,   1.0f,   0.0f,     0.5f, 1.0f,   0.0f, 0.0f, 0.0f	  // 3
  };

  // FOR INDEXED DRAW
  unsigned int indices[] = {                                                
    0, 3, 1,
    1, 3, 2,
    2, 3, 0,
    0, 1, 2
  };

  // CREATING FLOOR FOR THE SCENE
  GLfloat floorVertices[] = {
    -10.0f, 0.0f, -10.0f,   0.0f, 0.0f,     0.0f, -1.0f, 0.0f,
    10.0f, 0.0f, -10.0f,    10.0f, 0.0f,    0.0f, -1.0f, 0.0f,
    -10.0f, 0.0f, 10.0f,    0.0f, 10.0f,    0.0f, -1.0f, 0.0f,
    10.0f, 0.0f, 10.0f,     10.0f, 10.0f,   0.0f, -1.0f, 0.0f
  };

  unsigned int floorIndices[] = {
    0, 2, 1,
    1, 2, 3
  };

  CalculateAverageNormals(indices, 12, vertices, 32, 8, 5);

  Mesh *obj1 = new Mesh();
  obj1->CreateMesh(vertices, indices, 32, 12);
  meshList.push_back(obj1);

  Mesh *obj2 = new Mesh();
  obj2->CreateMesh(vertices, indices, 32, 12);
  meshList.push_back(obj2);

  Mesh *obj3 = new Mesh();
  obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
  meshList.push_back(obj3);
}

int main() {
  // CREATE WINDOW
  mainWindow = MyWindow(1366, 768);                                                                        	    
  mainWindow.Initialize();

  // CALL FUNCTION TO CREATE OBJECT DATA
  CreateObjects(); 
  // ADD AND COMPILE SHADERS AND SHADER PROGRAM
  CreateShaders();                                                                                            	

  // INITIALIZE CAMERA
  camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);        	

  // INITIALIZE TEXTURES - PASS TEXTURE LOCATIONS AND LOAD TEXTURES
  brickTexture = Texture((char*)"Textures/brick.png");                                                          
  brickTexture.LoadTextureWithAlpha();
  dirtTexture = Texture((char*)"Textures/dirt.png");
  dirtTexture.LoadTextureWithAlpha();
  floorTexture = Texture((char*)"Textures/plain.png");
  floorTexture.LoadTextureWithAlpha();

  // INITIALIZE DIRECTIONAL LIGHT
  mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, // r, g, b
                                1.0f, 0.0f, // ambient, diffuse intensities
                                0.0f, -1.0f, 0.0f); // xDir, yDir, zDir                                         

  // INITIALIZE POINT LIGHTS
  unsigned int pointLightCount = 0;
  pointLights[0] = PointLight(1.0f, 0.0f, 0.0f, 
                              1.0f, 0.1f, 
                              -4.0f, 2.0f, 0.0f, 
                              0.3f, 0.1f, 0.1f);                                                                
  pointLightCount++;
  pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
                              1.0f, 0.1f,
                              4.0f, 2.0f, 0.0f,
                              0.3f, 0.2f, 0.1f);
  pointLightCount++;

  // INITIALIZE SPOT LIGHTS
  unsigned int spotLightCount = 0;
  spotLights[0] = SpotLight(0.0f, 0.0f, 1.0f,
                            0.0f, 1.0f,
                            0.0f, 0.0f, 0.0f,
                            0.0f,-1.0f, 0.0f,
                            0.3f, 0.2f, 0.1f,
                            20.0f);                                                                           
  spotLightCount++;
  
  // INITIALIZE MATERIALS
  shinyMat = Material(1.0f, 32);
  dullMat = Material(0.3f, 4);

  // INITIALIZE MODELS
  pottedPlant = MyModel();
  pottedPlant.LoadModel("Models/indoor plant_02.obj");

  // CALCULATE PROJECTION MATRIX ONCE AND REUSE IN THE APPLICATION
  glm::mat4 projectionMatrix = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);    	

  GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
  
  // LOOP UNTIL WINDOW CLOSES
  while (!mainWindow.GetShouldClose()) {
    // CALCULATE DELTA TIME
    GLfloat now = glfwGetTime();                                                                                 
    deltaTime = now - lastTime;
    lastTime = now;

    // GET AND HANDLE USER INPUT EVENTS
    glfwPollEvents();                                                                                       		

    camera.KeyControl(mainWindow.getKeys(), deltaTime);
    camera.MouseControl(mainWindow.getXChange(), mainWindow.getYChange());

    // CLEAR WINDOW
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                                                                    		
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // USE SHADER PROGRAM
    shaderList[0]->UseShader();
    // GET UNIFORMS' LOCATIONS
    uniformModel = shaderList[0]->GetModelLocation();                                                           
    uniformProjection = shaderList[0]->GetProjectionLocation();
    uniformView = shaderList[0]->GetViewLocation();
    uniformEyePosition = shaderList[0]->GetEyePositionLocation();
    uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
    uniformShininess = shaderList[0]->GetShininessLocation();

    // CREATE FLASHLIGHT FROM CAMERA'S PERSPECTIVE
    glm::vec3 lowerLight = camera.GetCameraPosition();                                                          
    lowerLight.y -= 0.3f;
    spotLights[0].SetFlash(lowerLight, camera.GetCameraDirection());                                            

    // USE DIRECTIONAL LIGHT
    //mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection);    
    shaderList[0]->SetDirectionalLight(&mainLight);
    shaderList[0]->SetPointLights(pointLights, pointLightCount);
    shaderList[0]->SetSpotLights(spotLights, spotLightCount);

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
    glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

    // // BEGIN OBJECT 1 SECTION
    // INITIALIZE MODEL MATRIX AS IDENTITY AND TRANSFORM
    glm::mat4 modelMatrix = glm::mat4(1.0f);                                                                		
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -2.5f));
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.3f, 1.0f));
    // // DEBUG PRINT glm::mat4
    //std::cout << glm::to_string(modelMatrix) << std::endl;
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    brickTexture.UseTexture();
    shinyMat.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[0]->RenderMesh();
    // // END OBJECT 1 SECTION
    
    // // BEGIN OBJECT 2 SECTION
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 4.0f, -2.5f));
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.3f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    dirtTexture.UseTexture();
    dullMat.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[1]->RenderMesh();
    // // END OBJECT 2 SECTION

    // // BEGIN OBJECT 3 SECTION
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -2.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    floorTexture.UseTexture();
    shinyMat.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[2]->RenderMesh();
    // // END OBJECT 3 SECTION

    // // BEGIN OBJECT 4 SECTION
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -2.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.001f, 0.001f, 0.001f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    shinyMat.UseMaterial(uniformSpecularIntensity, uniformShininess);
    pottedPlant.RenderModel();
    // // END OBJECT 4 SECTION

    // DETACH SHADER PROGRAM
    glUseProgram(0);
    // SWAP FRAMEBUFFER TO BE DRAWN
    mainWindow.SwapBuffers();                                                                               		
  }

  return 0;
}