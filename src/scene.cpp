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

void SimKit::IScene::iterator::forward() {
    if (this->end) return;
    
    this->begin = false;
    
    if (this->cur_elem->children_size() > 0) {
        //Element has children. Descend down the tree
        this->i_stack.push_back(this->cur_elem->children_begin());
        this->cur_elem = *(this->cur_elem->children_begin());
        return;
    }

    while (this->cur_elem.parent() != NULL) {
        //Recursively ascend up the tree until we find an iterator we can increment
        this->cur_elem = this->cur_elem.parent();
        if (this->i_stack.back() != this->cur_elem.children_end()) {
            ++this->i_stack.back();
            this->cur_elem = *(this->i_stack.back());
            return;
        } else {
            this->i_stack.pop_back();
        }
    }
    
    this->end = true;
};

void SimKit::IScene::iterator::backward() {
    if (this->begin) return;

    if (this->end) {
        this->end = false;
        //Reestablish iterator stack
        while (this->cur_elem->children_size() > 0) {
            std::vector<SimKit::ISceneNode*>::iterator i = this->cur_elem.children_end() - 1;
            this->i_stack.push_back(i);
            this->cur_elem = *i;
        }
    }

    //If we can't go left, go up
    if (this->cur_elem->parent() && this->i_stack.back() == this->cur_elem->parent()->children_begin()) {
        this->cur_elem = this->cur_elem->parent();
        this->i_stack.pop_back();
        
        if (!this->cur_elem->parent()) this->begin = true;
        
        return;
    }

    //Otherwise, move left.
    this->i_stack.back() -= 1;
    this->cur_elem = *(this->i_stack.back());
    
    while (this->cur_elem->children_size() > 0) {
        std::vector<SimKit::ISceneNode*>::iterator i = this->cur_elem.children_end() - 1;
        this->i_stack.push_back(i);
        this->cur_elem = *i;
    }
};

SimKit::IScene::iterator::iterator() : begin(true), end(true), cur_elem(NULL) {};
SimKit::IScene::iterator::iterator(SimKit::IScene::iterator& copy) : {
    this->begin = copy.begin;
    this->end = copy.end;
    this->cur_elem = copy.cur_elem;
    this->i_stack = copy.i_stack;
};
SimKit::IScene::iterator::iterator(SimKit::ISceneNode* base) : begin(true), end(false), cur_elem(base) {};
SimKit::IScene::iterator::iterator(SimKit::ISceneNode* base, bool goto_end) {
    this->begin = !goto_end;
    this->end = goto_end;
    this->cur_elem = base;
};

SimKit::IScene::iterator& SimKit::IScene::iterator::operator=(SimKit::IScene::iterator& f) {
    this->begin = f.begin;
    this->end = f.end;
    this->cur_elem = f.cur_elem;
    this->i_stack = f.i_stack;

    return this;
};

bool SimKit::IScene::iterator::operator==(SimKit::IScene::iterator& f) {
    if (f.end && this->begin || f.begin && this->end) return false;
    else return this->cur_elem == f.cur_elem;
};

bool SimKit::IScene::iterator::operator!=(SimKit::IScene::iterator& f) {
    if (f.end && this->begin || f.begin && this->end) return true;
    else return this->cur_elem != f.cur_elem;
};

SimKit::ISceneNode* SimKit::IScene::iterator::operator*() {
    return this->cur_elem;
};

SimKit::ISceneNode* SimKit::IScene::iterator::operator->() {
    return this->cur_elem;
};

SimKit::IScene::iterator& SimKit::IScene::iterator::operator++() {
    this->forward();
    return this;
};

SimKit::IScene::iterator& SimKit::IScene::iterator::operator--() {
    this->backward();
    return this;
};

SimKit::IScene::iterator& SimKit::IScene::iterator::operator++(int) {
    SimKit::IScene::iterator ret(this);
    this->forward();
    return ret;
};

SimKit::IScene::iterator& SimKit::IScene::iterator::operator--(int) {
    SimKit::IScene::iterator ret(this);
    this->backward();
    return ret;
};

SimKit::IScene::iterator& SimKit::IScene::iterator::operator+(int fwd) {
    while (fwd > 0) {
        this->forward();
        fwd--;
    }

    return this;
};

SimKit::IScene::iterator& SimKit::IScene::iterator::operator-(int bck) {
    while (bck > 0) {
        this->backward();
        bck--;
    }

    return this;
};

SimKit::IScene::iterator SimKit::IScene::begin() {
    SimKit::IScene::iterator i(this->get_root_node());
    return i;
};

SimKit::IScene::iterator SimKit::IScene::end() {
    SimKit::IScene::iterator i(this->get_root_node(), true);
    return i;
};
