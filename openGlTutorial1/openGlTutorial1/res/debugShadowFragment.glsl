#version 430

in vec2 vertUV;
out vec4 color;

uniform sampler2D depthMap;
uniform float nearPlane;
uniform float farPlane;


float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // Back to NDC
	return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

void main()
{             
    float depthValue = texture2D(depthMap, vertUV).r;
    //color = vec4(vec3(LinearizeDepth(depthValue) / farPlane), 1.0); // perspective
	color = vec4(vec3(depthValue), 1.0); // orthographic
}  