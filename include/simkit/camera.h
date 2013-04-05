#ifndef __SIMKIT_CAMERA_H__
#define __SIMKIT_CAMERA_H__

#include <simkit.h>
#include <glm/glm.hpp>

namespace SimKit {
    class ISceneNode;

    /* A Camera is a special kind of SceneNode that can "look at" other SceneNodes.
     * 
     * It is responsible for determining the viewpoint and projection used to
     * render a scene.
     */
    class SIMKIT_API ICamera : public virtual ISceneNode {
    protected:
        ICamera();
    public:
        virtual ~ICamera() = 0;
        
        /* Get information about where the camera is looking and it's up-vector.
         */
        virtual ISceneNode* get_look_target() = 0;
        virtual void set_look_target(ISceneNode* lookat) = 0;
        virtual glm::vec3 get_up_vector() = 0;
        virtual void set_up_vector(const glm::vec3 upvector) = 0;
        
        /* Build a view matrix from the look target, up vector, and current position.
         * 
         * This specifies how points are transformed from world to view space.
         */
        virtual glm::mat4 get_view_matrix();
        
        enum LensType {
            LENS_ORTHOGRAPHIC,
            LENS_PERSPECTIVE
        };
        
        /* Get information about the camera's "lens", e.g. the way that points are transformed from view to clip space.
         */
        virtual LensType get_lens_type() = 0;

        /* Get the parameters of a camera's possible lens types.
         * 
         * Calling a function corresponding to a lens type that the camera is
         * not currently set to invokes undefined behavior.
         * 
         * The default implementations of these parameters are no-ops.
         * You must override 
         */
        virtual void get_ortho_parameters(float* out_left, float* out_right, float* out_top, float* out_bottom, float* out_near, float* out_far) = 0;
        virtual void get_persp_parameters(float* out_fov, float* out_aspect, float* out_near, float* out_far) = 0;
        
        /* Set the parameters of a particular lens projection, and implicitly change the lens type.
         */
        virtual void set_ortho_parameters(const float left, const float right, const float top, const float bottom, const float near, const float far) = 0;
        virtual void set_persp_parameters(const float fov, const float aspect, const float near, const float far) = 0;
        
        /* Some lens projection transforms can be represented as a linear transform matrix.
         */
        virtual bool is_linear_lens();
        virtual glm::mat4 get_linear_lens();
    };
};

#endif
