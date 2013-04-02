#ifndef __SIMKIT_RENDERABLE_H__
#define __SIMKIT_RENDERABLE_H__

#include <simkit.h>

namespace SimKit {
    class ISceneNode;
    class IVMeshData;
    class Material;
    
    /* A Renderable is a SceneNode that has geometry & materials attached to it.
     */
    class SIMKIT_API IRenderable : public virtual ISceneNode {
        IRenderable();
    public:
        virtual ~IRenderable() = 0;
        
        virtual Material* get_material() = 0;
        virtual IVMeshData* get_mesh_data() = 0;
    };
};

#endif
