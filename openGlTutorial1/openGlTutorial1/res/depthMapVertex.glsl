#version 430
layout (location = 0) in vec3 position;

uniform mat4 viewMatrix;
uniform mat4 perspectiveMatrix;

void main()
{
	gl_Position = perspectiveMatrix * viewMatrix * vec4(position, 1.0f);
}