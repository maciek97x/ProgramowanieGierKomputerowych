#version 430 core

in vec3 interpNormal;

void main()
{
	vec3 normal = normalize(interpNormal);
	gl_FragColor = vec4((normal + 1)/2, 1.0);
}
