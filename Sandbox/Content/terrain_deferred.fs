#version 440 core

#include <resources/functions.head>

in vec3 pass_position;
in vec3 pass_uv;
in vec3 pass_normal;

layout (location = 0) out vec3 position;
layout (location = 1) out vec3 normal;
layout (location = 2) out vec4 diffuse;
layout (location = 3) out vec4 specular;

uniform sampler2D   albedoMap;
uniform sampler2D   specularMap;
uniform float       texture_scale;
uniform vec3       terrain_scale;

void main( void )
{
    position = pass_position;
    normal   = pass_normal;
    diffuse  = triplanar_texture(albedoMap, pass_position, normalize(pass_normal / terrain_scale), texture_scale);
    specular = triplanar_texture(specularMap, pass_position, normalize(pass_normal / terrain_scale), texture_scale);
}