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
        std::vector<ISceneNode*> m_children;
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
        virtual void get_transform(glm::vec4* out_pos, glm::glx::quaternion::quat* out_orientation, glm::vec3* out_scale) = 0;
        virtual void set_transform(const glm::vec4* pos, const glm::glx::quaternion::quat* orientation, const glm::vec3* scale) = 0;
        virtual void add_transform(const glm::vec4* pos, const glm::glx::quaternion::quat* orientation, const glm::vec3* scale);
        
        /* Convert the aformentioned transform into a matrix.
         */
        virtual void get_transform(glm::mat4* out_matr);
        
        /* SceneNodes have children.
         * 
         * By default, these iterators manage an internal list of child nodes.
         */
        virtual std::vector<ISceneNode*>::iterator children_begin();
        virtual std::vector<ISceneNode*>::iterator children_end();
        virtual std::vector<ISceneNode*>::size_type children_size();
        
        virtual void append_child(ISceneNode* child);
        virtual void insert_child(ISceneNode* child, std::vector<ISceneNode*>::iterator place);
        virtual void remove_child(ISceneNode* child);
        virtual void remove_child(std::vector<ISceneNode*>::iterator place);
    };
}

#endif
