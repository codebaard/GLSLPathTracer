/*
3D Model Class using Assimp
written by Julius Neudecker
v0.1 26.07.2020
v0.2 28.07.2020 - Added MTL material and texture loading
v0.3 31.07.2020 - Added PBR material and texture loading
*/

#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>

#include <Mesh.h>
#include <Shader.h>
#include <jLog.h>
//#include <Material.h>

#include <string>
#include <vector>

class Model {
public:
	Model() {}
	Model(std::string path) { _loadModel(path); }
	void Draw(RenderShader& shader);

	//unsigned int glTextureID = 0x84C0; //initialize with GL_TEXTURE0
private:
	//model data
	std::vector<Mesh> _meshes;
	std::vector<Texture> _loadedTextures;
	std::string _directory;

	void _loadModel(std::string path);
	void _processNode(aiNode* node, const aiScene* scene);
	Mesh _processMesh(aiMesh* mesh, const aiScene* scene);
	Texture _loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif