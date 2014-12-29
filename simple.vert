#version 130
in vec3 vertex_position;
//in vec3 vertex_normal;

uniform mat4 MVP;

//out vec3 n;

void main(void)
{
    gl_Position = MVP * vec4(vertex_position, 1.);
    //n = vertex_normal;
}
