#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

out vec2 fragUv;

void main()
{
	gl_Position = vec4(position, 1);
	fragUv = uv;
}