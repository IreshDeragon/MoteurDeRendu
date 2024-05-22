#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

int main()
{
    // Initialisation
    gl::init("TPs de Rendering"); // On crée une fenêtre et on choisit son nom
    gl::maximize_window(); // On peut la maximiser si on veut
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE); // On peut configurer l'équation qui mélange deux couleurs, comme pour faire différents blend mode dans Photoshop. Cette équation-ci donne le blending "normal" entre pixels transparents.
    
    auto const triangle_mesh = gl::Mesh{{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position3D{0 /*Index de l'attribut dans le shader, on en reparle juste après*/}},
            .data   = {
                -0.5f, -0.5f, 0.5f,  // Position3D du 1er sommet
                +0.5f, -0.5f, 0.5f, // Position3D du 2ème sommet
                +0.5f, +0.5f, 0.5f, // Position3D du 3ème sommet
                -0.5f, +0.5f, 0.5f,  // Position3D du 4ème sommet
                -0.5f, -0.5f, -0.5f,  // Position3D du 5ème sommet
                +0.5f, -0.5f, -0.5f, // Position3D du 6ème sommet
                +0.5f, +0.5f, -0.5f, // Position3D du 7ème sommet
                -0.5f, +0.5f, -0.5f  // Position3D du 8ème sommet
            },
        }},
        .index_buffer   = {
        0, 1, 2, // Indices du premier triangle : on utilise le 1er, 2ème et 3ème sommet
        0, 2, 3,  // Indices du deuxième triangle : on utilise le 1er, 3ème et 4ème sommet
        1, 5, 6, // triangle 1 face droite
        1, 6, 2,// triangle 2 face droite
        4, 0, 3, // triangle 1 face gauche
        4, 3, 7, // triangle 2 face gauche
        4, 5, 1,// triangle 1 face dessus
        4, 1, 0,// triangle 2 face dessus
        2, 7, 3,// triangle 1 face dessous
        2, 6, 7,// triangle 2 face dessous
        1, 4, 2,// triangle 1 face derrière
        4, 7, 2// triangle 2 face derrière
        },
    }};
    auto const shader = gl::Shader{{
        .vertex   = gl::ShaderSource::File{"res/vertex.glsl"},
        .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
    }};

    // auto const fadeScreenMesh = gl::Mesh{{
    //     .vertex_buffers = {{
    //         .layout = {gl::VertexAttribute::Position2D{0 /*Index de l'attribut dans le shader, on en reparle juste après*/}},
    //         .data   = {
    //             -1.f, -1.f, // Position2D du 1er sommet
    //             +1.f, -1.f, // Position2D du 2ème sommet
    //             +1.f, +1.f, // Position2D du 3ème sommet
    //             -1.f, +1.f  // Position2D du 4ème sommet
    //         },
    //     }},
    //     .index_buffer   = {
    //     0, 1, 2, // Indices du premier triangle : on utilise le 1er, 2ème et 3ème sommet
    //     0, 2, 3  // Indices du deuxième triangle : on utilise le 1er, 3ème et 4ème sommet
    //     },
    // }};

    //Caméra
    auto camera = gl::Camera{};
    gl::set_events_callbacks({camera.events_callbacks()});
    

    float time=0.0;

    glClearColor(0.f, 0.f, 1.f, 1.f); // Choisis la couleur à utiliser. Les paramètres sont R, G, B, A avec des valeurs qui vont de 0 à 1
    glClear(GL_COLOR_BUFFER_BIT); // Exécute concrètement l'action d'appliquer sur tout l'écran la couleur choisie au-dessus

    while (gl::window_is_open())
    {
        glm::mat4 const view_matrix = camera.view_matrix();
        //glm::mat4 const projection_matrix = glm::infinitePerspective(1.8f /*field of view in radians*/, gl::framebuffer_aspect_ratio() /*aspect ratio*/, 0.1f /*near plane*/);
        //glm::mat4 const projection_matrix = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        glm::mat4 const projection_matrix = glm::infinitePerspective(1.f /*field of view in radians*/, gl::framebuffer_aspect_ratio() /*aspect ratio*/, 0.001f /*near plane*/);
        glm::mat4 const view_projection_matrix = projection_matrix * view_matrix;

        // glm::mat4 const rotation = glm::rotate(glm::mat4{1.f}, gl::time_in_seconds() /*angle de la rotation*/, glm::vec3{0.f, 0.f, 1.f} /* axe autour duquel on tourne */);
        // glm::mat4 const translation = glm::translate(glm::mat4{1.f}, glm::vec3{0.f, 1.f, 0.f} /* déplacement */);
        // glm::mat4 model = translation*rotation;
        // glm::mat4 const view_projection_matrix = model * projection_matrix * view_matrix;


        glClearColor(0.f, 0.f, 1.f, 1.f); // Choisis la couleur à utiliser. Les paramètres sont R, G, B, A avec des valeurs qui vont de 0 à 1
        glClear(GL_COLOR_BUFFER_BIT); // Exécute concrètement l'action d'appliquer sur tout l'écran la couleur choisie au-dessus
        // shader.bind();
        // shader.set_uniform("colo", glm::vec4{0.f, 0.f, 1.f, 0.03f});
        // shader.set_uniform("aspect_ratio", gl::framebuffer_aspect_ratio());
        // shader.set_uniform("isFade", true);
        // shader.set_uniform("offset", glm::vec2{0., 0.});
        // shader.set_uniform("time", time);
        // fadeScreenMesh.draw();
        
        shader.bind();
        shader.set_uniform("colo", glm::vec4{0.4,0.1,0.6,1});
        shader.set_uniform("aspect_ratio", gl::framebuffer_aspect_ratio());
        shader.set_uniform("isFade", false);
        shader.set_uniform("offset", glm::vec3{0., 0., 0.});
        shader.set_uniform("time", time);
        shader.set_uniform("view_projection_matrix", view_projection_matrix);
        time+= gl::delta_time_in_seconds();
        triangle_mesh.draw(); // C'est ce qu'on appelle un "draw call" : on envoie l'instruction à la carte graphique de dessiner notre mesh.

    }
}