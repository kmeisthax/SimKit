#ifndef __SIMKIT_SCENERENDERER_H_
#define __SIMKIT_SCENERENDERER_H_

#include <simkit.h>

namespace SimKit {
    class IRenderer;
    class IScene;

    /* A Renderer that takes ISceneNode(s) as data.
     */
    class SIMKIT_API ISceneRenderer : public virtual IRenderer {
    protected:
        ISceneRenderer();
    public:
        virtual ~ISceneRenderer() = 0;
        
        virtual void set_scene_root(IScene* root) = 0;
    };
};

#endif
