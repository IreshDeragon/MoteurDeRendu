#version 410

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_uv;
uniform float aspect_ratio;
uniform float time;
uniform vec2 offset;
uniform bool isFade;
uniform mat4 view_projection_matrix;
out vec2 vertex_position;
out vec2 uv;

void main()
{
    uv = in_uv;
    vertex_position = in_position;
    vec2 pos = in_position;
    pos.x /= aspect_ratio;
    vec2 foffset = offset;
    if(!isFade){
        foffset.x += sin(time)/2;
        foffset.y += cos(time)/2;
        gl_Position = view_projection_matrix * vec4(in_position, 1., 1);
    }
    else{
        pos = in_position;
        gl_Position = vec4(pos + foffset, 1.,1.);
    }
    //vec2 offset = vec2(0.4, 0.4);
    //gl_Position = vec4(pos + foffset, 0., 1.);
    
    
}