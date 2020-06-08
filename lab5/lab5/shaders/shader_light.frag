#version 330 core

uniform vec3 lightSource;

in vec2 interpTexCoord;
in vec3 outColor;

void main()
{
	vec2 r = interpTexCoord - 0.5;
	float a = exp(-20 * dot(r,r));
	gl_FragColor = vec4(outColor, a);
}
