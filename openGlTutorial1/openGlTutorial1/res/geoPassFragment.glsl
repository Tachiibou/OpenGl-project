#version 430

layout(location = 0) out vec3 gPos;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gColor;

in vec3 fragPos;
in vec2 fragUv;
in vec3 fragNormal;
in vec3 fragLightPos;

uniform sampler2D texture;
uniform sampler2D depth;

void main()
{


	gPos = clamp(fragPos,0.0,1.0);
	gNormal = clamp(normalize(fragNormal),0.0,1.0);
	gColor = clamp(vec3(texture2D(texture, fragUv)) + vec3(texture2D(depth, fragUv)),0.0,1.0);
}