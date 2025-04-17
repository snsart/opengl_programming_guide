#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
class SpotLight
{
	public:
		glm::vec3 Position;
		glm::vec3 LightColor;
		glm::vec3 Direction;
		float CutOff;
		float OuterCutOff;
		SpotLight(glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f), 
			glm::vec3 lightColor = glm::vec3(0.0f, 1.0f, 1.0f),
			glm::vec3 direction = glm::vec3(0.0f,0.0f,-1.0f),
			float cutOff = glm::radians(10.0f),
			float outerCutOff = glm::radians(12.0f)) {
			Position = position;
			LightColor = lightColor;
			Direction = direction;
			CutOff = cos(cutOff);
			OuterCutOff = cos(outerCutOff);
		}
};

