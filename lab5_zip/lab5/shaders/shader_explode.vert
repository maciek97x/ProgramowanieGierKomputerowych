#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec3 vertexTexCoord;

out VS_OUT {
    vec2 texCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vs_out.texCoords = vec2(vertexTexCoord);
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0); 
}