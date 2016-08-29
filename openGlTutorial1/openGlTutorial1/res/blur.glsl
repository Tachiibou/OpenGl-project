#version 430
layout(rgba32f) uniform image2D colorTexture;
layout (local_size_x = 16, local_size_y = 16) in;

void main() {
	ivec2 storePos = ivec2(gl_GlobalInvocationID.xy); // The pixel position this compute shader works with

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
		//color1 = imageLoad(colorTexture,storePos + ivec2(-1,-1)) / 16;
		color2 = imageLoad(colorTexture,storePos + ivec2(0,-1)) / 6;
		color3 = imageLoad(colorTexture,storePos + ivec2(1,-1)) / 12;
		//color4 = imageLoad(colorTexture,storePos + ivec2(-1,0)) / 8;
		color5 = imageLoad(colorTexture,storePos + ivec2(0,0)) / 3;
		color6 = imageLoad(colorTexture,storePos + ivec2(1,0))/ 6;
		//color7 = imageLoad(colorTexture,storePos + ivec2(-1,1)) / 16;
		color8 = imageLoad(colorTexture,storePos + ivec2(0,1)) / 6;
		color9 = imageLoad(colorTexture,storePos + ivec2(1,1)) / 12;

		final = clamp(color2 + color3 + color5 + color6 + color8 + color9,0.0,1.0);
	}

	else if(storePos.y == 0) // bottom of the screen, ignore pixels outside of screen
	{
		//color1 = imageLoad(colorTexture,storePos + ivec2(-1,-1)) / 16;
		//color2 = imageLoad(colorTexture,storePos + ivec2(0,-1)) / 8;
		//color3 = imageLoad(colorTexture,storePos + ivec2(1,-1)) / 16;
		color4 = imageLoad(colorTexture,storePos + ivec2(-1,0)) / 6;
		color5 = imageLoad(colorTexture,storePos + ivec2(0,0)) / 3;
		color6 = imageLoad(colorTexture,storePos + ivec2(1,0))/ 6;
		color7 = imageLoad(colorTexture,storePos + ivec2(-1,1)) / 12;
		color8 = imageLoad(colorTexture,storePos + ivec2(0,1)) / 6;
		color9 = imageLoad(colorTexture,storePos + ivec2(1,1)) / 12;

		final = clamp(color4 + color5 + color6 + color7 + color8 + color9,0.0,1.0);
	}

	else // If we are not in any corner dont ignore any pixel
	{
		color1 = imageLoad(colorTexture,storePos + ivec2(-1,-1)) / 16;
		color2 = imageLoad(colorTexture,storePos + ivec2(0,-1)) / 8;
		color3 = imageLoad(colorTexture,storePos + ivec2(1,-1)) / 16;
		color4 = imageLoad(colorTexture,storePos + ivec2(-1,0)) / 8;
		color5 = imageLoad(colorTexture,storePos + ivec2(0,0)) / 4;
		color6 = imageLoad(colorTexture,storePos + ivec2(1,0))/ 8;
		color7 = imageLoad(colorTexture,storePos + ivec2(-1,1)) / 16;
		color8 = imageLoad(colorTexture,storePos + ivec2(0,1)) / 8;
		color9 = imageLoad(colorTexture,storePos + ivec2(1,1)) / 16;

		final = clamp(color1 + color2 + color3 + color4 + color5 + color6 + color7 + color8 + color9,0.0,1.0);
		
	}

	


	memoryBarrier(); // wait for all to read texture before overwriting the texture
	
	imageStore(colorTexture, storePos,  final );
	
}
