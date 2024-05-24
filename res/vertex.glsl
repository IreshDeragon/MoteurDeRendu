#version 410

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;
uniform float aspect_ratio;
uniform float time;
uniform vec3 offset;
uniform bool isFade;
uniform mat4 view_projection_matrix;
out vec3 vertex_position;
out vec2 uv;

void main()
{
    uv = in_uv;
    vertex_position = in_position;
    vec3 pos = in_position;
    pos.x /= aspect_ratio;
    vec3 foffset = offset;
    if(!isFade){
        foffset.x += sin(time)/2;
        foffset.y += cos(time)/2;
        gl_Position = view_projection_matrix * vec4(in_position, 1.);
    }
    else{
        pos = in_position;
        gl_Position = vec4(pos + foffset, 1.);
    }
    
}