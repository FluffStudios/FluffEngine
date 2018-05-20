#version 440 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 uv;
layout (location = 2) in vec3 normal;

layout (std140, binding = 0) uniform ProjView
{
	mat4 proj;
	mat4 view;
};

uniform mat4 modl;

out vec3 pass_position;
out vec3 pass_uv;
out vec3 pass_normal;

void main( void )
{
    gl_Position = proj * view * modl * vec4(position, 1.0);
    pass_position = (modl * vec4(position, 1.0)).xyz;
    pass_uv = uv;
    pass_normal = transpose(inverse(mat3(modl))) * normal;
}