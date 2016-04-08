#version 420

in vec2 GSuv;
in vec3 GSnormal;

uniform sampler2D texture;

void main()
{
	gl_FragColor = texture2D(texture, GSuv);
}