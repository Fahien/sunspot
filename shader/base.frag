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

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Camera {
	vec3 position;
};

uniform Light light; 
uniform Camera camera;
uniform Material material;

in vec3 position;
in vec3 normal;
in vec2 texCoords;

out vec4 color;

void main()
{
	vec2 coords = vec2(one - texCoords.x, texCoords.y);
	vec3 sNormal = normalize(normal);
	vec3 fragment = material.hasDiffuseMap ? vec3(texture(material.diffuseMap, coords)) : vec3(one);

	vec3 ambient = light.ambient * material.ambient * fragment;

	vec3 lightDirection = normalize(light.position - position);
	float diffuseFactor = max(dot(sNormal, lightDirection), zero);
	vec3 diffuse = diffuseFactor * light.diffuse * material.diffuse * fragment;

	vec3 cameraDirection = normalize(camera.position - position);
	vec3 reflectDirection = reflect(-lightDirection, sNormal);
	float specularFactor = pow(max(dot(cameraDirection, reflectDirection), zero), material.shininess);
	vec3 specular = material.hasSpecularMap ? vec3(texture(material.specularMap, coords)) : vec3(one);
	specular *= specularFactor * light.specular * material.specular;

	color = vec4(ambient + diffuse + specular, one);
}
