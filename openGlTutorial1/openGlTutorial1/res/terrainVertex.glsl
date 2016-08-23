#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in float heightPercentage;

out vec3 VSPos;
out vec2 VSUv;
out vec3 VSNormal;
out vec3 VSLightPos;
out float VSHeightPercentage;

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
	
	VSPos = vec3(worldMatrix * vec4(position, 1));
	VSUv = uv;
	VSNormal = normal;
	VSLightPos = vec3(bias * lightPerspectiveMatrix * lightViewMatrix * worldMatrix * vec4(position,1));

	VSHeightPercentage = heightPercentage;
	//fragLightPos = vec3(lightPerspectiveMatrix * lightViewMatrix * vec4(position,1));
}