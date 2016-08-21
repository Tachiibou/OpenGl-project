#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

out vec3 VSpos;
out vec2 VSuv;
out vec3 VSnormal;
out vec4 VStransformedPos;
out vec3 VLightPos;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 perspectiveMatrix;
uniform mat4 lightViewMatrix;
uniform mat4 lightPerspectiveMatrix;


void main()
{
	//vec4 worldPos = vec4(position, 1);
	mat4 bias = mat4(
				vec4(0.5,0,0,0),
				vec4(0,0.5,0,0),
				vec4(0,0,0.5,0),
				vec4(0.5,0.5,0.5,1)
				);

	gl_Position = perspectiveMatrix * viewMatrix * worldMatrix * vec4(position, 1);
	
	VSpos = vec3(worldMatrix * vec4(position, 1));
	VSuv = uv;
	VSnormal = normal;
	VLightPos = vec3(lightPerspectiveMatrix * lightViewMatrix * worldMatrix * vec4(position,1));
	VStransformedPos = viewMatrix*worldMatrix*vec4(position,1);
	//fragLightPos = vec3(lightPerspectiveMatrix * lightViewMatrix * vec4(position,1));
}