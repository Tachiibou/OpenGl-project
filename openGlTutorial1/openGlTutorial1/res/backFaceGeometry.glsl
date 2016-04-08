#version 420 

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 viewMatrix;
uniform vec3 viewPos;

in vec3 VSpos[];
in vec2 VSuv[];
in vec3 VSnormal[];

out vec2 GSuv;
out vec3 GSnormal;

void main()
{
	vec3 posToView = normalize(VSpos[0] - viewPos);
	float test = dot(-posToView, VSnormal[0]);
	
	if(test >= 0.0)
	{
		for(int i = 0; i < 3; i++)
		{
			gl_Position = gl_in[i].gl_Position;
			GSuv = VSuv[i];
			GSnormal = VSnormal[i];
			EmitVertex();
		}
		EndPrimitive();
	}
}