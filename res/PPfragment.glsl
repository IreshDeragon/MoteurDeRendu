#version 410

out vec4 out_color;
in vec2 vertex_position;
uniform sampler2D my_texture;
in vec2 uv;

void main()
{
    float contrast = 2.0;

    vec4 texture_color = texture(my_texture, uv);
    out_color = texture_color;
   
}