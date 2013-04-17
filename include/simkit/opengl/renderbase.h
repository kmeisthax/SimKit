#ifndef __SIMKIT_OPENGL_RENDERBASE_H_
#define __SIMKIT_OPENGL_RENDERBASE_H_

#include <simkit.h>
#include <simkit/opengl.h>
#include <glload/_int_gl_type.h>
#include <istream>

namespace SimKit {
    class ISceneRenderer;
    class ISceneNode;
    
    namespace OpenGL {
        /* Support class for all OpenGL rendering.
         */
        class SIMKIT_API RenderBase : public virtual ISceneRenderer {
        private:
        protected:
            /* Compile a shader, given an istream that yields the shader source,
             * and the type of shader to compile.
             * 
             * The shader source will be prefixed with a header which either
             * declares an OpenGL ES minimum shader version of 100, or an OpenGL
             * minimum shader version of 130 with empty preprocessor declares
             * for OpenGL ES shader language specific precision qualifiers.
             * 
             * Source provided should be written with the proper GLES precision
             * qualifiers for an OpenGL ES shader. We'll take care of the
             * differences.
             */
            GLuint compile_shader(GLenum type, std::istream* src);

            /* Given a node, execute OpenGL commands suitable to render it.
             * 
             * This function is responsible for setting up 
             * 
             * This function can be overridden by subclasses in order to change
             * how rendering works.
             */
            virtual void render_node(ISceneNode* node, ICamera* cam) = 0;
            
            RenderBase();
        public:
            virtual ~RenderBase() = 0;
            
            /* Determine if OpenGL or OpenGL|ES is being used. */
            enum HardwareMode { GL_DESKTOP, GL_ES };
            HardwareMode get_gl_mode();
        };
    };
};

#endif
