#ifndef __SIMKIT_OPENGL_MESHCACHE_H_
#define __SIMKIT_OPENGL_MESHCACHE_H_

#include <simkit.h>
#include <simkit/opengl.h>
#include <SDL.h>
#include <glload/_int_gl_type.h>

namespace SimKit {
    namespace OpenGL {
        class SIMKIT_API MeshCache : TMeshCache<SDL_GLContext, GLuint, MeshCache> {
        public:
            MeshCache();
            virtual ~MeshCache();

            GLuint upload_data(SDL_GLContext ctxt, const float* data, const int count, const TMeshCache<SDL_GLContext, GLuint, MeshCache>::MeshDataType type, const IVMeshData::Usage usage);
            GLuint upload_data(SDL_GLContext ctxt, const int* data, const int count, const TMeshCache<SDL_GLContext, GLuint, MeshCache>::MeshDataType type, const IVMeshData::Usage usage);
            GLuint upload_data(SDL_GLContext ctxt, const float* data, const int count, const TMeshCache<SDL_GLContext, GLuint, MeshCache>::MeshDataType type, const IVMeshData::Usage usage, GLuint old_data);
            GLuint upload_data(SDL_GLContext ctxt, const int* data, const int count, const TMeshCache<SDL_GLContext, GLuint, MeshCache>::MeshDataType type, const IVMeshData::Usage usage, GLuint old_data);

            GLuint destroy_data(SDL_GLContext ctxt, GLuint old_data);
        };
    };
};

#endif
