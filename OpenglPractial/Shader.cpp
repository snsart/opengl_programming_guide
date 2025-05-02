#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	try
	{
		vertexString = loadShaderSource(vertexPath);
		fragmentString = loadShaderSource(fragmentPath);
		vertexSource = vertexString.c_str();
		fragmentSource = fragmentString.c_str();
		
		unsigned int vertex, fragment;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexSource, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentSource, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
	}
	catch (const std::exception& ex)
	{
		printf(ex.what());
	}
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	try
	{
		vertexString = loadShaderSource(vertexPath);
		fragmentString = loadShaderSource(fragmentPath);
		geometryString = loadShaderSource(geometryPath);
		vertexSource = vertexString.c_str();
		fragmentSource = fragmentString.c_str();
		geometrySource = geometryString.c_str();

		unsigned int vertex, fragment, geometry;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexSource, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentSource, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geometrySource, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glAttachShader(ID, geometry);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
	}
	catch (const std::exception& ex)
	{
		printf(ex.what());
	}
}

std::string Shader::loadShaderSource(const char* sourcePath) {
	std::ifstream sourceFile;
	sourceFile.open(sourcePath);
	std::stringstream vertexStream;
	sourceFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);
	try
	{
		if (!sourceFile.is_open()) {
			throw std::exception("open file error");
		}
		vertexStream << sourceFile.rdbuf();
		return vertexStream.str();
	}
	catch (const std::exception& ex)
	{
		printf(ex.what());
		return "";
	}
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setUniform3f(const char* paramNameString, glm::vec3 param)
{
	glUniform3f(glGetUniformLocation(ID, paramNameString), param.x, param.y, param.z);
}

void Shader::setUniform1f(const char* paramNameString, float param)
{
	glUniform1f(glGetUniformLocation(ID, paramNameString), param);
}

void Shader::setUniform1i(const char* paramNameString, unsigned int slot)
{
	glUniform1i(glGetUniformLocation(ID, paramNameString), slot);
}

void Shader::checkCompileErrors(unsigned int ID, std::string type) {
	int success;
	char infoLogo[512];

	if (type != "PROGRAM") {
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(ID, 512, NULL, infoLogo);
			std::cout << "shader compile error:" << infoLogo << std::endl;
		}
	}
	else {
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLogo);
			std::cout << "program link error:" << infoLogo << std::endl;
		}
	}
}
