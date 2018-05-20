#version 440 core

layout (location = 0) in vec2 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec4 Color;

uniform mat4 ProjMtx;

out VS_OUT
{
	vec2 FRAG_UV;
	vec4 FRAG_COLOR;
} vs_out;

void main(void)
{
	vs_out.FRAG_UV = TexCoord;
	vs_out.FRAG_COLOR = Color;
	gl_Position = ProjMtx * vec4(Position.xy, 0, 1);
}