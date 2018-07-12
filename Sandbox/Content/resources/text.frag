#version 330 core

in vec2 texture_coords;
out vec4 final_color;

uniform sampler2D character_map;
uniform vec3 text_color;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(character_map, texture_coords).r);
    final_color = vec4(text_color, 1.0) * sampled;
}