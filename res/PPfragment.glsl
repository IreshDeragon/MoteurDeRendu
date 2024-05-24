#version 410

out vec4 out_color;
in vec2 vertex_position;
uniform sampler2D my_texture;
in vec2 uv;

void main()
{

    vec4 texture_color = texture(my_texture, uv);
    float intensity = (texture_color.x + texture_color.y + texture_color.z)/3.;
    texture_color.x = intensity;
    texture_color.y = intensity;
    texture_color.z = intensity;
    out_color = texture_color;
    
}