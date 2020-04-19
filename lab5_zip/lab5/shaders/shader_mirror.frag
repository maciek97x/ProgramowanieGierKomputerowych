#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{             
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));

	float reflectivity = 1 - abs(dot(I, Normal));
	reflectivity = clamp(reflectivity, 0, 1)/2 + 0.5;
	FragColor = mix(vec4(0.7, 0.7, 0.7, 1.0), vec4(texture(skybox, R).rgb, 1.0), reflectivity);
}