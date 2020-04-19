#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoords;
} gs_in[];

out vec2 TexCoords; 

uniform float time;


vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}


void main() {    
    vec3 normal = GetNormal();

	int i;

	for (i = 0; i < 3; ++i) {
		gl_Position = gl_in[i].gl_Position + normalize(2 * gl_in[i].gl_Position - (gl_in[(i + 1) % 3].gl_Position + gl_in[(i + 2) % 3].gl_Position)) * 4.0 * fract(time/2);
		TexCoords = gs_in[i].texCoords + normalize(2 * gs_in[i].texCoords - (gs_in[(i + 1) % 3].texCoords + gs_in[(i + 2) % 3].texCoords)) * 0.4 * fract(time/2);
		EmitVertex();
	}

    EndPrimitive();
}  