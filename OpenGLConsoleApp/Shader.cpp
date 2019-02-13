#include "Shader.h"

// CONSTRUCTOR
Shader::Shader() {
	shaderID = 0;
	uniformModel = 0;
	uniformView = 0;
	uniformProjection = 0;
  uniformAmbientColor = 0;
  uniformAmbientIntensity = 0;
}

// GETTER FUNCTIONS
GLuint Shader::GetProjectionLocation() {
	return uniformProjection;
}

GLuint Shader::GetModelLocation() {
	return uniformModel;
}

GLuint Shader::GetViewLocation() {
	return uniformView;
}

GLuint Shader::GetAmbientIntensityLocation() {
  return uniformAmbientIntensity;
}

GLuint Shader::GetAmbientColorLocation() {
  return uniformAmbientColor;
}

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
}

void Shader::CompileShader(const char *vertexCode, const char *fragmentCode) {
	// CREATE SHADER PROGRAM
	shaderID = glCreateProgram();
	if (!shaderID) {
		printf("Error creating shader program!");
		return;
	}

	// ADD VERTEX SHADER AND FRAGMENT SHADER TO THE PROGRAM
	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	// LINK SHADER PROGRAM, GET LINK STATUS AND PRINT ERRORS IF ANY
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	// VALIDATE SHADER PROGRAM, GET VALIDATION STATUS AND PRINT ERRORS IF ANY
	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
  uniformAmbientIntensity = glGetUniformLocation(shaderID, "dl.ambientIntensity");
  uniformAmbientColor = glGetUniformLocation(shaderID, "dl.color");
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
  uniformAmbientColor = 0;
  uniformAmbientIntensity = 0;
}

// DESTRUCTOR
Shader::~Shader() {
	ClearShader();
}
