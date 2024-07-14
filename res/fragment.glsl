#version 410

out vec4 out_color;
in vec3 vertex_position;
uniform sampler2D my_texture;
uniform vec3 light_direction;
uniform vec3 ponctualLight_position;
in vec2 uv;
in vec3 normal;

void main()
{
    float shade = dot(light_direction, normal);
    if(shade<0){
        shade = 0;
    }
    shade = shade+0.3;
    float distanceX = vertex_position.x-ponctualLight_position.x;
    float distanceY = vertex_position.y-ponctualLight_position.y;
    float distanceZ = vertex_position.z-ponctualLight_position.z;
    if(distanceX<0){
        distanceX*=-1;
    }
    if(distanceY<0){
        distanceY*=-1;
    }
    if(distanceZ<0){
        distanceZ*=-1;
    }
    float totalDistance = distanceX + distanceY + distanceZ;
    float pointShade = dot((vertex_position-ponctualLight_position), normal) / (totalDistance * totalDistance); // normalised?
    if(pointShade<0){
        pointShade = 0;
    }
    shade += pointShade;
    vec4 texture_color = texture(my_texture, uv);
    texture_color.x *= shade;
    texture_color.y *= shade;
    texture_color.z *= shade;
    out_color = texture_color;
    
}