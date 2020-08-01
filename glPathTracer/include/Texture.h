/*
Texture Class using Assimp for openGL Interaction
written by Julius Neudecker, based on Joey de Vries tutorial
v0.1 26.07.2020
v0.2 28.07.2020 - Added more Texture properties for smoother interaction
*/

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glfwHandler.h>
#include <stb_image.h>
#include <jLog.h>
#include <string>
#include <List>

enum UniformIndex {
	DIFFUSE_MAP,
	SPECULAR_MAP,
	AMBIENT_MAP,
	EMISSIVE_MAP,
	NORMAL_MAP,
	ALBEDO_MAP,		//ASSIMP: Base Color
	ROUGHNESS_MAP,
	AO_MAP
};

class Texture {
public:
	Texture() { IsLoaded = false; }
	Texture(const char* filepath, unsigned int aiTextureType, std::string typeName);
	Texture(unsigned int aiTextureType, std::string typeName, Texture* loadedTex);
	static void FlipTexturesVertically(); //not needed, when using assimp->FlipUVs

	void ActivateTexture();
	

	bool IsLoaded = false;

	std::string TextureType; //diffuse, normal, specular, displacement, ...
	std::string FileName;

	std::string UniformName; //according to frag-shader
	unsigned int UniformID; //according to frag-shader

	/*New stuff:
	GL Texture type
	Texture Buffer (for use as renderbuffer)
	*/
	
	void DeactivateTexture();
	void SetGLParams(unsigned int id, unsigned int glTextureUnit);
	unsigned int GiveGLTextureUnit();
	unsigned int GiveGlTextureID();

private:
	unsigned int _determineClrChannels();
	void _setUniformParams(unsigned int aiTextureType, std::string uniformName);
	void _configGLTexture();
	

	unsigned int _clrChannels;
	unsigned int _textureID;
	unsigned char* _data;
	unsigned int _glTextureObject;
	int _width;
	int _height;
	int _nrChannels;
};

#endif
