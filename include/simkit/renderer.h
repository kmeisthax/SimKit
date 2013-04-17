#ifndef __SIMKIT_RENDERER_H_
#define __SIMKIT_RENDERER_H_

#include <simkit.h>
#include <SDL.h>

namespace SimKit {
    class ITask;

    /* Represents a Task that takes a data structure of some form and displays
     * it on screen.
     */
    class SIMKIT_API IRenderer : public virtual ITask {
    protected:
        IRenderer();
    public:
        virtual ~IRenderer() = 0;
        
        /* Set the target window or texture to render to.
         * 
         * Optionally, you may specify a viewport; a rectangle such that only
         * pixels in the surface or texture with coordinates within the viewport
         * will be altered.
         */
        virtual void set_render_target(SDL_Window* wnd) = 0;
        virtual void set_render_target(SDL_Window* wnd, const SDL_Rect viewport) = 0;
    };
};

#endif
