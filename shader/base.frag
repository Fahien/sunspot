#version 330 core

const float zero = 0.0f;
const float one  = 1.0f;
const float pi   = 3.14159265359f;

struct Vertex
{
	bool hasColor;
};

struct Material
{
	vec3 color;
	bool hasColorTexture;
	sampler2D colorTexture;
	float metallic;
	float roughness;
	float ambientOcclusion;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	bool hasDiffuseMap;
	sampler2D diffuseMap;
	bool hasSpecularMap;
	sampler2D specularMap;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Fragment
{
	vec3 position;
	vec3 normal;
	vec2 coords;
	bool hasVertexColor;
	vec3 color;
};

struct Camera
{
	vec3 position;
};

uniform bool pointLightActive;
uniform PointLight pLight;

uniform bool directionalLightActive;
uniform DirectionalLight dLight;

uniform Camera camera;
uniform Material material;
uniform Vertex vertex;

in vec3 position;
in vec3 normal;
in vec2 texCoords;
in vec4 vertColor;

out vec4 color;


void applyDirectionalLight(in DirectionalLight light, in Camera camera, inout Fragment fragment)
{
	vec3 ambient = light.ambient * material.ambient * fragment.color;

	vec3 lightDirection = normalize(-light.direction);
	float diffuseFactor = max(dot(fragment.normal, lightDirection), zero);
	vec3 diffuse = diffuseFactor * light.diffuse * material.diffuse * fragment.color;

	vec3 cameraDirection = normalize(camera.position - fragment.position);
	vec3 reflectDirection = reflect(-lightDirection, fragment.normal);
	float specularFactor = pow(max(dot(cameraDirection, reflectDirection), zero), material.shininess);
	vec3 specular = material.hasSpecularMap ? vec3(texture(material.specularMap, fragment.coords)) : vec3(one);
	specular *= specularFactor * light.specular * material.specular;

	fragment.color = ambient + diffuse + specular;
}


void applyPointLight(in PointLight light, in Camera camera, inout Fragment fragment)
{
	vec3 ambient = light.ambient * material.ambient * fragment.color;

	vec3 lightDirection = normalize(light.position - fragment.position);
	float diffuseFactor = max(dot(fragment.normal, lightDirection), zero);
	vec3 diffuse = diffuseFactor * light.diffuse * material.diffuse * fragment.color;

	vec3 cameraDirection = normalize(camera.position - fragment.position);
	vec3 reflectDirection = reflect(-lightDirection, fragment.normal);
	float specularFactor = pow(max(dot(cameraDirection, reflectDirection), zero), material.shininess);
	vec3 specular = material.hasSpecularMap ? vec3(texture(material.specularMap, fragment.coords)) : vec3(one);
	specular *= specularFactor * light.specular * material.specular;

	fragment.color = ambient + diffuse + specular;
}


float calculateDistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a      = roughness * roughness;
	float a2     = a * a;
	float NdotH  = max(dot(N, H), zero);
	float NdotH2 = NdotH * NdotH;

	float num   = a2;
	float denom = (NdotH2 * (a2 - one) + one);
	denom = pi * denom * denom;

	return num / denom;
}


float calculateGeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + one);
	float k = (r * r) / 8.0f;

	float numerator   = NdotV;
	float denominator = NdotV * (one - k) + k;

	return numerator / denominator;
}


float calculateGeometrySmith(vec3 fragmentNormal, vec3 cameraDirection, vec3 lightDirection, float roughness)
{
	float NdotV = max(dot(fragmentNormal, cameraDirection), zero);
	float NdotL = max(dot(fragmentNormal, lightDirection), zero);
	float ggx2  = calculateGeometrySchlickGGX(NdotV, roughness);
	float ggx1  = calculateGeometrySchlickGGX(NdotL, roughness);
	
	return ggx1 * ggx2;
}


vec3 calculateFresnelSchlick(float cosTheta, vec3 f0)
{
	return f0 + (one - f0) * pow(one - cosTheta, 5.0f);
}


void applyPBRPointLight(in PointLight light, in Camera camera, inout Fragment fragment)
{
	vec3  lightVector      = light.position - fragment.position;
	float lightDistance    = length(lightVector);
	float lightAttenuation = one / (lightDistance * lightDistance / 64.0f);
	vec3  lightDirection   = normalize(lightVector);

	float diffuseFactor = max(dot(fragment.normal, lightDirection), zero);
	vec3 radiance = lightAttenuation * light.diffuse;
	vec3 diffuse  = lightAttenuation * diffuseFactor * light.diffuse * material.color * fragment.color;

	vec3 cameraVector    = camera.position - fragment.position;
	vec3 cameraDirection = normalize(cameraVector);

	vec3 halfVector  = normalize(cameraVector + lightVector);
	vec3 lightOutput = vec3(zero);

	vec3 f0 = vec3(0.04f);
	f0 = mix(f0, fragment.color, material.metallic);

	float cosTheta = max(dot(cameraDirection, lightDirection), zero);
	vec3 fresnelSchlick = calculateFresnelSchlick(cosTheta, f0);

	float normalDistributionFactor = calculateDistributionGGX(fragment.normal, halfVector, material.roughness);
	float geometrySmith = calculateGeometrySmith(fragment.normal, cameraDirection, lightDirection, material.roughness);

	vec3 numerator = normalDistributionFactor * geometrySmith * fresnelSchlick;
	float denominator = 4.0 * max(dot(fragment.normal, cameraDirection), zero) * diffuseFactor;

	vec3 specular = numerator / max(denominator, one / 1024.0f);

	vec3 kS = fresnelSchlick;
	vec3 kD = vec3(one) - kS;
	kD *= one - material.metallic;

	lightOutput += (kD * fragment.color / pi + specular) * radiance * diffuseFactor;

	vec3 ambient = vec3(0.03f) * fragment.color * material.ambientOcclusion;
	vec3 color = ambient + lightOutput;
	color = color / (color + vec3(one));
	color = pow(color, vec3(one / 2.2f));

	fragment.color = color;
}


void main()
{
	Fragment fragment;
	fragment.position = position;
	fragment.normal   = normalize(normal);
	fragment.coords   = texCoords;
	fragment.color    = material.hasColorTexture ? texture(material.colorTexture, fragment.coords).rgb : material.color;
	if (vertex.hasColor)
	{
		fragment.color *= vertColor.rgb;
	}

	applyPBRPointLight(pLight, camera, fragment);

	color = vec4(fragment.color, vertColor.a);
}
