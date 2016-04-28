#version 430

in vec2 fragUv;
out vec3 fragColor;

uniform sampler2D renderedTexture;
uniform sampler2D renderedTexture2;
uniform sampler2D renderedTexture3;
uniform sampler2D renderedTexture4;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{

	// Get the data from the textures
	vec3 fragPos = texture2D(renderedTexture, fragUv).xyz;
	vec3 fragNormal = texture2D(renderedTexture2, fragUv).xyz;
	vec3 fragDiff = texture2D(renderedTexture3, fragUv).xyz;

	vec3 lighting = fragDiff * 0.1; //Hardcoded ambient
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 lightColor = vec3(1, 1, 1);
	
	//Diffuse
	vec3 diffuse = max(dot(fragNormal, lightDir), 0.0) * fragDiff * lightColor;
	
	//Specular
	float specStr = 0.5; //Hardcoded specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(fragNormal, halfwayDir), 0.0), 16.0);
	vec3 specular = lightColor * spec * specStr;

	lighting += diffuse + specular;
	//fragColor = lighting;
	fragColor = texture2D(renderedTexture3, fragUv).xyz;
}