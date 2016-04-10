#version 430

in vec3 position;
in vec2 uv;

out vec2 fragment_uv;

void main()
{
	fragment_uv = uv;

	gl_Position = position;
}