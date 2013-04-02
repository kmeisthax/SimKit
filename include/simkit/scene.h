#ifndef __SIMKIT_RENDERER_H_
#define __SIMKIT_RENDERER_H_

#include <simkit.h>

namespace SimKit {
    class ISceneNode;

    /* Represents an entire scene to render.
     * 
     * It may include things like skyboxes or what have you.
     */
    class SIMKIT_API IScene {
        ISceneNode* m_root;
    protected:
        Scene();
        Scene(ISceneNode* root);
        
        void set_root_node(ISceneNode* root);
    public:
        virtual ~Scene() = 0;
        
        ISceneNode* get_root_node();
    };
};

#endif
