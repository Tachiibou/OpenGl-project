#version 430
layout(rgba32f) uniform image2D destTex;
layout (local_size_x = 16, local_size_y = 16) in;
uniform float number;
uniform vec3 colorVector;

void main() {
	ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
	float globalCoef = sin(float(gl_WorkGroupID.x + gl_WorkGroupID.y) * number);
	vec4 color = imageLoad(destTex,storePos) + vec4(0,0,1,1);
	//vec4 color = vec4(0,0,0,0);
	imageStore(destTex, storePos,  color );
	memoryBarrier();
}