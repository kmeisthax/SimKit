#ifndef __SIMKIT_MATERIAL_H__
#define __SIMKIT_MATERIAL_H__

#include <simkit.h>

namespace SimKit {
    class IVImage;
    class IVMeshData;
    class MRefCount;
    
    /* A Material configures how a 3D model should be displayed.
     * 
     * It consists of parameters which can be mapped to a per-model constant,
     * a UV-mapped image, or per-vertex attributes.
     */
    class SIMKIT_API Material : public virtual MRefCount {
    public:
        enum ParamType {
            PT_UNMAPPED,        //Parameter is completely unmapped.
                                //i.e. forget about this rendering feature entirely
                                
            PT_CONSTANT,        //Parameter is constant throughout the model
            PT_IMAGE,           //Parameter varies according to a UV-mapped img
            PT_ATTRIB,          //Parameter varies using vertex attribute data
            PT_NORMAL,          //Parameter varies using vertex normal data
        };
        
        /* Some parameters can take vectors of up to four components.
         * The swizzle configures how an incoming vector of four components RGBA
         * is mapped to the components taken by the material, IJKL.
         * 
         * Here's an example of how to construct a swizzle:
         * 
         *      ParamSwizzle swiz = CMP_G << TO_K | CMP_A << TO_J;
         * 
         * This will mean that the material will take components IJKL to be
         * incoming components RAGR.
         * 
         * The way it works is that the swizzle is encoded as an array of 2 bit
         * fields. By shifting component numbers by output indicies, you create
         * the swizzle. This also means that you don't have to specify any
         * component that takes R.
         * 
         * A precomposed identity swizzle exists at SWIZZLE_RGBA.
         */
        enum ParamSwizzle {
            CMP_R = 0x00,
            CMP_G = 0x01,
            CMP_B = 0x02,
            CMP_A = 0x03,
            TO_I = 0x00,
            TO_J = 0x02,
            TO_K = 0x04,
            TO_L = 0x06,
            
            SWIZZLE_RGBA = CMP_R << TO_I | CMP_G << TO_J | CMP_B << TO_K | CMP_A << TO_L,
        };
        
        /* Contains a large number of material attributes which this material can specify.
         * 
         * Depending on the renderer used, some or even none of these properties
         * may actually affect rendering. It all depends on the renderer used,
         * the capabilities of the hardware, and if the user has enabled them.
         */
        enum Parameter {
            PARAM_DIFFUSE,  //The color of the material under grazing or .
            PARAM_SPECULAR, //The color of the material under directly incident light
            PARAM_NORMAL,   //The vector perpendicular to the surface of the object
        };
        
        struct ParamSpec {
            Parameter param;
            ParamType mapping;
            
            //Useful for constant parameters only
            float constant[4];
            
            //Useful for varying parameters only
            ParamSwizzle swiz;       //Rearranges parameter vectors
            std::string attrib_name; //can also be used to identify a UV map
            int attrib_id;           //same as above, but direct ID number
                                     //if name is empty string, id is used instead
            Ref<IVImage> vtex;      //Image to use as texture to look up
        };
        
        Material();
        ~Material();
        
        void map_material_parameter(const Parmeter param, const float constant[4]);
        void map_material_parameter(const Parmeter param, const ParamSwizzle swiz);
        void map_material_parameter(const Parmeter param, const std::string attrib_name, const ParamSwizzle swiz);
        void map_material_parameter(const Parmeter param, const std::string uvmap_name, const ParamSwizzle swiz, IVImage* vtex);
        void map_material_parameter(const Parmeter param, const int attrib_id, const ParamSwizzle swiz);
        void map_material_parameter(const Parmeter param, const int uvmap_id, const ParamSwizzle swiz, IVImage* vtex);

        ParamType get_material_parameter_mapping(const Parameter param);
        void get_material_parameter(const Parameter param, float out_constant[4]);
        void get_material_parameter(const Parameter param, std::string &out_attrib_name, int *out_attrib_id, ParamSwizzle *out_swiz);
        void get_material_parameter(const Parameter param, std::string &out_uvmap_name, int *out_uvmap_id, ParamSwizzle *out_swiz, IVImage** out_vtex);

        void unmap_material_parameter(const Parameter param);
    private:
        std::map<Parameter, ParamSpec> m_params;
    }
};

#endif
