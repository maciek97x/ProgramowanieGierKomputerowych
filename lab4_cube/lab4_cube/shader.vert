#version 430 core

layout(location = 0) in vec3 vertexPosition;

uniform mat4 viewProjectionMatrix;

out vec3 interpNormal;

void main()
{
	gl_Position = viewProjectionMatrix * vec4(vertexPosition, 1.0);
	interpNormal = vertexPosition;
}
