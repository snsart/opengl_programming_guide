#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"
#include <GL/glew.h>
#include <assimp/types.h>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 Texcoords;
};
struct Texture {
	unsigned int id;
	std::string type;
	aiString path;
};
class Mesh
{
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		unsigned int VAO;
		Mesh(float vertices[]);
		Mesh(std::vector<Vertex> vertices,std::vector<unsigned int> indices,std::vector<Texture> textures);
		void Draw(Shader* shader);
	private:
		unsigned int VBO, EBO;
		void setupMesh();
};

