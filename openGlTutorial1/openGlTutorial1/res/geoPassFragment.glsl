#version 430

layout(location = 0) out vec3 gPos;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gColor;
layout(location = 3) out vec3 gSpecular;

in vec3 fragPos;
in vec2 fragUv;
in vec3 fragNormal;
in vec3 fragLightPos; // Line 10
in vec3 fragSpecular;

uniform sampler2D texture;
uniform sampler2D depth;

void main()
{
	float bias = 0.005;
	float visibility = 1.0;
	if(texture2D(depth, fragLightPos.xy).z < fragLightPos.z - bias)
		visibility = 0.5;
	// Line 20
	gPos = fragPos;
	gNormal = normalize(fragNormal);
	//gColor = clamp(vec3(texture2D(texture, fragUv)) + vec3(texture2D(depth, fragLightPos.xy)),0.0,1.0);
	//gColor = gNormal;
	gColor = fragSpecular;//vec3(texture2D(texture, fragUv)) * visibility;
	gSpecular = fragSpecular;
}