#version 430

in vec2 fragment_uv;
out vec4 color;

uniform sampler2D renderedTexture;
uniform sampler2D renderedTexture2;
uniform sampler2D renderedTexture3;

void main()
{
	//color = vec4(1,0,0,1);
	//color = texture2D(renderedTexture3, fragment_uv);
	color = (texture2D(renderedTexture, fragment_uv) * 0.5) + (texture2D(renderedTexture2, fragment_uv) * 0.5) + (texture2D(renderedTexture3, fragment_uv) * 0.0) ;
}