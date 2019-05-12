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

GLuint Shader::GetAmbientColorLocation() { return uniformDirectionalLight.baseLight.uniformColor; }
GLuint Shader::GetAmbientIntensityLocation() { return uniformDirectionalLight.baseLight.uniformAmbientIntensity; }
GLuint Shader::GetDiffuseIntensityLocation() { return uniformDirectionalLight.baseLight.uniformDiffuseIntensity; }
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
  // CREATE SHADER, GIVEN SHADER TYPE
  GLuint shdr = glCreateShader(shaderType);                                           	

  const GLchar* code[1];
  code[0] = shaderCode;
  GLint codeLength[1];
  codeLength[0] = strlen(shaderCode);

  // ASSIGN SHADER CODE SOURCE
  glShaderSource(shdr, 1, code, codeLength);
  // COMPILE SHADER
  glCompileShader(shdr);                                                              

  GLint result = 0;                   
  GLchar eLog[1024] = { 0 };
  // GET COMPILE STATUS AND PRINT ERRORS IF ANY
  glGetShaderiv(shdr, GL_COMPILE_STATUS, &result);                                   	
  if (!result) {
    glGetProgramInfoLog(shdr, 1024, NULL, eLog);
    printf("Error compiling %d shader: '%s'\n", shaderType, eLog);
    return;
  }

  // ATTACH COMPILED SHADER TO THE GIVEN SHADER PROGRAM
  glAttachShader(program, shdr);                                                    	

  return;
}

void Shader::CompileShader(const char *vertexCode, const char *fragmentCode) {
  // CREATE SHADER PROGRAM
  shaderID = glCreateProgram();                                                      	
  if (!shaderID) {
    printf("Error creating shader program!");
    return;
  }

  // ADD VERTEX SHADER TO THE PROGRAM
  AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
  // ADD FRAGMENT SHADER TO THE PROGRAM
  AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);                              

  GLint result = 0;
  GLchar eLog[1024] = { 0 };
  // LINK SHADER PROGRAM
  glLinkProgram(shaderID);
  // GET LINK STATUS AND PRINT ERRORS IF ANY
  glGetProgramiv(shaderID, GL_LINK_STATUS, &result);                                  
  if (!result) {
    glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
    printf("Error linking program: '%s'\n", eLog);
    return;
  }

  // VALIDATE SHADER PROGRAM
  glValidateProgram(shaderID);
  // GET VALIDATION STATUS AND PRINT ERRORS IF ANY
  glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);                              
  if (!result) {
    glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
    printf("Error validating program: '%s'\n", eLog);
    return;
  }

  // ASSIGN UNIFORMS' TO THEIR CORRESPONDENTS IN THE SHADER
  uniformModel = glGetUniformLocation(shaderID, "model");                             
  uniformProjection = glGetUniformLocation(shaderID, "projection");
  uniformView = glGetUniformLocation(shaderID, "view");
  uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

  uniformDirectionalLight.baseLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "dl.base.ambientIntensity");
  uniformDirectionalLight.baseLight.uniformColor = glGetUniformLocation(shaderID, "dl.base.color");
  uniformDirectionalLight.baseLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "dl.base.diffuseIntensity");
  uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "dl.direction");
  
  uniformSpecularIntensity = glGetUniformLocation(shaderID, "mat.specularIntensity");
  uniformShininess = glGetUniformLocation(shaderID, "mat.shininess");

  char locBuff[100] = { '\0' };

  uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");
  // ASSIGN TO ARRAY OF POINT LIGHTS
  for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {                                      
    snprintf(locBuff, sizeof(locBuff), "pl[%d].base.color", i);
    uniformPointLight[i].baseLight.uniformColor = glGetUniformLocation(shaderID, locBuff);
    snprintf(locBuff, sizeof(locBuff), "pl[%d].base.ambientIntensity", i);
    uniformPointLight[i].baseLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);
    snprintf(locBuff, sizeof(locBuff), "pl[%d].base.diffuseIntensity", i);
    uniformPointLight[i].baseLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

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
  for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++) {                                      // ASSIGN TO ARRAY OF SPOT LIGHTS
    snprintf(locBuff, sizeof(locBuff), "sl[%d].base.base.color", i);
    uniformSpotLight[i].baseLight.uniformColor = glGetUniformLocation(shaderID, locBuff);
    snprintf(locBuff, sizeof(locBuff), "sl[%d].base.base.ambientIntensity", i);
    uniformSpotLight[i].baseLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);
    snprintf(locBuff, sizeof(locBuff), "sl[%d].base.base.diffuseIntensity", i);
    uniformSpotLight[i].baseLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

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
  dLight->UseLight(uniformDirectionalLight.baseLight.uniformAmbientIntensity, uniformDirectionalLight.baseLight.uniformColor,
                   uniformDirectionalLight.baseLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight *pLight, unsigned int lightCount) {
  if (lightCount > MAX_POINT_LIGHTS) {
    lightCount = MAX_POINT_LIGHTS;
  }
  glUniform1i(uniformPointLightCount, lightCount);

  for (size_t i = 0; i < lightCount; i++) {
    pLight[i].UseLight(uniformPointLight[i].baseLight.uniformAmbientIntensity, uniformPointLight[i].baseLight.uniformColor, uniformPointLight[i].baseLight.uniformDiffuseIntensity,
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
    sLight[i].UseLight(uniformSpotLight[i].baseLight.uniformAmbientIntensity, uniformSpotLight[i].baseLight.uniformColor, uniformSpotLight[i].baseLight.uniformDiffuseIntensity,
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
  
  uniformSpecularIntensity = 0;
  uniformShininess = 0;
  
  pointLightCount = 0;
  uniformPointLightCount = 0;
}

// DESTRUCTOR
Shader::~Shader() {
  ClearShader();
}
