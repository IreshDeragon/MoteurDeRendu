#version 410

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;
uniform float aspect_ratio;
uniform float time;
uniform vec3 offset;
uniform bool isFade;
uniform mat4 view_projection_matrix;
out vec3 vertex_position;
out vec2 uv;
out vec3 normal;
//out vec4 view_projection_matrix_frag;



void main()
{
    //view_projection_matrix_frag = view_projection_matrix;
    uv = in_uv;
    normal = in_normal;
    vertex_position = in_position;
    vec3 pos = in_position;
    pos.x /= aspect_ratio;
    vec3 foffset = offset;

    gl_Position = view_projection_matrix * vec4(in_position, 1.);
}