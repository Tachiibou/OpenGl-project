#version 420

layout(location = 0)in vec3 position;
layout(location = 1)in vec2 uv;
layout(location = 2)in vec3 normal;

uniform mat4 viewMatrix;
uniform mat4 perspectiveMatrix;

out vec3 VSpos;
out vec2 VSuv;
out vec3 VSnormal;
out vec4 VStransformedPos;

void main()
{
	gl_Position = perspectiveMatrix * viewMatrix * vec4(position, 1);

	VSpos = position;
	VSuv = uv;
	VSnormal = normal;
	VStransformedPos = viewMatrix * vec4(position, 1);
}
