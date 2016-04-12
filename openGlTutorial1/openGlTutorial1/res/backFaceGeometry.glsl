#version 420 

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 viewMatrix;
uniform vec3 viewPos;

in vec3 VSpos[];
in vec2 VSuv[];
in vec3 VSnormal[];
in vec4 VStransformedPos[];

out vec2 GSuv;
out vec4 GSposition;
out vec3 GSnormal;

void main()
{
	vec3 v1 = VSpos[1] - VSpos[0];
	vec3 v2 = VSpos[2] - VSpos[0];
	vec3 finalNor = cross(v1,v2);

	vec3 posToView = normalize(viewPos - VSpos[0]);
	float test = dot(posToView, finalNor);
	
	if(test >= 0.0)
	{
		for(int i = 0; i < 3; i++)
		{
			gl_Position = gl_in[i].gl_Position;
			GSuv = VSuv[i];
			GSnormal = VSnormal[i];
			GSposition = VStransformedPos[i];
			EmitVertex();
		}
		EndPrimitive();
	}
}