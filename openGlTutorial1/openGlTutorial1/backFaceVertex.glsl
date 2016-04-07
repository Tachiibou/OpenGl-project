#version 420

layout(location = 0)in vec3 position;
layout(location = 1)in vec2 uv;
layout(location = 2)in vec3 normal;

uniform mat4 viewMatrix;
uniform mat4 perspectiveMatrix;

out VertexVS
{
	vec2 uv;
	vec3 normal;
} vertex;

int main()
{
	gl_Position = viewMatrix * perspectiveMatrix * vec4(position, 1);
	vertex.uv = uv;
	vertex.normal = normal;
}