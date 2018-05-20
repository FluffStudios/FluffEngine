#version 440 core

in vec3 pass_position;
in vec3 pass_uv;
in vec3 pass_normal;

uniform sampler2D albedoMap;
uniform sampler2D specularMap;

layout (location = 0) out vec3 position;
layout (location = 1) out vec3 normal;
layout (location = 2) out vec4 diffuse;
layout (location = 3) out vec4 specular;

void main( void )
{
    position = pass_position;
    normal = pass_normal;
    diffuse = texture(albedoMap, pass_uv.xy);
    specular = texture(specularMap, pass_uv.xy);
}