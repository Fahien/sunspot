#version 330 core

const float zero = 0.0f;
const float one = 1.0f;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	bool hasDiffuseMap;
	sampler2D diffuseMap;
	bool hasSpecularMap;
	sampler2D specularMap;
};

struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Fragment {
	vec3 position;
	vec3 normal;
	vec2 coords;
	vec3 color;
};

struct Camera {
	vec3 position;
};

uniform bool pointLightActive;
uniform PointLight pLight;

uniform bool directionalLightActive;
uniform DirectionalLight dLight;

uniform Camera camera;
uniform Material material;

in vec3 position;
in vec3 normal;
in vec2 texCoords;

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
	float diffuseFactor = max(dot(normal, lightDirection), zero);
	vec3 diffuse = diffuseFactor * light.diffuse * material.diffuse * fragment.color;

	vec3 cameraDirection = normalize(camera.position - fragment.position);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specularFactor = pow(max(dot(cameraDirection, reflectDirection), zero), material.shininess);
	vec3 specular = material.hasSpecularMap ? vec3(texture(material.specularMap, fragment.coords)) : vec3(one);
	specular *= specularFactor * light.specular * material.specular;

	fragment.color = ambient + diffuse + specular;
}


void main()
{
	Fragment fragment;
	fragment.position = position;
	fragment.normal   = normalize(normal);
	fragment.coords   = vec2(one - texCoords.x, texCoords.y);
	fragment.color    = material.hasDiffuseMap ? vec3(texture(material.diffuseMap, fragment.coords)) : vec3(one);

//	if (pointLightActive)
//	{
//		applyPointLight(pLight, camera, fragment);
//	}

//	if (directionalLightActive)
//	{
		applyDirectionalLight(dLight, camera, fragment);
//	}

	color = vec4(fragment.color, one);
}
