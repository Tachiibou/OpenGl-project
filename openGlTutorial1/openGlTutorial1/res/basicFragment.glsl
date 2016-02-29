#version 420

in vec2 fragment_uv;
in vec3 fragment_normal;

void main(){
	gl_FragColor = vec4(fragment_uv, 0,1.0);
}