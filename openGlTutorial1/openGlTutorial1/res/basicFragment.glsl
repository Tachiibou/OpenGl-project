#version 430

uniform sampler2D texture;
layout(location = 0)out color;

in vec2 fragment_uv;
in vec3 fragment_normal;

void main(){
	float ambient = .2f;
	vec3 lightNor = vec3(1, 1, -0);
	lightNor= normalize(lightNor);
	vec3 tempNorm = normalize(fragment_normal);
	float lightStrenght = clamp(dot(tempNorm,lightNor) + ambient, ambient, 1);
	gl_FragColor = texture2D(texture, fragment_uv) * lightStrenght;
	//gl_FragColor = vec4(fragment_normal, 1);
}