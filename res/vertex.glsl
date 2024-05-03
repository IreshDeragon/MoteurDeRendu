#version 410

layout(location = 0) in vec2 in_position;
uniform float aspect_ratio;
uniform float time;
uniform vec2 offset;

void main()
{
    vec2 pos = in_position;
    pos.x /= aspect_ratio;
    vec2 foffset = offset;
    foffset.x += sin(time)/2;
    foffset.y += cos(time)/2;
    //vec2 offset = vec2(0.4, 0.4);
    gl_Position = vec4(pos + foffset, 0., 1.);
}