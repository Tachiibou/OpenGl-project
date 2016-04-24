#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

out vec3 fragPos;
out vec2 fragUv;
out vec3 fragNormal;
out vec3 fragLightPos;

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

	gl_Position = perspectiveMatrix * viewMatrix * vec4(position, 1);
	
	fragPos = position;
	fragUv = uv;
	fragNormal = normal;
	fragLightPos = vec3(bias * lightPerspectiveMatrix * lightViewMatrix * vec4(position,1));
}