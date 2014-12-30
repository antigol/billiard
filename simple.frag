#version 130
in vec3 n_V;
in vec3 n_M;

uniform vec3 l;

uniform float lw;
uniform vec3 line_color;
uniform vec3 main_color;
uniform bool phong;

out vec3 color;

void main(void)
{
    float pi = 3.1415926535897932384626433832795;

    if (phong) {
        vec3 nv = normalize(n_V);
        vec3 nm = normalize(n_M);

        vec3 c = main_color;

        if (any(lessThan(abs(nm), vec3(lw)))) {
            float x = min(min(abs(nm.x), abs(nm.y)), abs(nm.z)) / lw;
            float f = 1. / (exp(100*(x - .9)) + 1.);
            c = line_color * f + (1.-f) * line_color;
        }
        color = clamp(dot(nv, l), 0.1, 1) * c;
    } else {
        color = main_color;
    }
}
