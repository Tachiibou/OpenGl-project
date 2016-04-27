#version 430
layout(rgba32f) uniform image2D destTex;
layout (local_size_x = 16, local_size_y = 16) in;
uniform float number;
uniform vec3 colorVector;

void main() {
	ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
	vec4 color1 = imageLoad(destTex,storePos + ivec2(-1,-1)) / 9;
	vec4 color2 = imageLoad(destTex,storePos + ivec2(0,-1)) / 9;
	vec4 color3 = imageLoad(destTex,storePos + ivec2(1,-1)) / 9;
	vec4 color4 = imageLoad(destTex,storePos + ivec2(-1,0)) / 9;
	vec4 color5 = imageLoad(destTex,storePos + ivec2(0,0)) / 9;
	vec4 color6 = imageLoad(destTex,storePos + ivec2(1,0))/ 9;
	vec4 color7 = imageLoad(destTex,storePos + ivec2(-1,1)) / 9;
	vec4 color8 = imageLoad(destTex,storePos + ivec2(0,1)) / 9;
	vec4 color9 = imageLoad(destTex,storePos + ivec2(1,1)) / 9;
	vec4 final = clamp(color1 + color2 + color3 + color4 + color5 + color6 + color7 + color8 + color9,0.0,1.0);


	memoryBarrier(); // wait for all to read texture before overwriting the texture
	
	imageStore(destTex, storePos,  final );
	
}
