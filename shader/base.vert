#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_TexCoords;

out vec3 position;
out vec3 normal;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 modelSpace = model * vec4(in_Position, 1.0);
	position = vec3(modelSpace);
	gl_Position = projection * view * modelSpace;
	normal = normalize(vec3(model * vec4(in_Normal, 1.0f)));
	texCoords = vec2(in_TexCoords.x, 1.0 - in_TexCoords.y);
}
