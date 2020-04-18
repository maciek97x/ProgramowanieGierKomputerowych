#version 330 core
#define M_PI 3.1415926535897932384626433832795
#define N 100

layout (points) in;
layout (triangle_strip, max_vertices = N) out;

in VS_OUT {
    vec3 color;
} gs_in[]; 

out vec3 fColor; 

void build_house(vec4 position)
{   
	for (int i = 1; i <= N; ++i) {
		if (i % 2 == 0) {
			gl_Position = position + vec4(cos(i*2*M_PI/(N - 2))/10, sin(i*2*M_PI/(N - 2))/10, 0.0, 0.0);
			fColor = gs_in[0].color;
		}
		else {
			gl_Position = position + vec4(cos(i*2*M_PI/(N - 2))/5, sin(i*2*M_PI/(N - 2))/5, 0.0, 0.0);
			fColor = vec3(1.0, 1.0, 1.0);
		}
		EmitVertex();   
	}
	EndPrimitive(); 
}

void main() {    
    build_house(gl_in[0].gl_Position);
}  