#version 420

uniform sampler2D texture;

in vec2 fragment_uv;
in vec3 fragment_normal;

void main(){
	gl_FragColor = texture2D(texture, fragment_uv);
}