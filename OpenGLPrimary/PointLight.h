#pragma once
#include <glm/glm.hpp>
class PointLight
{
public:
	glm::vec3 Position;
	glm::vec3 LightColor;
	PointLight(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 lightColor = glm::vec3(1.0f, 0.8f, 0.8f)) {
		Position = position;
		LightColor = lightColor;
	}
};

