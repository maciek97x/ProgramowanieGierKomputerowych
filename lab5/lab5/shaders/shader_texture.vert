#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec3 vertexTexCoord;

out vec3 lightColor;
out vec2 texCoords;
out float brightness;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 directionalLight;
uniform float lightsCount;
uniform vec3 lightSourcesColors[32];
uniform vec3 lightSourcesPositions[32];

void main()
{
	lightColor = vec3(0.0);
	for (int i = 0; i < lightsCount; ++i) {
		float d = dot(normalize(mat3(model) * vertexNormal), normalize(lightSourcesPositions[i] - vertexPosition));
		if (d > 0) {
			lightColor += lightSourcesColors[i] * d / (1 + length(lightSourcesPositions[i] - vertexPosition) / 64);
		}
	}
	brightness = (dot(normalize(mat3(model) * vertexNormal), directionalLight) + 1)/2;
    texCoords = vec2(vertexTexCoord);
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0); 
}