#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;

uniform mat4 modelViewProjectionMatrix;
uniform vec3 color;

out vec2 interpTexCoord;
out vec3 outColor;

void main()
{
	outColor = color;
	interpTexCoord = vertexTexCoord;
	gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
}
