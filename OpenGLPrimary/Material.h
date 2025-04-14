#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Material
{
	public:
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
		Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess):
			ambient(ambient),
			diffuse(diffuse),
			specular(specular),
			shininess(shininess)
		{};
};

