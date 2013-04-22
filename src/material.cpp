#include <simkit.h>

SimKit::Material::Material() : m_params() {};
SimKit::Material::~Material() {};

void SimKit::Material::ensure_material_parameter_exists(const SimKit::Material::Parameter param) {
    if (this->m_params.count(param) == 0) {
        SimKit::Material::ParamSpec spc;
        
        spc.param = param;
        spc.mapping = SimKit::Material::PT_UNMAPPED;
        
        this->m_params[param] = spc;
    }
}

void SimKit::Material::map_material_constant(const SimKit::Material::Parmeter param, const float constant[4]) {
    this->ensure_material_parameter_exists(param);
    this->m_params[param].mapping |= SimKit::Material::PT_CONSTANT;
    this->m_params[param].constant[0] = constant[0];
    this->m_params[param].constant[1] = constant[1];
    this->m_params[param].constant[2] = constant[2];
    this->m_params[param].constant[3] = constant[3];
};

void SimKit::Material::map_material_normal(const SimKit::Material::Parmeter param) {
    this->ensure_material_parameter_exists(param);
    this->m_params[param].mapping |= SimKit::Material::PT_NORMAL;
};

void SimKit::Material::map_material_attrib(const SimKit::Material::Parmeter param, const std::string attrib_name) {
    this->ensure_material_parameter_exists(param);
    this->m_params[param].mapping |= SimKit::Material::PT_ATTRIB;
    this->m_params[param].attrib_name = attrib_name;
};

void SimKit::Material::map_material_attrib(const SimKit::Material::Parmeter param, const int attrib_id) {
    this->ensure_material_parameter_exists(param);
    this->m_params[param].mapping |= SimKit::Material::PT_ATTRIB;
    this->m_params[param].attrib_name = "";
    this->m_params[param].attrib_id = attrib_id;
    
    this->m_params[param] = spc;
};

void SimKit::Material::map_material_image(const SimKit::Material::Parmeter param, const std::string uvmap_name, const SimKit::Material::ParamSwizzle swiz, SimKit::IVImage* vtex) {
    this->ensure_material_parameter_exists(param);
    this->m_params[param].mapping |= SimKit::Material::PT_IMAGE;
    this->m_params[param].uvmap_name = uvmap_name;
    this->m_params[param].swiz = swiz;
    this->m_params[param].vtex = vtex;
};

void SimKit::Material::map_material_image(const SimKit::Material::Parmeter param, const int uvmap_id, const SimKit::Material::ParamSwizzle swiz, SimKit::IVImage* vtex) {
    this->ensure_material_parameter_exists(param);
    this->m_params[param].mapping != SimKit::Material::PT_IMAGE;
    this->m_params[param].uvmap_name = "";
    this->m_params[param].uvmap_id = uvmap_id;
    this->m_params[param].swiz = swiz;
    this->m_params[param].vtex = vtex;
};

SimKit::Material::ParamType SimKit::Material::get_material_mappings(const SimKit::Material::Parameter param) {
    if (this->m_params.count(param) != 1) return SimKit::Material::PT_UNMAPPED;
    else return this->m_params[param].mapping;
};

void SimKit::Material::get_material_constant(const SimKit::Material::Parameter param, float out_constant[4], bool* out_mapped) {
    if (this->m_params.count(param) != 1 || this->m_params[param].mapping & SimKit::Material::PT_CONSTANT == SimKit::Material::PT_UNMAPPED) {
        if (out_mapped) *out_mapped = false;
        return;
    }
    
    if (out_mapped) *out_mapped = true;
    if (out_constant) {
        out_constant[0] = this->m_params[param].constant[0];
        out_constant[1] = this->m_params[param].constant[1];
        out_constant[2] = this->m_params[param].constant[2];
        out_constant[3] = this->m_params[param].constant[3];
    }
};

void SimKit::Material::get_material_attrib(const SimKit::Material::Parameter param, std::string &out_attrib_name, int *out_attrib_id, bool* out_mapped) {
    if (this->m_params.count(param) != 1 || this->m_params[param].mapping & SimKit::Material::PT_ATTRIB == SimKit::Material::PT_UNMAPPED) {
        if (out_mapped) *out_mapped = false;
        return;
    }

    if (out_mapped) *out_mapped = true;
    out_attrib_name = this->m_params[param].attrib_name;
    *out_attrib_id = this->m_params[param].attrib_id;
};

void SimKit::Material::get_material_image(const SimKit::Material::Parameter param, std::string &out_uvmap_name, int *out_uvmap_id, SimKit::Material::ParamSwizzle *out_swiz, SimKit::IVImage** out_vtex, bool* out_mapped) {
    if (this->m_params.count(param) != 1 || this->m_params[param].mapping & SimKit::Material::PT_IMAGE == SimKit::Material::PT_UNMAPPED) {
        if (out_mapped) *out_mapped = false;
        return;
    }
    
    if (out_mapped) *out_mapped = true;
    out_uvmap_name = this->m_params[param].attrib_name;
    *out_uvmap_id = this->m_params[param].attrib_id;
    *out_swiz = this->m_params[param].swiz;
    *out_vtex = this->m_params[param].vtex;
};

void SimKit::Material::unmap_material_parameter(const SimKit::Material::Parameter param) {
    this->m_params.erase(param);
};

void SimKit::Material::unmap_material_parameter(const SimKit::Material::Parameter param, const SimKit::Material::ParamType unmapped) {
    if (this->m_params.count(param) == 0) return;
    
    this->m_params[param].mapping &= !unmapped;
    if (this->m_params[param].mapping == SimKit::Material::PT_UNMAPPED) this->m_params.erase(param);
};
