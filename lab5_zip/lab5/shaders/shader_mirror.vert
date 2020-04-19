#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

out vec3 Normal;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;

void main()
{
    Normal = mat3(transpose(inverse(model))) * vertexNormal;
    Position = vec3(model * vec4(vertexPosition, 1.0));
    gl_Position = projection * view * vec4(Position, 1.0);
} 