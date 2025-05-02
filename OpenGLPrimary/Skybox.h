#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"
#include <GL/glew.h>

class Skybox
{
	public: 
		unsigned int textureID;
		Skybox(std::string path);
		void draw(Shader* shader);
	private:
		unsigned int VAO, VBO;
		std::string directory;
		void setup();
		unsigned int loadCubemap(std::vector<std::string> texture_faces);
};

