#version 440 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 uv;

out vec3 pass_uv;

layout (std140, binding = 0) uniform ProjView
{
	mat4 proj;
	mat4 view;
};

uniform mat4 modl;

void main( void )
{
    gl_Position = vec4(position, 1.0);
    pass_uv = uv;
}