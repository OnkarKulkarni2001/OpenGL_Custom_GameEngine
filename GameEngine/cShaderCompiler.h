#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class cShaderCompiler {
public:
	GLuint vertexShader;
	GLuint fragmentShader;
	
	int success;
	char infoLog[512];

	GLuint shaderProgram;

	GLuint CompileShader();
	void SetSceneView(GLFWwindow* window);

private:
	GLenum viewMode = GL_LINE;
};