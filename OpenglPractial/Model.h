#pragma once

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "Shader.h"
#include "Mesh.h"

class Model
{
public:
	std::vector<Mesh> meshes;
	Model(std::string path);
	void Draw(Shader* shader);
private:
	
	std::string directory;
	std::vector<Texture> textures_loaded;
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh,const aiScene* scene);
	std::vector<Texture>  loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* str, std::string directory);
};

