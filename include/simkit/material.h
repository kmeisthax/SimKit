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
        /* Defines what kinds of parameters participate in defining the material.
         * 
         * Note that, despite being a bitmask, not all combinations of mappings
         * are valid. Specifically:
         * 
         *     If no mappings are set, the material parameter is IGNORED.
         *     What that means depends on the parameter being defined:
         *     Some parameters are required to specify a complete material and
         *     not mapping them properly may result in your object not being
         *     rendered. Others will be replaced with a default value which
         *     implies that rendering continues without that feature in place.
         *     
         *     You cannot combine more than one per-vertex mapping.
         *     That means that PT_IMAGE | PT_ATTRIB is fine, but you cannot use
         *     PT_NORMAL | PT_ATTRIB and expect a valid mapping. Invalid mapping
         *     will cause the resulting parameter to be considered as if it had
         *     no material mappings at all.
         * 
         * The meaning of "combine" varies based on the parameter and it's
         * interpretation. The default interpretation is that the resulting
         * values from each mapping are multiplied componentwise to produce the
         * final parameter value.
         */
        enum ParamType {
            PT_UNMAPPED = 0,        //Parameter is completely unmapped.
                                    //i.e. forget about this rendering feature entirely
                                    
            PT_CONSTANT = 1 << 0,   //Parameter is constant throughout the model
            PT_IMAGE    = 1 << 1,   //Parameter varies according to a UV-mapped img
            PT_ATTRIB   = 1 << 2,   //Parameter varies using vertex attribute data
            PT_NORMAL   = 1 << 3,   //Parameter varies using vertex normal data
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
         * 
         * Swizzling is currently only supported for the PT_IMAGE mapping.
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
        /* Generic Phong Lighting Model. (All mandatory) */
            PARAM_AMBIENT,      //%,color of light reflected absent of any light (used to fake global illumination)
                                //RGB parameter, normalized to range 0 to 1.
            PARAM_DIFFUSE,      //%,color of light reflected when surface is seen from any particular angle
                                //RGB parameter, normalized to range 0 to 1.
            PARAM_SPECULAR,     //%,color of light reflected when surface is seen in perfect reflections
                                //RGB parameter, normalized to range 0 to 1.
            PARAM_NORMAL,       //The vector perpendicular to the surface of the object
                                //XYZ parameter, normalized to range -1 to 1.
            PARAM_SHININESS,    //Phong shininess parameter. Controls size of specular highlights.
                                //Float parameter, range from 0 to 1.
            
        /* Toon Lighting Model (Optional) */
            PARAM_TOON_DIFFUSE, //Reciprocal of number of distinct diffuse shading layers (i.e. quantization step)
                                //Float parameter, range from 0 to 1. 0 is unmapped value.
            PARAM_TOON_SPECULAR,//Reciprocal of number of distinct specular shading layers (i.e. quantization step)
                                //Float parameter, range from 0 to 1. 0 is unmapped value.
        };
        
        struct ParamSpec {
            Parameter param;
            ParamType mapping;
            
            /* PT_CONSTANT */
            float constant[4];

            /* PT_NORMAL (empty) */
            
            /* PT_ATTRIB */
            std::string attrib_name;
            int attrib_id;
            
            /* PT_IMAGE */
            ParamSwizzle swiz;
            std::string uv_name;
            int uv_id;
            Ref<IVImage> vtex;
        };
        
        Material();
        ~Material();
        
        /* Add an additional parameter mapping to the param specification.
         */
        void map_material_constant(const Parmeter param, const float constant[4]);
        void map_material_normal(const Parmeter param);
        void map_material_attrib(const Parmeter param, const std::string attrib_name);
        void map_material_attrib(const Parmeter param, const int attrib_id);
        void map_material_image(const Parmeter param, const std::string uvmap_name, const ParamSwizzle swiz, IVImage* vtex);
        void map_material_image(const Parmeter param, const int uvmap_id, const ParamSwizzle swiz, IVImage* vtex);
        
        /* Find what material mappings are enabled for a particular parameter,
         * and retrieve their settings.
         */
        ParamType get_material_mappings(const Parameter param);
        void get_material_constant(const Parameter param, float out_constant[4], bool* out_mapped);
        void get_material_attrib(const Parameter param, std::string &out_attrib_name, int *out_attrib_id, bool* out_mapped);
        void get_material_image(const Parameter param, std::string &out_uvmap_name, int *out_uvmap_id, ParamSwizzle *out_swiz, IVImage** out_vtex, bool* out_mapped);
        
        /* Unmap a previously mapped material parameter.
         * 
         * The optional unmapped parameter determines what mappings to destroy.
         * If not provided, all data relating to the parameter will be destroyed.
         */
        void unmap_material_parameter(const Parameter param);
        void unmap_material_parameter(const Parameter param, const ParamType unmapped);
    private:
        void ensure_material_parameter_exists(const Parameter param);
        std::map<Parameter, ParamSpec> m_params;
    }
};

#endif
