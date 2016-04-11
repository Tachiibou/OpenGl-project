#version 420

in vec2 GSuv;
in vec3 GSnormal;

out vec3 color;
out vec3 color2;

uniform sampler2D texture;

void main()
{
	color = vec3(texture2D(texture, GSuv));
	color2 = GSnormal;
}