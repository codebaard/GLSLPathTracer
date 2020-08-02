/*
This class represents one Polygon for the raytracing algorithm.
Written by Julius Neudecker
v0.1 - 02.08.2020
*/

#ifndef RENDERMESH_H
#define RENDERMESH_H

#include <glm/glm.hpp>
#include <Model.h>

struct Triangle {
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;

	glm::vec3 indices;

	glm::vec3 Edge1;
	glm::vec3 Edge2;

	glm::vec3 n1;
	glm::vec3 n2;
	glm::vec3 n3;

	glm::vec3 Albedo;

	float Metalness;
	float Roughness;
	float IOR;
	float Tr;
	
	glm::vec3 F0;
};

class Rendermesh {
public:
	Rendermesh() { Facecount = 0; Faces = NULL; }
	Triangle* Faces;

	void ParseModelData(Model* model);
	__int64 Facecount;

private:

};

#endif
