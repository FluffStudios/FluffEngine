#version 420 core

#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS	 16

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 textureCoords;
layout (location = 2) in vec3 normals;
layout (location = 3) in vec3 tangents;
layout (location = 4) in vec3 bitangents;
layout (location = 5) in vec4 diffuse;
layout (location = 6) in vec4 specular;

layout (std140, binding = 0) uniform ProjView
{
	mat4 proj;
	mat4 view;
};

out VS_OUT
{
	vec3 position;
	vec2 texCoords;
	vec4 normals;
} vs_out;

uniform mat4 modl;

void main()
{
	gl_Position = proj * view * modl * vec4(position, 1.0);
	vs_out.position = gl_Position.xyz;
	vs_out.texCoords = textureCoords.xy;
	vs_out.normals = vec4(normals, 1.0);
}
