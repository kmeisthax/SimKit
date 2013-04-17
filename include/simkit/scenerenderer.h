#ifndef __SIMKIT_SCENERENDERER_H_
#define __SIMKIT_SCENERENDERER_H_

#include <simkit.h>

namespace SimKit {
    class IRenderer;
    class IScene;
    class ICamera;

    /* A Renderer that takes ISceneNode(s) as data.
     */
    class SIMKIT_API ISceneRenderer : public virtual IRenderer {
    protected:
        ISceneRenderer();
    public:
        virtual ~ISceneRenderer() = 0;

        /* Specify the scene to render and what viewpoint to render from.
         * 
         * The Camera must be a member of the Scene.
         */
        virtual void set_scene_root(IScene* scene, ICamera* cam) = 0;
    };
};

#endif
