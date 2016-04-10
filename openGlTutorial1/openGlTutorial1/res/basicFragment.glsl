#version 420

uniform sampler2D texture;

in vec2 fragment_uv;
in vec3 fragment_normal;

void main(){
	vec3 lightNor = vec3(1, 1, 0);
	lightNor= normalize(lightNor);
	vec3 tempNorm = normalize(fragment_normal);
	float lightStrenght = max(dot(tempNorm,lightNor),0);
	gl_FragColor = texture2D(texture, fragment_uv) * lightStrenght;
}