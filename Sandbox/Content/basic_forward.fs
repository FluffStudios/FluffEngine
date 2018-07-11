#version 420 core

in VS_OUT
{
	vec3 position;
	vec2 texCoords;
	vec4 normals;
} fs_in;

layout (location = 0) out vec4 finalColor;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D ambientOcclusionMap;

void main()
{
	finalColor = texture(albedoMap, fs_in.texCoords);
	if (finalColor.a > 0.5) break;
}
