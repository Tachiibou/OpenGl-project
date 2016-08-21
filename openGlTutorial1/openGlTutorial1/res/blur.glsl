#version 430
layout(rgba32f) uniform image2D destTex;
layout (local_size_x = 16, local_size_y = 16) in;
uniform float number;
uniform vec3 colorVector;

void main() {
	ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);

		vec4 color1;
		vec4 color2;
		vec4 color3;
		vec4 color4;
		vec4 color5;
		vec4 color6;
		vec4 color7;
		vec4 color8;
		vec4 color9;

		vec4 final;


	if(storePos.x == 0) // if we are at the left corner, ignore the pixels outside the screen
	{
		//color1 = imageLoad(destTex,storePos + ivec2(-1,-1)) / 16;
		color2 = imageLoad(destTex,storePos + ivec2(0,-1)) / 6;
		color3 = imageLoad(destTex,storePos + ivec2(1,-1)) / 12;
		//color4 = imageLoad(destTex,storePos + ivec2(-1,0)) / 8;
		color5 = imageLoad(destTex,storePos + ivec2(0,0)) / 3;
		color6 = imageLoad(destTex,storePos + ivec2(1,0))/ 6;
		//color7 = imageLoad(destTex,storePos + ivec2(-1,1)) / 16;
		color8 = imageLoad(destTex,storePos + ivec2(0,1)) / 6;
		color9 = imageLoad(destTex,storePos + ivec2(1,1)) / 12;

		final = clamp(color2 + color3 + color5 + color6 + color8 + color9,0.0,1.0);
	}

	else if(storePos.y == 0) // bottom of the screen
	{
		//color1 = imageLoad(destTex,storePos + ivec2(-1,-1)) / 16;
		//color2 = imageLoad(destTex,storePos + ivec2(0,-1)) / 8;
		//color3 = imageLoad(destTex,storePos + ivec2(1,-1)) / 16;
		color4 = imageLoad(destTex,storePos + ivec2(-1,0)) / 6;
		color5 = imageLoad(destTex,storePos + ivec2(0,0)) / 3;
		color6 = imageLoad(destTex,storePos + ivec2(1,0))/ 6;
		color7 = imageLoad(destTex,storePos + ivec2(-1,1)) / 12;
		color8 = imageLoad(destTex,storePos + ivec2(0,1)) / 6;
		color9 = imageLoad(destTex,storePos + ivec2(1,1)) / 12;

		final = clamp(color4 + color5 + color6 + color7 + color8 + color9,0.0,1.0);
	}
	else
	{
		color1 = imageLoad(destTex,storePos + ivec2(-1,-1)) / 16;
		color2 = imageLoad(destTex,storePos + ivec2(0,-1)) / 8;
		color3 = imageLoad(destTex,storePos + ivec2(1,-1)) / 16;
		color4 = imageLoad(destTex,storePos + ivec2(-1,0)) / 8;
		color5 = imageLoad(destTex,storePos + ivec2(0,0)) / 4;
		color6 = imageLoad(destTex,storePos + ivec2(1,0))/ 8;
		color7 = imageLoad(destTex,storePos + ivec2(-1,1)) / 16;
		color8 = imageLoad(destTex,storePos + ivec2(0,1)) / 8;
		color9 = imageLoad(destTex,storePos + ivec2(1,1)) / 16;

		final = clamp(color1 + color2 + color3 + color4 + color5 + color6 + color7 + color8 + color9,0.0,1.0);
		
	}

	


	//memoryBarrier(); // wait for all to read texture before overwriting the texture
	
	imageStore(destTex, storePos,  final );
	
}
