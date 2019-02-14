#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
  DirectionalLight();
  DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, 
                    GLfloat aIntensity, GLfloat dIntensity, 
                    GLfloat xDirection, GLfloat yDirection, GLfloat zDirection);

  void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, 
                GLuint diffuseIntensityLocation, GLuint directionLocation);
  
  ~DirectionalLight();

private:
  glm::vec3 direction;
};

