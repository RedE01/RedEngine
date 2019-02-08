#pragma once
#include <GL/glew.h>
#include "Debug.h"

class Shader {
private:
	unsigned int m_programID;
	int m_projectionUniformLocation;
	int m_modelUniformLocation;
	int m_viewUniformLocation;
public:
	Shader();
	~Shader();

	void useShader();
	void setUniformMat4f(int uniformLocation, float* mat4);
	int getUniformLocation(const char* uniformName);
	inline int const getProjectionUniformLocation() const { return m_projectionUniformLocation; }
	inline int const getModelUniformLocation() const { return m_modelUniformLocation; }
	inline int const getViewUniformLocation() const { return m_viewUniformLocation; }
private:
	unsigned int compileShader(GLenum shaderType, const char* shaderSource);
	unsigned int createShaderProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID);
	void checkForShaderError(unsigned int shaderID);
	void checkForProgramError(unsigned int programID);
};