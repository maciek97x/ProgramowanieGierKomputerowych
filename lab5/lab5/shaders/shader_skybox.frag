#version 330 core

in vec3 skyboxCoord;

out vec4 outputColor;

uniform samplerCube skybox;


void main()
{
        outputColor = texture(skybox, skyboxCoord);
}