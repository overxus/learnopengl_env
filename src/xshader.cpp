#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <xshader.h>


static const int SHADER_VERTEX = 0;
static const int SHADER_FRAGMENT = 1;
static const int SHADER_PROGRAM = 2;


xShader::xShader(const char* vertexFilePath, const char* fragmentFilePath) {
	std::ifstream vShaderFile, fShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	std::string vCode, fCode;
	try {
		vShaderFile.open(vertexFilePath);
		fShaderFile.open(fragmentFilePath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vCode = vShaderStream.str();
		fCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << "error: failed to open file: " << e.what() << std::endl;
		exit(-1);
	}
	const char* vShaderCode = vCode.c_str();
	unsigned int vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexID, 1, &vShaderCode, NULL);
	glCompileShader(vertexID);
	checkCompileErrors(vertexID, SHADER_VERTEX);

	const char* fShaderCode = fCode.c_str();
	unsigned int fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentID, 1, &fShaderCode, NULL);
	glCompileShader(fragmentID);
	checkCompileErrors(fragmentID, SHADER_FRAGMENT);

	ID = glCreateProgram();
	glAttachShader(ID, vertexID);
	glAttachShader(ID, fragmentID);
	glLinkProgram(ID);
	checkCompileErrors(ID, SHADER_PROGRAM);
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}


void xShader::checkCompileErrors(GLuint shader, int shader_type) {
	GLint success;
	GLchar infoLog[1024];
	if (shader_type != SHADER_PROGRAM)
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			if (shader_type == SHADER_VERTEX) {
				std::cerr << "error: vertex shader compile error: " << infoLog << "\n";
				exit(-1);
			}
			else {
				// shader_type == SHADER_FRAGMENT
				std::cerr << "error: fragment shader compile error: " << infoLog << "\n";
				exit(-1);
			}
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "error: program link error: " << infoLog << "\n";
			exit(-1);
		}
	}
}