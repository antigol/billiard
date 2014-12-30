#version 130
in vec3 n_V;
in vec3 n_M;

uniform vec3 l;

out vec3 color;

void main(void)
{
    vec3 nv = normalize(n_V);
    vec3 nm = normalize(n_M);

    vec3 c = vec3(1,1,1);

    if (abs(nm.z) < 0.2 || abs(nm.x) < 0.2 || abs(nm.z) < 0.2) c = vec3(1,0,0);

    color = clamp(dot(nv, l), 0.1, 1) * c;
}
