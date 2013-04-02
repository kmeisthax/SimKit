#include <simkit.h>

SimKit::IScene::IScene() : m_root(NULL) {};
SimKit::IScene::IScene(SimKit::ISceneNode* root) : m_root(root) {};
SimKit::IScene::~IScene() {};

void SimKit::IScene::set_root_node(SimKit::ISceneNode* root) {
    this->m_root = root;
};

SimKit::ISceneNode* SimKit::IScene::get_root_node() {
    return this->m_root;
};
