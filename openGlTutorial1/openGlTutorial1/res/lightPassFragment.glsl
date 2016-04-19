#version 430

in vec2 fragUv;
out vec3 fragColor;

uniform sampler2D renderedTexture;
uniform sampler2D renderedTexture2;
uniform sampler2D renderedTexture3;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
	fragColor = vec3(texture2D(renderedTexture3, fragUv));
}