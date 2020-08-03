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
	glm::float32 pad0 = 0;

	glm::vec3 v2;
	glm::float32 pad1;

	glm::vec3 v3;
	glm::float32 pad2;

	glm::vec3 indices;
	glm::float32 pad3;

	glm::vec3 Edge1;
	glm::float32 pad4;

	glm::vec3 Edge2;
	glm::float32 pad5;

	glm::vec3 n1;
	glm::float32 pad6;

	glm::vec3 n2;
	glm::float32 pad7;

	glm::vec3 n3;
	glm::float32 pad8;

	glm::vec3 Albedo;
	glm::float32 pad9;

	glm::vec3 F0;
	glm::float32 pad10;

	glm::float32 Metalness;
	glm::float32 pad11[3];

	glm::float32 Roughness;
	glm::float32 pad12[3];

	glm::float32 IOR;
	glm::float32 pad13[3];

	glm::float32 Tr;
	glm::float32 pad14[3];

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
