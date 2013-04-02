#include <simkit.h>

#include <glm/matrix_transform.cpp>

SimKit::ISceneNode::ISceneNode() {};
SimKit::ISceneNode::~ISceneNode() {};

void SimKit::ISceneNode::get_transform(glm::mat4* out_matr) {
    if (out_matr) {
        glm::vec4 pos;
        glm::glx::quaternion::quat orientation;
        glm::vec3 scale;
        
        this->get_transform(&pos, &orientation, &scale);

        glm::mat4 tmatr = glm::gtc::matrix_transform::translate(glm::mat4(1.0f), pos);
        glm::mat4 omatr = glm::gtx::quaternion::toMat4(orientation);
        glm::mat4 smatr = glm::gtc::matrix_transform::scale(glm::mat4(1.0f), scale);
        *out_matr = tmatr * smatr * omatr;
    }
};

std::vector<ISceneNode*>::iterator SimKit::ISceneNode::children_begin() {
    return this->m_children.begin();
};

std::vector<ISceneNode*>::iterator SimKit::ISceneNode::children_end() {
    return this->m_children.end();
};

std::vector<ISceneNode*>::size_type SimKit::ISceneNode::children_size() {
    return this->m_children.size();
};

void SimKit::ISceneNode::append_child(SimKit::ISceneNode* child) {
    this->m_children.push_back(child);
};

void SimKit::ISceneNode::insert_child(SimKit::ISceneNode* child, std::vector<SimKit::ISceneNode*>::iterator place) {
    this->m_children.insert(child, place);
};

void SimKit::ISceneNode::remove_child(std::vector<SimKit::ISceneNode*>::iterator place) {
    this->m_children.erase(place);
};

void SimKit::ISceneNode::remove_child(SimKit::ISceneNode* child) {
    for (std::vector<SimKit::ISceneNode*>::iterator i = this->children_begin(); i != this->children_end(); i++) {
        if (*i == child) {
            this->remove_child(i);
            return;
        }
    }
};

#endif
