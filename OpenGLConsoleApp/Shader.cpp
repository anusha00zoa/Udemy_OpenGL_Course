#include "Shader.h"

// CONSTRUCTOR
Shader::Shader() {
	shaderID = 0;
	uniformModel = 0;
	uniformView = 0;
	uniformProjection = 0;
  uniformEyePosition = 0;

  /*uniformDirectionalLight.uniformColor = 0;
  uniformDirectionalLight.uniformAmbientIntensity = 0;
  uniformDirectionalLight.uniformDirection = 0;
  uniformDirectionalLight.uniformDiffuseIntensity = 0;*/
  
  uniformSpecularIntensity = 0;
  uniformShininess = 0;

  pointLightCount = 0;
  uniformPointLightCount = 0;
}

// GETTER FUNCTIONS
GLuint Shader::GetProjectionLocation() { return uniformProjection; }
GLuint Shader::GetModelLocation() { return uniformModel; }
GLuint Shader::GetViewLocation() { return uniformView; }
GLuint Shader::GetEyePositionLocation() { return uniformEyePosition; }

GLuint Shader::GetAmbientColorLocation() { return uniformDirectionalLight.uniformColor; }
GLuint Shader::GetAmbientIntensityLocation() { return uniformDirectionalLight.uniformAmbientIntensity; }
GLuint Shader::GetDiffuseIntensityLocation() { return uniformDirectionalLight.uniformDiffuseIntensity; }
GLuint Shader::GetDirectionLocation() { return uniformDirectionalLight.uniformDirection; }

GLuint Shader::GetSpecularIntensityLocation() { return uniformSpecularIntensity; }
GLuint Shader::GetShininessLocation() { return uniformShininess;  }

void Shader::CreateFromString(const char *vertexCode, const char *fragmentCode) {
	CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char *fileLocation) {
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesnt exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::CreateFromFiles(const char *vertexLocation, const char *fragmentLocation) {
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);

	const char *vertexCode = vertexString.c_str();
	const char *fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}


void Shader::AddShader(GLuint program, const char* shaderCode, GLenum shaderType) {
	GLuint shdr = glCreateShader(shaderType);                                           	// CREATE SHADER, GIVEN SHADER TYPE

	const GLchar* code[1];
	code[0] = shaderCode;
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(shdr, 1, code, codeLength);                                        	// ASSIGN SHADER CODE SOURCE
	glCompileShader(shdr);                                                              // COMPILE SHADER

	GLint result = 0;                   
	GLchar eLog[1024] = { 0 };
	glGetShaderiv(shdr, GL_COMPILE_STATUS, &result);                                   	// GET COMPILE STATUS AND PRINT ERRORS IF ANY
	if (!result) {
		glGetProgramInfoLog(shdr, 1024, NULL, eLog);
		printf("Error compiling %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(program, shdr);                                                    	// ATTACH COMPILED SHADER TO THE GIVEN SHADER PROGRAM

	return;
}

void Shader::CompileShader(const char *vertexCode, const char *fragmentCode) {
	shaderID = glCreateProgram();                                                      	// CREATE SHADER PROGRAM
	if (!shaderID) {
		printf("Error creating shader program!");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);                                	// ADD VERTEX SHADER TO THE PROGRAM
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);                              // ADD FRAGMENT SHADER TO THE PROGRAM

	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shaderID);                                                           	// LINK SHADER PROGRAM
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);                                  // GET LINK STATUS AND PRINT ERRORS IF ANY
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shaderID);                                                       	// VALIDATE SHADER PROGRAM
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);                              // GET VALIDATION STATUS AND PRINT ERRORS IF ANY
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
  uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

  uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "dl.base.ambientIntensity");
  uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "dl.base.color");
  uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "dl.base.diffuseIntensity");
  uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "dl.direction");
  
  uniformSpecularIntensity = glGetUniformLocation(shaderID, "mat.specularIntensity");
  uniformShininess = glGetUniformLocation(shaderID, "mat.shininess");

  uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");
  for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {
    char locBuff[100] = { '\0' };

    snprintf(locBuff, sizeof(locBuff), "pl[%d].base.color", i);
    uniformPointLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);
    snprintf(locBuff, sizeof(locBuff), "pl[%d].base.ambientIntensity", i);
    uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);
    snprintf(locBuff, sizeof(locBuff), "pl[%d].base.diffuseIntensity", i);
    uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

    snprintf(locBuff, sizeof(locBuff), "pl[%d].position", i);
    uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);
    snprintf(locBuff, sizeof(locBuff), "pl[%d].constant", i);
    uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);
    snprintf(locBuff, sizeof(locBuff), "pl[%d].linear", i);
    uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);
    snprintf(locBuff, sizeof(locBuff), "pl[%d].exponent", i);
    uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
  }

  uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");
  for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++) {
    char locBuff[100] = { '\0' };

    snprintf(locBuff, sizeof(locBuff), "sl[%d].base.base.color", i);
    uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);
    snprintf(locBuff, sizeof(locBuff), "sl[%d].base.base.ambientIntensity", i);
    uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);
    snprintf(locBuff, sizeof(locBuff), "sl[%d].base.base.diffuseIntensity", i);
    uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

    snprintf(locBuff, sizeof(locBuff), "sl[%d].base.position", i);
    uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);
    snprintf(locBuff, sizeof(locBuff), "sl[%d].base.constant", i);
    uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);
    snprintf(locBuff, sizeof(locBuff), "sl[%d].base.linear", i);
    uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);
    snprintf(locBuff, sizeof(locBuff), "sl[%d].base.exponent", i);
    uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

    snprintf(locBuff, sizeof(locBuff), "sl[%d].direction", i);
    uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);
    snprintf(locBuff, sizeof(locBuff), "sl[%d].edge", i);
    uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
  }
}


void Shader::SetDirectionalLight(DirectionalLight *dLight) {
  dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor, 
                  uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight *pLight, unsigned int lightCount) {
  if (lightCount > MAX_POINT_LIGHTS) {
    lightCount = MAX_POINT_LIGHTS;
  }

  glUniform1i(uniformPointLightCount, lightCount);

  for (size_t i = 0; i < lightCount; i++) {
    pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor, uniformPointLight[i].uniformDiffuseIntensity,
                        uniformPointLight[i].uniformPosition, 
                        uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
  }
}

void Shader::SetSpotLights(SpotLight * sLight, unsigned int lightCount) {
  if (lightCount > MAX_SPOT_LIGHTS) {
    lightCount = MAX_SPOT_LIGHTS;
  }

  glUniform1i(uniformSpotLightCount, lightCount);

  for (size_t i = 0; i < lightCount; i++) {
    sLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColor, uniformSpotLight[i].uniformDiffuseIntensity,
      uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
      uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
      uniformSpotLight[i].uniformEdge);
  }
}

void Shader::UseShader() {
	glUseProgram(shaderID);
}

void Shader::ClearShader() {
	if (shaderID != 0) {
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
  uniformEyePosition = 0;

  /*uniformDirectionalLight.uniformColor = 0;
  uniformDirectionalLight.uniformAmbientIntensity = 0;
  uniformDirectionalLight.uniformDirection = 0;
  uniformDirectionalLight.uniformDiffuseIntensity = 0;*/
  
  uniformSpecularIntensity = 0;
  uniformShininess = 0;
  
  pointLightCount = 0;
  uniformPointLightCount = 0;
}

// DESTRUCTOR
Shader::~Shader() {
	ClearShader();
}
