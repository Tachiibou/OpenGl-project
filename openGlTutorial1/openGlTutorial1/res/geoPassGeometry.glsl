#version 420 

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform vec3 viewPos;

in vec3 VSpos[];
in vec2 VSuv[];
in vec3 VSnormal[];
in vec4 VStransformedPos[];
in vec3 VLightPos[];
in vec3 VSpecular[];

out vec3 fragPos;
out vec2 fragUv;
out vec3 fragNormal;
out vec3 fragLightPos;
out vec3 fragSpecular;

void main()
{
	vec3 v1 = VSpos[1] - VSpos[0];
	vec3 v2 = VSpos[2] - VSpos[0];
	vec3 finalNor = normalize(cross(v1,v2));

	vec3 posToView = normalize(viewPos - VSpos[0]);
	float test = dot(posToView, finalNor);
	
	if(test >= 0.0)
	{
		for(int i = 0; i < 3; i++)
		{
			fragPos = VSpos[i];
			gl_Position = gl_in[i].gl_Position;
			fragUv = VSuv[i];
			fragNormal = VSnormal[i];
			//fragPos = vec3(VStransformedPos[i]);
			fragLightPos = VLightPos[i];
			fragSpecular = VSpecular[i];
			EmitVertex();
		}
		EndPrimitive();
	}
}
