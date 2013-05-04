#ifndef __SIMKIT_OPENGL_TEXTURECACHE_H_
#define __SIMKIT_OPENGL_TEXTURECACHE_H_

#include <simkit.h>
#include <simkit/opengl.h>
#include <SDL.h>
#include <glload/_int_gl_type.h>

namespace SimKit {
    namespace OpenGL {
        class SIMKIT_API TextureCache : TTextureCache<SDL_GLContext, GLuint, TextureCache> {
        private:
            TextureCache();
        public:
            virtual ~TextureCache();
            
            GLuint upload_data(SDL_GLContext ctxt, const SDL_Surface* data, bool* out_success);
            
            void destroy_data(SDL_GLContext ctxt, GLuint old_data);
            
            static TextureCache sys_texcache;
        };
    };
};

#endif
