/*
SSBO (Shader Storage Buffer Object)-Class
One Instance is a Handle for each GLSL SSBO and provides the necessary Interface for
loading and storing all data within the scope of this application.
written by Julius Neudecker

v0.1 - 02.08.2020
*/
#ifndef SSBO_H
#define SSBO_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <glad/glad.h>

#include <Rendermesh.h>

//binding points according to shader code!
#define TRANSFORM_SSBO_BINDING_POINT 0
#define RENDERMESH_SSBO_BINDING_POINT 1

struct Matrices {
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;
};

class SSBO {
public:
	SSBO() { _initBuffer(); }
	virtual void MapBufferToAdressSpace(GLuint programID) = 0;

protected:
	GLuint _SSBO; //handle
	unsigned int _bindingIndex; //see layout specifier in shadercode
	const char* _bindingName;

	GLvoid* gpuMem;

	void _initBuffer();
};

class RendermeshSSBO : SSBO {
public:
	RendermeshSSBO();

	void FillBuffer(Rendermesh* rendermesh);
	void ReadBuffer();

	void MapBufferToAdressSpace(GLuint programID) override;

	std::string DebugGiveVector();

private: 
	Rendermesh* _mesh;

};

class TransformSSBO : SSBO {
public:
	TransformSSBO();

	void FillBuffer(glm::mat4 projection, glm::mat4 view, glm::mat4 model);
	void ReadBuffer();

	void MapBufferToAdressSpace(GLuint programID) override;

	std::string DebugGiveVector();

private:
	Matrices* _mat;
	void _packData(glm::mat4 projection, glm::mat4 view, glm::mat4 model);
};


#endif
