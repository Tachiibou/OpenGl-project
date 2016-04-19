#version 430

layout(location = 0) out vec3 gPos;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gColor;

in vec3 fragPos;
in vec2 fragUv;
in vec3 fragNormal;

uniform sampler2D texture;

void main()
{
	gPos = fragPos;
	gNormal = normalize(fragNormal);
	gColor = vec3(texture2D(texture, fragUv));
}