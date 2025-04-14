#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Material
{
	public:
		glm::vec3 ambient;
		unsigned int diffuse;
		unsigned int specular;
		float shininess;
		Material(glm::vec3 _ambient, unsigned int _diffuse, unsigned int _specular, float _shininess):
			ambient(_ambient),
			diffuse(_diffuse),
			specular(_specular),
			shininess(_shininess)
		{};
};

