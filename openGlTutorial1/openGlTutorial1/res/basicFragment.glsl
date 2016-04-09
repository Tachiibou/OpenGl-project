#version 430

uniform sampler2D texture;
out color;

in vec2 fragment_uv;
in vec3 fragment_normal;

void main(){
	color = texture2D(texture, fragment_uv);
}