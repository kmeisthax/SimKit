#ifndef __SIMKIT_SCENENODE_H_
#define __SIMKIT_SCENENODE_H_

#include <simkit.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace SimKit {
    /* A SceneNode structures renderables within a scene.
     */
    class SIMKIT_API ISceneNode {
    protected:
        ISceneNode();
    public:
        virtual ~ISceneNode() = 0;
        
        /* A scene node has separate position, orientation, and scale properties.
         * 
         * These may be retrieved, set, or added as appropriate. Sending a NULL
         * to any of the three parameters results in that parameter being
         * unchanged/ignored.
         * 
         * A default implementation of add_transform is provided which uses get
         * and set as appropriate.
         */
        virtual void get_transform(glm::vec4* out_pos, glm::quat* out_orientation, glm::vec3* out_scale) = 0;
        virtual void set_transform(const glm::vec4* pos, const glm::quat* orientation, const glm::vec3* scale) = 0;
        virtual void add_transform(const glm::vec4* pos, const glm::quat* orientation, const glm::vec3* scale);
        
        /* Convert the aformentioned transform into a matrix.
         */
        virtual void get_transform(glm::mat4* out_matr);
        
        /* SceneNodes may have children SceneNodes.
         * 
         * 
         */
        class iterator {
        
        }
    };
}

#endif
