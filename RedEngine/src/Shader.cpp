#include "Shader.h"
#include <GL/glew.h>
#include <fstream>
#include <sstream>

Shader::Shader(std::string shaderPath) {
	std::string vertShaderSource, fragShaderSource;
	parseShader(shaderPath.c_str(), &vertShaderSource, &fragShaderSource);

	unsigned int vertexShader = compileShader(ShaderType::VertexShader, vertShaderSource.c_str());
	unsigned int fragmentShader = compileShader(ShaderType::FragmentShader, fragShaderSource.c_str());

	m_programID = createShaderProgram(vertexShader, fragmentShader);

	useShader();

	m_projectionUniformLocation = getUniformLocation("u_projMat");
	m_modelUniformLocation = getUniformLocation("u_modelMat");
	m_viewUniformLocation = getUniformLocation("u_viewMat");
	m_lightPosUniformLocation = getUniformLocation("u_lightPos");
	m_lightBrightnessUniformLocation = getUniformLocation("u_lightBrightness");
	m_lightCountUniformLocation = getUniformLocation("u_lightCount");
	m_cameraPosUniformLocation = getUniformLocation("u_cameraPos");
	m_textureSamplerUniformLocation = getUniformLocation("u_texture");
	m_specularSamplerUniformLocation = getUniformLocation("u_specular");
	setUniform1i(m_textureSamplerUniformLocation, 0);
	setUniform1i(m_specularSamplerUniformLocation, 1);
}

Shader::~Shader() {
}

void Shader::useShader() {
	glDebug(glUseProgram(m_programID));
}

void Shader::setUniform1i(int uniformLocation, int value) {
	glDebug(if (uniformLocation > -1) glUniform1i(uniformLocation, value));
}

void Shader::setUniform1f(int uniformLocation, float value) {
	glDebug(if(uniformLocation > -1) glUniform1f(uniformLocation, value));
}

void Shader::setUniformMat4f(int uniformLocation, float * mat4) {
	glDebug(if(uniformLocation > -1) glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, mat4));
}

void Shader::setUniformVec3f(int uniformLocation, float * vec3) {
	glDebug(if (uniformLocation > -1) glUniform3fv(uniformLocation, 1, vec3));
}

void Shader::setUniformVec3fArray(int uniformLocation, float * arrayPointer, unsigned int vec3Count) {
	glDebug(if (uniformLocation > -1) glUniform3fv(uniformLocation, vec3Count, arrayPointer));
}

int Shader::getUniformLocation(const char * uniformName) {
	glDebug(return  glGetUniformLocation(m_programID, uniformName));
}

unsigned int Shader::compileShader(ShaderType shaderType, const char * shaderSource) {
	GLenum glShaderType = ShaderTypeToGLShaderType(shaderType);

	glDebug(unsigned int shaderID = glCreateShader(glShaderType));
	glDebug(glShaderSource(shaderID, 1, &shaderSource, NULL));
	glDebug(glCompileShader(shaderID));

	checkForShaderError(shaderID);

	return shaderID;
}

unsigned int Shader::createShaderProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID) {
	glDebug(unsigned int shaderProgram = glCreateProgram());
	glDebug(glAttachShader(shaderProgram, vertexShaderID));
	glDebug(glAttachShader(shaderProgram, fragmentShaderID));
	glDebug(glLinkProgram(shaderProgram));

	glDebug(glDeleteShader(vertexShaderID));
	glDebug(glDeleteShader(fragmentShaderID));

	checkForProgramError(shaderProgram);

	return shaderProgram;
}

void Shader::parseShader(const char* shaderPath, std::string * vertexShaderSource, std::string* fragmentShaderSource) {
	std::ifstream ifs(shaderPath);
	std::stringstream vertexStringstream, fragmentStringstream;
	std::string line;
	int shaderType = 0; //0:none, 1:vertex, 2:fragment
	if (ifs.is_open()) {
		while (std::getline(ifs, line)) {
			if (line[0] == '/' && line[1] == '/') {
				if (line.find("VERTEX SHADER") != std::string::npos) { shaderType = 1; }
				if (line.find("FRAGMENT SHADER") != std::string::npos) { shaderType = 2; }
			}
			else {
				if (shaderType == 1) {
					vertexStringstream << line << '\n';
				}
				else if (shaderType == 2) {
					fragmentStringstream << line << '\n';
				}
			}
		}
		*vertexShaderSource = vertexStringstream.str();
		*fragmentShaderSource = fragmentStringstream.str();
	}
	else {
		gameEngineDebug("Could not open shader file: " + (std::string)shaderPath);
	}
	
}

void Shader::checkForShaderError(unsigned int shaderID) {
	int shaderComp;
	glDebug(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderComp));
	if (shaderComp != GL_TRUE) {
		int log_len = 0;
		char message[1024];
		glDebug(glGetShaderInfoLog(shaderID, 1024, &log_len, message));

		gameEngineDebug(message);
	}
}

void Shader::checkForProgramError(unsigned int programID) {
	int programLink;
	glDebug(glGetProgramiv(programID, GL_LINK_STATUS, &programLink));
	if (programLink != GL_TRUE) {
		int logLen = 0;
		char message[1024];
		glDebug(glGetProgramInfoLog(programID, 1024, &logLen, message));

		gameEngineDebug(message);
	}
}

unsigned int Shader::ShaderTypeToGLShaderType(ShaderType shaderType) {
	switch(shaderType) {
		case ShaderType::VertexShader:
			return GL_VERTEX_SHADER;
		case ShaderType::FragmentShader:
			return GL_FRAGMENT_SHADER;
		case ShaderType::ComputeShader:
			return GL_COMPUTE_SHADER;
		case ShaderType::TessControlShader:
			return GL_TESS_CONTROL_SHADER;
		case ShaderType::TessEvaluationShader:
			return GL_TESS_EVALUATION_SHADER;
		case ShaderType::GeometryShader:
			return GL_GEOMETRY_SHADER;
		default:
			gameEngineDebug("Error in Shader.cpp: ShaderTypeToGLShaderType(ShaderType shaderType): Shadertype does not exist");
			break;
	}
	return 0;
}