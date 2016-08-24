#include "ConstVariables.h"
class Material
{
private:
	glm::vec3 diffuseRef;
	glm::vec3 ambientRef;
	glm::vec3 specularRef;
	glm::vec3 transmissionFilter;

	int illum;
	float opticalDensity;
public:
	Material(int illum, glm::vec3 diffuseRef,
		glm::vec3 ambientRef,
		glm::vec3 transmissionFilter,
		float opticalDensity,
		glm::vec3 specularRef);
	~Material();
	

	glm::vec3 &getDiffuseRef();
	glm::vec3 &getAmbientRef();
	glm::vec3 &getSpecularRef();
	glm::vec3 &getTransmissionFilter();

	int &getIllum();
	float &getOpticalDensity();
};

