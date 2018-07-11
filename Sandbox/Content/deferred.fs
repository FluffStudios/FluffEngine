#version 440 core

#include <resources/functions.head>
#define MAX_SCENE_LIGHTS 32

in vec3 pass_uv;

out vec3 final_color;

uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D depth_map;

uniform vec3 camera_position;

layout (std140, binding = 0) uniform ProjView
{
	mat4 proj;
	mat4 view;
};

layout (std140, binding = 1) uniform SceneLighting
{
	ivec3 LightCounts[4];
	DirectionalLight D_Light;
	PointLight P_Light[32];
	SpotLight S_Light[32];
};

void main (void) 
{
	vec3 position_vec = texture(position, pass_uv.xy).rgb;
    vec3 n_vec = texture(normal, pass_uv.xy).rgb;
    vec3 diffuse_col = texture(diffuse, pass_uv.xy).rgb;
    vec3 specular_col = texture(specular, pass_uv.xy).rgb;

//	final_color = directional_lighting(D_Light, diffuse_col, specular_col, position_vec, n_vec, camera_position, 0.2);
	for (int i = 0; i < LightCounts[0].y; i++)
	{
		final_color += point_lighting(P_Light[i], diffuse_col, specular_col, position_vec, n_vec, camera_position, 0.2);
	}
	gl_FragDepth = texture(depth_map, pass_uv.xy).r;
}