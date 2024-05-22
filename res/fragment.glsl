#version 410

out vec4 out_color;
uniform vec4 colo;
in vec2 vertex_position;
uniform sampler2D my_texture;
in vec2 uv;

void main()
{
    //out_color = colo;
    //out_color = vec4(vertex_position, 1., 1.);
    //out_color = vec4(uv.x, uv.y, 0., 1.);
    vec4 texture_color = texture(my_texture, uv);
    out_color = texture_color;
}