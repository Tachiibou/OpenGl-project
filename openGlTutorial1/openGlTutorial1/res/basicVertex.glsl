#version 430

layout(location = 0)in vec3 position;
layout(location = 1)in vec2 uv;
layout(location = 2)in vec3 normal;

uniform mat4 viewMatrix;
uniform mat4 perspectiveMatrix;

out vec2 fragment_uv;
out vec3 fragment_normal;

void main(){
	fragment_uv = uv;
	fragment_normal = normal;
	vec3 n = normalize(normal);
	gl_Position = (perspectiveMatrix * viewMatrix) * (vec4(position, 1.0) );
}