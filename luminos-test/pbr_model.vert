#version 440 core

#define MAX_DIR_LIGHTS = 1;
#define MAX_POINT_LIGHTS = 5;
#define MAX_SPOT_LIGHTS = 5;

struct DirectionalLight
{
	vec4 Direction;		// first 3 components used
	vec4 Ambient;		// first 3 components used
	vec4 Diffuse;		// first 3 components used
	vec4 Specular;		// first 3 components used
};

struct PointLight
{
	vec4 Position;		// first 3 components used
	vec4 Attenuation;	// first 3 components used
	vec4 Ambient;		// first 3 components used
	vec4 Diffuse;		// first 3 components used
	vec4 Specular;		// first 3 components used
};

struct SpotLight
{
	float Cutoff;
	vec4  Position; 	// first 3 components used
	vec4  Direction;	// first 3 components used
	vec4  Ambient;		// first 3 components used
	vec4  Diffuse;		// first 3 components used
	vec4  Specular;		// first 3 components used
};

struct Material
{
	vec4 diffuse;						// first 3 components used
	vec4 specular;						// first 3 components used
	vec4 ReflectMetallicRoughness;		// first 3 components used
};

layout (std140, binding = 0) uniform ViewProj
{
	mat4 proj;	//	64		0
	mat4 view;	//  64		64
};

layout (std140, binding = 1) uniform Model
{
	mat4 modl;		//	64		0
	Material mat;	//	48		64
};

layout (std140, binding = 2) uniform SceneData
{
	DirectionalLight dirLights[MAX_DIR_LIGHTS];		// 64		0
	PointLight pointLights[MAX_POINT_LIGHTS];		// 80		64
	SpotLight spotLights[MAX_SPOT_LIGHTS];			// 84		144
};

out VS_OUT
{
	vec2 texCoords;
	DirectionalLight normalizedDirLights[MAX_DIR_LIGHTS];
	PointLight normalizedPointLights[MAX_POINT_LIGHTS];
	SpotLight normalizedSpotLights[MAX_SPOT_LIGHTS];
	Material material;
} vs_out;

void main()
{
	gl_Position = proj * view * modl * vec4(position, 1.0);
	vs_out.texCoords = textureCoords;
	// Do more vertex shader stuff
}