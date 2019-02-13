#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader
{
public:
	Shader();

	void CreateFromString(const char *vertexCode, const char *fragmentCode);
	void CreateFromFiles(const char *vertexLocation, const char *fragmentLocation);
	std::string ReadFile(const char *fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
  GLuint GetEyePositionLocation();

  GLuint GetAmbientIntensityLocation();
  GLuint GetAmbientColorLocation();
  GLuint GetDiffuseIntensityLocation();
  GLuint GetDirectionLocation();
  GLuint GetSpecularIntensityLocation();
  GLuint GetShininessLocation();

	void UseShader();
	void ClearShader();

	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition,
          uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection, uniformSpecularIntensity, uniformShininess;

	void AddShader(GLuint program, const char* shaderCode, GLenum shaderType);
	void CompileShader(const char *vertexCode, const char *fragmentCode);
};

