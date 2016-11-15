#version 330 core

const float one = 1.0f;
const float ambientStrength = 0.125f;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light; 

in vec3 position; 
in vec3 normal;
in vec2 texCoords;

out vec4 color;

uniform Material theMaterial;
uniform Material theLight;
uniform sampler2D theTexture;

void main()
{
	vec3 ambient = ambientStrength * light.ambient;

	vec3 lightDirection = normalize(light.position - position);
	float diffuseStrength = max(dot(normal, lightDirection), 0.0f);
	vec3 diffuse = diffuseStrength * light.diffuse;
 
	vec4 light = vec4(ambient + diffuse, 1.0f);
	color = light * texture(theTexture, texCoords);
}