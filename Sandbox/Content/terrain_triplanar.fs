#version 440 core

#include <resources/functions.head>

in VS_OUT
{
	vec3 position;
	vec3 worldPosition;
	vec3 uvs;
	vec3 normal;
	vec3 worldNormal;
	flat mat4 transformation;
} fs_in;

layout (location = 0) out vec4 color;

layout (location = 1) uniform sampler2D albedoMap;
layout (location = 2) uniform float textureScale;
layout (location = 3) uniform vec3 scale;
layout (location = 4) uniform vec3 light_dir;
layout (location = 5) uniform vec3 cameraPos;

void main()
{
	vec3 unitNormal = normalize(fs_in.normal / scale);
	color = triplanar_texture(albedoMap, fs_in.position, unitNormal, textureScale);

	vec3 suncolor = vec3(1);
	float sunintensity = 1.0;
	float reflectivity = 0.0;

	vec3 unitVectorToCamera = normalize(fs_in.worldPosition - cameraPos);
	float shineDamper = 0.1;

	vec3 unitLightVector = normalize(light_dir);	
	float nDotl = dot(unitNormal,unitLightVector);
	float brightness = max(nDotl,0.0);
	vec3 lightDirection = -unitLightVector;
	vec3 reflectedLightDirection = reflect(lightDirection,unitNormal);
	float specularFactor = dot(reflectedLightDirection , unitVectorToCamera);
	specularFactor = max(specularFactor,0.0);
	float dampedFactor = pow(specularFactor,shineDamper);
	vec3 totalDiffuse = (brightness * suncolor) * sunintensity + 0.1;
	vec3 totalSpecular = (dampedFactor * reflectivity * suncolor) * sunintensity;

	float lightFactor = 1;
	totalDiffuse = max(totalDiffuse * lightFactor, 0.2);

	color = vec4(totalDiffuse,1.0) * color + vec4(totalSpecular,1.0);
}