#ifndef __SIMKIT_OPENGL_CONTEXTMGMT_H_
#define __SIMKIT_OPENGL_CONTEXTMGMT_H_

#include <simkit.h>
#include <simkit/opengl.h>

namespace SimKit {
    namespace OpenGL {
        /* Convenience functions for managing SDL_GL contexts.
         * 
         * They're intended to keep track of more information about contexts
         * than SDL normally provides. Specifically, it will attempt to share
         * contexts as much as possible, and provides facilities for binding
         * contexts without an associated window (so long as one still exists).
         * 
         * It also causes GL Load to reload it's functions as well.
         * 
         * These should be used instead of the SDL_GL functions, or not used at
         * all.
         */
        
        int make_current(SDL_Window* wnd, SDL_GLContext* out_selected_ctxt);
        int make_current(SDL_Window* wnd, SDL_GLContext ctxt);
        int make_current(SDL_GLContext ctxt);

        /* Invalidate about-to-be-destroyed resources.
         * 
         * The functions should be called before calling SDL_GL_DeleteContext,
         * or SDL_DestroyWindow.
         */
        void invalidate_window(SDL_Window* wnd);
        void invalidate_context(SDL_GLContext ctxt);
    };
};

#endif
