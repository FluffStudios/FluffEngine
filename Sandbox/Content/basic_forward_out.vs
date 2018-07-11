#version 420 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 uv;
layout (location = 2) in vec3 normal;

out VS_OUT
{
	vec3 position;
	vec3 uv;
	vec3 normal;
	flat mat4 transformation;
} fs_in;

layout (std140, binding = 0) uniform ProjView
{
	mat4 proj;
	mat4 view;
};

uniform mat4 modl;

void copy_to_output()
{
	fs_in.position = position;
	fs_in.uv = uv;
	fs_in.normal = normal;
	fs_in.transformation = proj * view * modl;
};

void main(void)
{
	gl_Position = proj * view * modl * vec4(position, 1.0);
	copy_to_output();
}