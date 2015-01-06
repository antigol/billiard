#version 130
in vec3 n_V;
in vec3 n_M;
in vec4 shc;

uniform vec3 l;

uniform float lw;
uniform vec3 line_color;
uniform vec3 main_color;
uniform bool phong;

uniform sampler2DShadow shadowMap;

out vec3 color;

vec2 rand2(in vec4 seed)
{
    float x = dot(seed, vec4(12.9898,78.233,45.164,94.673));
    float y = dot(seed, vec4(22.9898,74.233,46.164,14.673));
    return vec2(fract(sin(x) * 43758.5453), fract(sin(y) * 33728.6453));
}

void main(void)
{
    float pi = 3.1415926535897932384626433832795;

    vec3 nv = normalize(n_V);
    vec3 nm = normalize(n_M);

    float cosTheta = clamp(dot(nv, l), 0, 1);
    float bias = 5e-5 * tan(acos(cosTheta));

    vec3 s = vec3(shc.xy, (shc.z - bias) / shc.w);

    float visibility = 0.1;

    for (int i = 0; i < 9; ++i)
        visibility += 0.1 * texture(shadowMap, s + vec3(1e-4 * rand2(gl_FragCoord), 0));

    if (phong) {
        vec3 c = main_color;

        if (any(lessThan(abs(nm), vec3(lw)))) {
            c = line_color;
        }
        // clamp(dot(nv, l), 0.1, 1)
        color = visibility * c;
    } else {
        color = visibility * main_color;
    }
}
