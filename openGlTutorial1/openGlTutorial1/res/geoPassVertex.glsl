#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 specular;

out vec3 VSpos;
out vec2 VSuv;
out vec3 VSnormal;
out vec3 VLightPos;
out vec3 VSpecular;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 perspectiveMatrix;
uniform mat4 lightViewMatrix;
uniform mat4 lightPerspectiveMatrix;


void main()
{
	VSpecular = specular;

	gl_Position = perspectiveMatrix * viewMatrix * worldMatrix * vec4(position, 1);
	
	VSpos = vec3(worldMatrix * vec4(position, 1));
	VSuv = uv;
	VSnormal = normal;
	VLightPos = vec3(lightPerspectiveMatrix * lightViewMatrix * worldMatrix * vec4(position,1)); // calculate the position from the lights point of view
}