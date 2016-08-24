#include "Material.h"



Material::Material(int illum, glm::vec3 diffuseRef,glm::vec3 ambientRef,
	glm::vec3 transmissionFilter,float opticalDensity,glm::vec3 specularRef) {

	this->illum = illum;
	this->diffuseRef = diffuseRef;
	this->ambientRef = ambientRef;
	this->transmissionFilter = transmissionFilter;
	this->opticalDensity = opticalDensity;
	this->specularRef = specularRef;
}


Material::~Material()
{
}

glm::vec3 &Material::getDiffuseRef()
{
	return diffuseRef;
}

glm::vec3& Material::getAmbientRef()
{
	return ambientRef;
}

glm::vec3 &Material::getSpecularRef()
{
	return specularRef;
}

glm::vec3 &Material::getTransmissionFilter()
{
	return transmissionFilter;
}

int &Material::getIllum()
{
	return illum;
}

float &Material::getOpticalDensity()
{
	return opticalDensity;
}
