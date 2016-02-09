#version 420

attribute vec3 position;

uniform mat4 viewMatrix;
uniform mat4 perspectiveMatrix;

void main(){
	gl_Position = (perspectiveMatrix * viewMatrix) * vec4(position, 1.0);
}