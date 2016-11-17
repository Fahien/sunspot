#version 330 core

const float zero = 0.0f;
const float one = 1.0f;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
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
	vec3 fragment = vec3(texture(material.diffuse, texCoords));
	vec3 ambient = light.ambient * fragment;

	vec3 lightDirection = normalize(light.position - position);
	float diffuseFactor = max(dot(normal, lightDirection), zero);
	vec3 diffuse = diffuseFactor * light.diffuse * fragment;

	vec3 cameraDirection = normalize(camera.position - position);
	vec3 reflectDirection = reflect(-lightDirection, normal); 
	float specularFactor = pow(max(dot(cameraDirection, reflectDirection), zero), material.shininess);
	vec3 specular = specularFactor * light.specular * vec3(texture(material.specular, texCoords));

	color = vec4(ambient + diffuse + specular, one);
}