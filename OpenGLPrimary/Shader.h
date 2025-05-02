#pragma once
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
		Shader(const char* vertexPath, const char* fragmentPath);
		Shader(const char* vertexPath, const char* fragmentPath,const char* geometryPath);
		std::string vertexString;
		std::string fragmentString;
		std::string geometryString;
		const char* vertexSource;
		const char* fragmentSource;
		const char* geometrySource;
		unsigned int ID;//shader program ID£»
		enum Slot
		{
			DIFFUSE,
			SPECULAR,
			EMISSION
		};
		void use();
		void setUniform3f(const char* paramNameString,glm::vec3 param);
		void setUniform1f(const char* paramNameString, float param);
		void setUniform1i(const char* paramNameString, unsigned int slot);
private:
		void checkCompileErrors(unsigned int ID, std::string type);
};

