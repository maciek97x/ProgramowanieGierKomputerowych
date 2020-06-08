#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec3 vertexTexCoord;

out vec3 lightsColor;
out vec2 texCoords;
out float brightness;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 directionalLight;
uniform int lightsCount;
uniform vec3 lightSourcesColors[32];
uniform vec3 lightSourcesPositions[32];

void main()
{
	lightsColor = vec3(0.0);
	for (int i = 0; i < lightsCount; ++i) {
		lightsColor += lightSourcesColors[i];
	}
	brightness = (dot(normalize(mat3(model) * vertexNormal), directionalLight) + 1)/2;
    texCoords = vec2(vertexTexCoord);
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0); 
}