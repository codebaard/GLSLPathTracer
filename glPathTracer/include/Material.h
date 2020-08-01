/*
Material class for use in my openGL Renderer
Supports:
	- mtl-style material for OBJ-Files
	- PBR-style material (Linear Workflow!)
Intended for use in conjunction with my Texture-Class, glm and ASSIMP

written by Julius Neudecker
v0.1 28.07.2020
v0.2 31.07.2020 - Added pbr functionality
*/
#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <jLog.h>
#include <string>
#include <Texture.h>

class Material {
public:
	Material(){}
	//virtual void LoadMaterial(const char* filepath);

	Texture Normal;
	std::string MaterialName;
};

class pbrMaterial : public Material {
public:
	pbrMaterial() {}

	glm::vec3 Albedo;

	Texture AlbedoMap;
	Texture RoughnessMap;
	Texture AOMap;

	float Metallic;
	float Roughness;
	float IOR;
	float Tr;

	//The base reflectivity of metals is usual tinted.
	//If needed, this parameter can be adjusted for metals.
	glm::vec3 F0 = glm::vec3(0.04f);
};

class mtlMaterial : public Material {
public:
	mtlMaterial() {}

	pbrMaterial* TransformToPBR(); //easiest way to mangle parametres since OBJ-Model loading is mtl-Based.	

	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	glm::vec3 Ke;

	Texture Ambient;
	Texture Diffuse;
	Texture Specular;
	Texture Emissive;

	float Ns; //specular Exponent
	float Ni; //Optical density (refraction)
	float d; //dissolve (Transparency)

	unsigned int illum; //illumination Model
};



#endif
