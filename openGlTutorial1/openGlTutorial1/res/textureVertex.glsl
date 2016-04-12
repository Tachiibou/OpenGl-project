#version 430

layout(location = 0)in vec3 position;
layout(location = 1)in vec2 uv;

out vec2 fragment_uv;
out vec3 fragment_normal;

void main(){
	fragment_uv = uv;
	gl_Position = vec4(position, 1.0);
}