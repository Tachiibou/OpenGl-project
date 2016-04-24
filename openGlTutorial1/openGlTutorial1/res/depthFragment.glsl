#version 430


layout(location = 0) out vec3 gDepth;

void main()
{

	gDepth = vec3(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z);
}