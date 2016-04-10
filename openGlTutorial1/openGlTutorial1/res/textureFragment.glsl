#version 430

in vec2 fragment_uv;
out vec4 color;

void main()
{
	uniform sampler2D renderedTexture;

	//color = vec3(1,0,0);
	color = texture2D(renderedTexture, fragment_uv);
}