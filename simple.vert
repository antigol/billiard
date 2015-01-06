#version 130
in vec3 vertex_position;
in vec3 vertex_normal;

uniform mat4 MVP;
uniform mat3 Mn;
uniform mat4 DepthBiasMVP;

out vec3 n_V;
out vec3 n_M;
out vec3 e_V;
out vec4 shc;

void main(void)
{
    n_M = vertex_normal;
    n_V = Mn * vertex_normal;
    shc = DepthBiasMVP * vec4(vertex_position, 1);

    gl_Position = MVP * vec4(vertex_position, 1.);
}
