/*
Light Class for use in conjunction with ShaderClass
written by Julius Neudecker
v0.1 28.07.2020
v0.2 30.07.2020 - Added pbrLight to baseClass
*/
#ifndef LIGHT_H
#define LIGHT_H

#include <glfwHandler.h>
#include <glm/glm.hpp>
#include <jLog.h>

class Light {
public:
	Light();
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool hasRepresentation);
	Light(glm::mat3 colorParams, bool hasRepresentation);

	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	glm::vec3 pbrColor;
	bool isPBR = false;

	void ChangeLightColor(glm::mat3 colorParams);

	virtual void SetDirection(glm::vec3 direction) {}
	virtual void SetPosition(glm::vec3 position) {}
	virtual void SetDecayParams(glm::vec3 decayParams) {}
	virtual void SetCutOffParams(glm::vec2 params) {}

	bool HasRepresentation;
};

class DirectionalLight : public Light {
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 direction) : Direction{ direction } {}

	glm::vec3 Direction;
};

class DecayLight : public Light {
public:
	DecayLight();
	DecayLight(float, float, float) : ConstantDecay{ 1.0f }, LinearDecay{ 0.09f }, QuadraticDecay{ 0.032f } {}

	void SetDecayParams(glm::vec3 decayParams) override;

	float ConstantDecay;
	float LinearDecay;
	float QuadraticDecay;
};

class PointLight : public DecayLight {
public:
	PointLight();
	PointLight(glm::vec3 position);
	PointLight(glm::vec3 position, glm::vec3 _pbrColor);

	glm::vec3 Position;
};

class SpotLight : public DecayLight {
public:
	SpotLight();
	SpotLight(glm::vec3 position, glm::vec3 direction, float, float);

	glm::vec3 Position;
	glm::vec3 Direction;

	void SetCutOffParams(glm::vec2 params) override;

	//retrieves data direct from glfwHandler (dirty but easy...)
	void SetPosition(glm::vec3 position) override;
	void SetDirection(glm::vec3 direction) override;

	float InnerCutOff;
	float OuterCutOff;
};

#endif 


