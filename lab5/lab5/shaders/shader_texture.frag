#version 330 core
out vec4 FragColor;

in vec3 lightColors;
in vec2 texCoords;
in float brightness;

uniform sampler2D tex;

void main()
{
    FragColor = vec4(vec3(texture(tex, texCoords))*brightness + lightColors, 1.0);
}