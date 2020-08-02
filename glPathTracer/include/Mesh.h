/*
Mesh Class
Provides complete openGL render functionality
written by Julius Neudecker
v0.1 26.07.2020
v0.2 30.07.2020 - added pbrMaterial Functionality
*/
#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <Shader.h>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangents;
	glm::vec3 BiTangents;
};

class Mesh {
public:
	//mesh data
	std::vector<Vertex>	Vertices;
	std::vector<unsigned int> Indices;
	mtlMaterial* Mat;
	pbrMaterial* pbrMat;
	std::list<Texture*> LoadedTextures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, mtlMaterial* mat);
	void Draw(RenderShader& shader);

private:
	unsigned int _VAO, _VBO, _EBO;
	void _setupMesh();
	void _setupTextures();
};

#endif 


