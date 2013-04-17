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
            IScene* scene;
            ICamera* view;

            SDL_Window* wnd;
            SDL_GLContext ctxt;
            
            GLuint gbuf_fbo;
            GLuint gbuf_ambient, gbuf_diffuse, gbuf_specular, gbuf_phongparams, gbuf_depth, gbuf_normal;
            
            void render_node(ISceneNode* node, ICamera* cam);
            void render_pass(ICamera* cam);
            void gbuffer_pass();
            void deferred_pass();
        protected:
            virtual int think(int time_differential) = 0;
        public:
            DeferredRenderer();
            virtual ~DeferredRenderer();
            
            virtual void set_render_target(SDL_Window* wnd, SDL_GLContext ctxt) = 0;
            virtual void set_render_target(SDL_Window* wnd, SDL_GLContext ctxt, const SDL_Rect viewport) = 0;
            
            virtual void set_scene_root(IScene* scene, ICamera* cam) = 0;
        };
    };
};

#endif
