#version 130
in vec3 n_V;
in vec3 n_M;

uniform vec3 l;

out vec3 color;

void main(void)
{
    float pi = 3.1415926535897932384626433832795;

    vec3 nv = normalize(n_V);
    vec3 nm = normalize(n_M);

    vec3 c = vec3(1,1,1);

    if (abs(nm.x) < 0.2 || abs(nm.y) < 0.2 || abs(nm.z) < 0.2) {
        float x = min(min(abs(nm.x), abs(nm.y)), abs(nm.z)) / 0.2;
        float f = 1. / (exp(100*(x - .9)) + 1.);
        c = vec3(1,0,0) * f + (1.-f) * c;
    }
    color = clamp(dot(nv, l), 0.1, 1) * c;
}
