/*
Shader class. Contains all different implementations of the Shader class.
For Rasterized rendering, Raytracing and Postprocessing.
written by Julius Neudecker
v0.1 21.07.2020
v0.2 29.07.2020 - added support for pbrMaterials
v0.3 31.07.2020 - created derived classes with different shader types
*/

#ifndef SHADER_H
#define SHADER_H

#define INFO_LOG_SIZE 512

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <jLog.h>
#include <Light.h>
#include <Material.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

enum ShaderType {
	VERTEX,
	FRAGMENT,
	COMPUTE
};

struct ShaderProgram
{
	std::string filepath;
	ShaderType type;
	unsigned int ID;
	ShaderProgram* nextElement;
};


class Shader {
public:

	//the program ID
	unsigned int ID;

	//constructor and shader builds
	Shader();
	void AddShaderToPipeline(std::string cwd, const char* filename, ShaderType);
	void InitShader();

	//use/activate the shader
	void use() const; 

	//untility uniform functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setFloat4f(const std::string& name, float v1, float v2, float v3, float v4) const;
	void setMat4fv(const std::string& name, GLfloat* trans) const;
	void setVec3f(const std::string& name, float v1, float v2, float v3) const;
	void setVec3fv(const std::string& name, GLfloat* vec) const;
	void setVec4f(const std::string& name, float v0, float v1, float v2, float v3) const;
	void setVec4fv(const std::string& name, GLfloat* vec) const;

private:
	ShaderProgram* _shaderList;
	unsigned int _buildShader(ShaderType, const char*);
	void _cleanShaderList(); //unlink elements from list und delete structs
};

class RenderShader : public Shader {
public:
	RenderShader() {}

	//set of light sources
	void setLight(DirectionalLight* light);
	void setLight(PointLight* light);
	void setLight(PointLight* light, unsigned int i);
	void setLight(SpotLight* light);

	//set Material
	void setMaterial(mtlMaterial* mat);
	void setMaterial(pbrMaterial* mat);

};

class ComputeShader : public Shader {
public:
	ComputeShader() {}
	//void DispatchCompute(unsigned int ProblemSize, unsigned int WorkGroups, unsigned int WorkItems, unsigned int dim);
	void DispatchCompute(unsigned int x, unsigned int y, unsigned int z);
	//void SetWorkGroupSize();

	unsigned int ProblemSize;
	unsigned int WorkGroups; //Global problem space divided into several workgroups
	unsigned int WorkItems; //local in WorkGroups

	//void createSSBO();
};

#endif