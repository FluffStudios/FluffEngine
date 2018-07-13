#version 330 core

layout (location = 0) in vec4 position;
out vec2 texture_coords;

uniform mat4 proj = mat4(1.0);

void main()
{
    gl_Position = proj * vec4(position.xy, 0.0, 1.0);
    texture_coords = position.zw;
}