/*
SSBO (Shader Storage Buffer)-Class
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
#include <glad/glad.h>

#include <Model.h> //contains all necessary data

struct testInt {
	unsigned int position;
	float sqrt;
};

class SSBO {
public:
	SSBO() { _initBuffer(); }
	~SSBO();

	//pure virtual -> abstract!
	virtual void MapBuffer() = 0;
	virtual void FillBuffer() = 0;
	virtual void LoadBuffer() = 0;


protected:
	unsigned int _SSBO; //handle
	unsigned int _bindingIndex; //see layout specifier in shadercode
	GLvoid* gpuMem;

	void _initBuffer();

};

class vertexSSBO : SSBO {
public:
	vertexSSBO() { _bindingIndex = 0;  }
	~vertexSSBO();

	void FillBuffer(Model model);
private:

};

class testSSBO : SSBO {
public:
	testSSBO();
	testInt td;

	GLuint bindingPoint;

	void MapBuffer(Shader* shader); //no override bc of overloading
	void MapBuffer() override {}
	void FillBuffer() override;
	void LoadBuffer() override;
};


#endif
