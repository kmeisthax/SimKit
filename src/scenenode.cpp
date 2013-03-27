#include <simkit.h>

SimKit::SceneNode::SceneNode() {};
SimKit::SceneNode::~SceneNode() {};

void SimKit::SceneNode::get_transform(glm::mat4* out_matr) {
    if (out_matr) {
        glm::vec4 pos;
        glm::quat orientation;
        glm::vec3 scale;
        
        this->get_transform(&pos, &orientation, &scale);

        glm::mat4 pmatr = glm::translate(glm::mat4(1.0f), pos);
        , omatr, smatr;
        out_matr = 
    }
}

#endif
