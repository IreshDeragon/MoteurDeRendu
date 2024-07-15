#version 410

out vec4 out_color;
in vec3 vertex_position;
uniform sampler2D my_texture;
uniform vec3 light_direction;
uniform vec3 ponctualLight_position;
uniform float ponctualLigth_intensity;
uniform vec3 light_color;
in vec2 uv;
in vec3 normal;

// vec3 apply_matrix_to_position(mat4 matrix, vec3 point)
// {
//     vec4 tmp = matrix * vec4(point, 1.);
//     return tmp.xyz / tmp.w;
// }

// vec3 apply_matrix_to_direction(mat4 matrix, vec3 direction)
// {
//     vec4 tmp = matrix * vec4(direction, 0.);
//     return normalize(tmp.xyz);
// }

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
    float pointShade = dot(normalize(vertex_position-ponctualLight_position), normal) / (totalDistance * totalDistance); // normalised?
    if(pointShade<0){
        pointShade = 0;
    }
    pointShade*= ponctualLigth_intensity;
    vec3 coloredShade = light_color * pointShade;
    vec4 texture_color = texture(my_texture, uv);
    texture_color.x *= shade + coloredShade.x;
    texture_color.y *= shade + coloredShade.y;
    texture_color.z *= shade + coloredShade.z;
    out_color = texture_color;
    
}