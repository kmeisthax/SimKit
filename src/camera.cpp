#include <simkit.h>
#include <glm/gtc/matrix_transform.hpp>

SimKit::ICamera::ICamera() {};
SimKit::ICamera::~ICamera() {};

glm::mat4 SimKit::ICamera::get_view_matrix() {
    glm::vec4 campos, lookpos;
    ISceneNode* camnode = this;
    ISceneNode* looknode = this->get_look_target();
    
    camnode->get_transform(&campos, NULL, NULL);
    looknode->get_transform(&lookpos, NULL, NULL);
    
    while (camnode->parent()) {
        camnode = camnode->parent();
        
        glm::mat4 cammatr;
        camnode->get_transform(&cammatr);

        campos = cammatr * campos;
    };

    while (looknode->parent()) {
        looknode = looknode->parent();
        
        glm::mat4 lookmatr;
        looknode->get_transform(&lookmatr);

        lookpos = lookmatr * lookpos;
    };
    
    glm::lookAt(campos.xyz(), lookpos.xyz(), this->get_up_vector());
};

bool SimKit::ICamera::is_linear_lens() {
    switch (this->get_lens_type) {
        case SimKit::ICamera::LENS_ORTHOGRAPHIC:
        case SimKit::ICamera::LENS_PERSPECTIVE:
            return true;
        default:
            return false;
    }
};

glm::mat4 SimKit::ICamera::get_linear_lens() {
    glm::mat4 out(1.0f);
    if (!this->is_linear_lens()) return out;
    
    switch (this->get_lens_type) {
        case SimKit::ICamera::LENS_ORTHOGRAPHIC:
            float left, right, top, bottom, near, far;
            this->get_ortho_parameters(&left, &right, &top, &bottom, &near, &far);
            return glm::ortho(left, right, top, bottom, near, far);
        case SimKit::ICamera::LENS_PERSPECTIVE:
            float fov, aspect, near, far;
            this->get_persp_parameters(&fov, &aspect, &near, &far);
            return glm::perspective(fov, aspect, near, far);
        default:
            return out;
    }
};
