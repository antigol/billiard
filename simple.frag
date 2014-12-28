#version 130
in vec3 n;

uniform vec3 l;

out vec3 color;

void main(void)
{
    color = clamp(abs(dot(n, l)), 0.1, 1) * vec3(1, 1, 1);
}
