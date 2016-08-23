#version 420 

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform vec3 viewPos;

in vec3 VSPos[];
in vec2 VSUv[];
in vec3 VSNormal[];
in vec3 VSLightPos[];
in float VSHeightPercentage[];

out vec3 fragPos;
out vec2 fragUv;
out vec3 fragNormal;
out vec3 fragLightPos;
out float fragmentHeightPercentage;

void main()
{
	vec3 v1 = VSPos[1] - VSPos[0];
	vec3 v2 = VSPos[2] - VSPos[0];
	vec3 finalNor = normalize(cross(v1,v2));

	vec3 posToView = normalize(viewPos - VSPos[0]);
	float test = dot(posToView, finalNor);
	
	if(test >= 0.0)
	{
		for(int i = 0; i < 3; i++)
		{
			fragPos = VSPos[i];
			gl_Position = gl_in[i].gl_Position;
			fragUv = VSUv[i];
			fragNormal = VSNormal[i];
			//fragPos = vec3(VStransformedPos[i]);
			fragLightPos = VSLightPos[i];
			fragmentHeightPercentage = VSHeightPercentage[i];
			EmitVertex();
		}
		EndPrimitive();
	}
}
