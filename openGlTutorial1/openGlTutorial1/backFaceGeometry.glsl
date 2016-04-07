#version 420 

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out

in VertexVS
{
	vec2 geo_uv;
	vec3 geo_normal;
}vertex[];

out VertexGS
{
	vec2 geo_uv;
	vec3 geo_normal;
} outVertex;

int main()
{
	
}