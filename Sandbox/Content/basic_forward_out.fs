#version 420 core

in VS_OUT
{
	vec3 position;
	vec3 uv;
	vec3 normal;
	flat mat4 transformation;
} fs_in;

layout (location = 0) out vec4 color;

uniform sampler2D albedoMap;
uniform vec3 base_color;

void main(void)
{
	color = texture(albedoMap, fs_in.uv.xy);
}

