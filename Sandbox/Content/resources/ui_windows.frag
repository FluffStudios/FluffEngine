#version 440 core

in VS_OUT
{
	vec2 FRAG_UV;
	vec4 FRAG_COLOR;
} vs_out;

precision mediump float;
uniform sampler2D Texture;

out vec4 Out_Color;

void main(void)
{
	Out_Color = vs_out.FRAG_COLOR * texture(Texture, vs_out.FRAG_UV);
}