#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 perspectiveMatrix;

void main()
{
	//vec4 worldPos = vec4(position, 1);

	gl_Position = perspectiveMatrix * viewMatrix * worldMatrix * vec4(position, 1);
}