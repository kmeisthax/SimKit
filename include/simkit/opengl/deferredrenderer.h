#ifndef __SIMKIT_OPENGL_DEFERREDRENDERER_H_
#define __SIMKIT_OPENGL_DEFERREDRENDERER_H_

#include <simkit.h>
#include <simkit/opengl.h>
#include <glload/_int_gl_type.h>
#include <istream>

namespace SimKit {
    namespace OpenGL {
        class RenderBase;
        
        /* OpenGL renderer that does lighting and shading calculations in a separate pass.
         */
        class SIMKIT_API DeferredRenderer : public virtual RenderBase {
        private:
            ILoader* ldr;
            
            IScene* scene;
            ICamera* view;

            SDL_Window* wnd;
            SDL_Rect viewport;
            SDL_GLContext ctxt;
            
            GLuint gbuf_fbo;
            GLuint gbuf_ambient, gbuf_diffuse, gbuf_specular, gbuf_phongparams, gbuf_depth, gbuf_normal;
            
            void render_node(ISceneNode* node, ICamera* cam);
            GLuint gbuf_vshader, gbuf_fshader;
            GLuint gbuf_program;
            
            GLuint null_vshader, light_fshader;
            GLuint light_program;
            
            void render_pass(ICamera* cam);
            void gbuffer_pass();
            void deferred_pass();

            void setup_context();
            void teardown_context();
        protected:
            virtual int think(int time_differential) = 0;
        public:
            /* Construct and deconstruct DeferredRenderer.
             * 
             * DeferredRenderer requires a loader which has access to the
             * following GLSL shader resources:
             * 
             * opengl/postprocess_vertex.glsl
             * opengl/deferred_frag.glsl
             * opengl/gbuffer_vertex.glsl
             * opengl/gbuffer_frag.glsl
             */
            DeferredRenderer(ILoader* ldr);
            virtual ~DeferredRenderer();
            
            virtual void set_render_target(SDL_Window* wnd) = 0;
            virtual void set_render_target(SDL_Window* wnd, const SDL_Rect viewport) = 0;
            
            virtual void set_scene_root(IScene* scene, ICamera* cam) = 0;
        };
    };
};

#endif
