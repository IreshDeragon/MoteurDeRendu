#version 410

out vec4 out_color;
uniform vec4 colo;
in vec3 vertex_position;

void main()
{
    //out_color = colo;
    out_color = vec4(vertex_position, 1.);
}