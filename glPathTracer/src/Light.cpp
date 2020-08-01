#include <Light.h>

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool hasRepresentation) {
	HasRepresentation = hasRepresentation;

	Ambient = ambient;
	Diffuse = diffuse;
	Specular = specular;
}

Light::Light(glm::mat3 colorParams, bool hasRepresentation) {
	HasRepresentation = hasRepresentation;
	ChangeLightColor(colorParams);
}

Light::Light() {
	Ambient = glm::vec3(0.2f);
	Diffuse = glm::vec3(0.5f);
	Specular = glm::vec3(1.0f);
	HasRepresentation = false;
}

void Light::ChangeLightColor(glm::mat3 colorParams) {

	//glm is column major matrix. To Access rows, we need to transpose
	//colorParams = glm::transpose(colorParams);

	Ambient = colorParams[0];
	Diffuse = colorParams[1];
	Specular = colorParams[2];
}

DecayLight::DecayLight() {
	ConstantDecay = 1.0f;
	LinearDecay = 0.09f;
	QuadraticDecay = 0.032f;
}

void DecayLight::SetDecayParams(glm::vec3 decayParams) {
	ConstantDecay = decayParams.x;
	LinearDecay = decayParams.y;
	QuadraticDecay = decayParams.z;
}

PointLight::PointLight(glm::vec3 position) {
	Position = position;
	HasRepresentation = true;
}

PointLight::PointLight(glm::vec3 position, glm::vec3 _pbrColor) {
	Position = position;
	pbrColor = _pbrColor;
	HasRepresentation = true;
	isPBR = true;
}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float innerCutOff, float outerCutOff) {

	Position = position;
	Direction = direction;

	InnerCutOff = innerCutOff;
	OuterCutOff = outerCutOff;

	HasRepresentation = false; //might change - depending on usage in programm later on
}

void SpotLight::SetCutOffParams(glm::vec2 params) {
	InnerCutOff = params.x;
	OuterCutOff = params.y;
}

void SpotLight::SetPosition(glm::vec3 position) {
	Position = position;
}
void SpotLight::SetDirection(glm::vec3 direction) {
	Direction = direction;
}