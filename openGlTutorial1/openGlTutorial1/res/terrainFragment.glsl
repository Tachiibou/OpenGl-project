#version 430

layout(location = 0) out vec3 gPos;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gColor;

in vec3 fragPos;
in vec2 fragUv;
in vec3 fragNormal;
in vec3 fragLightPos; // Line 10
in float fragmentHeightPercentage;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D depth;

const float highMax = .7f;

vec3 calulateBlend(){
	float amount = clamp(fragmentHeightPercentage / highMax, 0, 1);

	vec3 firstColor = (1-amount) * vec3(texture2D(texture0, fragUv)); //Lower
	vec3 secondColor = amount * vec3(texture2D(texture1, fragUv));    //Upper

	return firstColor + secondColor;
}

void main()
{
	float bias = 0.005;
	float visibility = 1.0;
	if(texture2D(depth, fragLightPos.xy).z < fragLightPos.z - bias)
		visibility = 0.5;
	// Line 20
	gPos = clamp(fragPos,0.0,1.0);
	gNormal = clamp(normalize(fragNormal),0.0,1.0);
	//gColor = clamp(vec3(texture2D(texture, fragUv)) + vec3(texture2D(depth, fragLightPos.xy)),0.0,1.0);
	//gColor = gNormal;


	gColor = clamp ( calulateBlend()* visibility, 0.0,1.0); //Final Color
	//gColor = clamp(mix(vec3(texture2D(texture0, fragUv)), vec3(texture2D(texture1, fragUv)),fragmentHeightPercentage - (1-highMax))  * visibility, 0.0,1.0);
}