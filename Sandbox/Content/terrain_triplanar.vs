#version 440 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 uvs;
layout (location = 2) in vec3 normal;

out VS_OUT
{
	vec3 position;
	vec3 worldPosition;
	vec3 uvs;
	vec3 normal;
	vec3 worldNormal;
	flat mat4 transformation;
} fs_in;

layout (std140, binding = 0) uniform ProjView
{
	mat4 proj;
	mat4 view;
};

layout (location = 0) uniform mat4 modl;

void copy_to_output()
{
	fs_in.position = position;
	fs_in.worldPosition = (modl * vec4(position, 1.0)).xyz;
	fs_in.uvs = uvs;
	fs_in.normal = normal;
	fs_in.worldNormal = (modl * vec4(normal, 1.0)).xyz;
	fs_in.transformation = proj * view * modl;
};

void main(void)
{
	gl_Position = proj * view * modl * vec4(position, 1.0);
	copy_to_output();
}