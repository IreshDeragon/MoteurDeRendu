#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

int main()
{
    // Initialisation
    gl::init("TPs de Rendering"); // On crée une fenêtre et on choisit son nom
    gl::maximize_window(); // On peut la maximiser si on veut
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE); // On peut configurer l'équation qui mélange deux couleurs, comme pour faire différents blend mode dans Photoshop. Cette équation-ci donne le blending "normal" entre pixels transparents.
    
    auto const triangle_mesh = gl::Mesh{{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position3D{0}, gl::VertexAttribute::UV{1}},
            .data   = {
                -0.5f, -0.5f, 0.5f, 0,0, // Position3D du 1er sommet
                +0.5f, -0.5f, 0.5f, 1, 0,// Position3D du 2ème sommet
                +0.5f, +0.5f, 0.5f, 1, 1, // Position3D du 3ème sommet
                -0.5f, +0.5f, 0.5f, 0, 1, // Position3D du 4ème sommet
                -0.5f, -0.5f, -0.5f, 0,0, // Position3D du 5ème sommet
                +0.5f, -0.5f, -0.5f, 1, 0,// Position3D du 6ème sommet
                +0.5f, +0.5f, -0.5f, 1, 1,// Position3D du 7ème sommet
                -0.5f, +0.5f, -0.5f, 0, 1,  // Position3D du 8ème sommet
            },
        }},
        .index_buffer   = {
        0, 1, 2,  0, 2, 3,  // Face 1 (bottom)
            4, 5, 6,  4, 6, 7,  // Face 2 (top)
            0, 1, 5,  0, 5, 4,  // Face 3 (front)
            2, 3, 7,  2, 7, 6,  // Face 4 (back)
            0, 3, 7,  0, 7, 4,  // Face 5 (left)
            1, 2, 6,  1, 6, 5   // Face 6 (right)
        },
    }};
    auto const postProcessMesh = gl::Mesh{{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position2D{0}, gl::VertexAttribute::UV{1}},
            .data   = {
                -1, -1, 0, 0,  // Position3D du 1er sommet
                +1, -1, 1, 0, // Position3D du 2ème sommet
                +1, +1, 1, 1, // Position3D du 3ème sommet
                -1, +1,  0, 1, // Position3D du 4ème sommet
            },
        }},
        .index_buffer   = {
        0, 1, 2,  0, 2, 3,  // Face 1 (bottom)
        },
    }};

    auto render_target = gl::RenderTarget{gl::RenderTarget_Descriptor{
        .width          = gl::framebuffer_width_in_pixels(),
        .height         = gl::framebuffer_height_in_pixels(),
        .color_textures = {
            gl::ColorAttachment_Descriptor{
                .format  = gl::InternalFormat_Color::RGBA8,
                .options = {
                    .minification_filter  = gl::Filter::NearestNeighbour, // On va toujours afficher la texture à la taille de l'écran,
                    .magnification_filter = gl::Filter::NearestNeighbour, // donc les filtres n'auront pas d'effet. Tant qu'à faire on choisit le moins coûteux.
                    .wrap_x               = gl::Wrap::ClampToEdge,
                    .wrap_y               = gl::Wrap::ClampToEdge,
                },
            },
        },
        .depth_stencil_texture = gl::DepthStencilAttachment_Descriptor{
            .format  = gl::InternalFormat_DepthStencil::Depth32F,
            .options = {
                .minification_filter  = gl::Filter::NearestNeighbour,
                .magnification_filter = gl::Filter::NearestNeighbour,
                .wrap_x               = gl::Wrap::ClampToEdge,
                .wrap_y               = gl::Wrap::ClampToEdge,
            },
        },
    }};


    auto const shader = gl::Shader{{
        .vertex   = gl::ShaderSource::File{"res/vertex.glsl"},
        .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
    }};
    auto const postProcessShader = gl::Shader{{
        .vertex = gl::ShaderSource::File{"res/PPvertex.glsl"},
        .fragment = gl::ShaderSource::File{"res/PPfragment.glsl"},
    }};

    auto const texture = gl::Texture{
    gl::TextureSource::File{ // Peut être un fichier, ou directement un tableau de pixels
        .path           = "res/texture.png",
        .flip_y         = true, // Il n'y a pas de convention universelle sur la direction de l'axe Y. Les fichiers (.png, .jpeg) utilisent souvent une direction différente de celle attendue par OpenGL. Ce booléen flip_y est là pour inverser la texture si jamais elle n'apparaît pas dans le bon sens.
        .texture_format = gl::InternalFormat::RGBA8, // Format dans lequel la texture sera stockée. On pourrait par exemple utiliser RGBA16 si on voulait 16 bits par canal de couleur au lieu de 8. (Mais ça ne sert à rien dans notre cas car notre fichier ne contient que 8 bits par canal, donc on ne gagnerait pas de précision). On pourrait aussi stocker en RGB8 si on ne voulait pas de canal alpha. On utilise aussi parfois des textures avec un seul canal (R8) pour des usages spécifiques.
    },
    gl::TextureOptions{
        .minification_filter  = gl::Filter::Linear, // Comment on va moyenner les pixels quand on voit l'image de loin ?
        .magnification_filter = gl::Filter::Linear, // Comment on va interpoler entre les pixels quand on zoom dans l'image ?
        .wrap_x               = gl::Wrap::Repeat,   // Quelle couleur va-t-on lire si jamais on essaye de lire en dehors de la texture ?
        .wrap_y               = gl::Wrap::Repeat,   // Idem, mais sur l'axe Y. En général on met le même wrap mode sur les deux axes.
    }
    };


    //Caméra
    auto camera = gl::Camera{};
    gl::set_events_callbacks({camera.events_callbacks()});
    gl::set_events_callbacks({
        camera.events_callbacks(),
        {.on_framebuffer_resized = [&](gl::FramebufferResizedEvent const& e) {
            if(e.width_in_pixels!=0 && e.height_in_pixels!=0){
                render_target.resize(e.width_in_pixels, e.height_in_pixels);
            }
        }},
    });
    

    float time=0.0;

        while (gl::window_is_open())
        {
            glClearColor(1.f, 0.f, 1.f, 1.f); // Dessine du rouge, non pas à l'écran, mais sur notre render target
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            render_target.render([&]() {
                glClearColor(1.f, 0.f, 0.f, 1.f); // Dessine du rouge, non pas à l'écran, mais sur notre render target
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    glm::mat4 const view_matrix = camera.view_matrix();
                    glm::mat4 const projection_matrix = glm::infinitePerspective(1.f /*field of view in radians*/, gl::framebuffer_aspect_ratio() /*aspect ratio*/, 0.001f /*near plane*/);
                    glm::mat4 const view_projection_matrix = projection_matrix * view_matrix;
                    shader.bind();
                    shader.set_uniform("colo", glm::vec4{0.4,0.1,0.6,1});
                    shader.set_uniform("aspect_ratio", gl::framebuffer_aspect_ratio());
                    shader.set_uniform("isFade", false);
                    shader.set_uniform("offset", glm::vec3{0., 0., 0.});
                    shader.set_uniform("time", time);
                    shader.set_uniform("view_projection_matrix", view_projection_matrix);
                    shader.set_uniform("my_texture", texture);
                    triangle_mesh.draw();
            });

            postProcessShader.bind();
            postProcessShader.set_uniform("my_texture", render_target.color_texture(0));
            postProcessMesh.draw();

        }
    
}