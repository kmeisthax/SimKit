#include <simkit.h>

SimKit::Material::Material() : m_params() {};
SimKit::Material::~Material() {};
        
void SimKit::Material::map_material_parameter(const SimKit::Material::Parmeter param, const float constant[4]) {
    SimKit::Material::ParamSpec spc;

    spc.param = param;
    spc.mapping = SimKit::Material::PT_CONSTANT;
    spc.constant[0] = constant[0];
    spc.constant[1] = constant[1];
    spc.constant[2] = constant[2];
    spc.constant[3] = constant[3];
    
    this->m_params[param] = spc;
};

void SimKit::Material::map_material_parameter(const SimKit::Material::Parmeter param, const SimKit::Material::ParamSwizzle swiz) {
    SimKit::Material::ParamSpec spc;

    spc.param = param;
    spc.mapping = SimKit::Material::PT_NORMAL;
    spc.swiz = swiz;
    
    this->m_params[param] = spc;
};

void SimKit::Material::map_material_parameter(const SimKit::Material::Parmeter param, const std::string attrib_name, const SimKit::Material::ParamSwizzle swiz) {
    SimKit::Material::ParamSpec spc;

    spc.param = param;
    spc.mapping = SimKit::Material::PT_ATTRIB;
    spc.attrib_name = attrib_name;
    spc.swiz = swiz;
    
    this->m_params[param] = spc;
};

void SimKit::Material::map_material_parameter(const SimKit::Material::Parmeter param, const std::string uvmap_name, const SimKit::Material::ParamSwizzle swiz, SimKit::IVImage* vtex) {
    SimKit::Material::ParamSpec spc;

    spc.param = param;
    spc.mapping = SimKit::Material::PT_IMAGE;
    spc.attrib_name = uvmap_name;
    spc.swiz = swiz;
    spc.vtex = vtex;
    
    this->m_params[param] = spc;
};

void SimKit::Material::map_material_parameter(const SimKit::Material::Parmeter param, const int attrib_id, const SimKit::Material::ParamSwizzle swiz) {
    SimKit::Material::ParamSpec spc;

    spc.param = param;
    spc.mapping = SimKit::Material::PT_ATTRIB;
    spc.attrib_name = "";
    spc.attrib_id = attrib_id;
    spc.swiz = swiz;
    
    this->m_params[param] = spc;
};

void SimKit::Material::map_material_parameter(const SimKit::Material::Parmeter param, const int uvmap_id, const SimKit::Material::ParamSwizzle swiz, SimKit::IVImage* vtex) {
    SimKit::Material::ParamSpec spc;

    spc.param = param;
    spc.mapping = SimKit::Material::PT_ATTRIB;
    spc.attrib_name = "";
    spc.attrib_id = uvmap_id;
    spc.swiz = swiz;
    spc.vtex = vtex;
    
    this->m_params[param] = spc;
};

SimKit::Material::ParamType SimKit::Material::get_material_parameter_mapping(const SimKit::Material::Parameter param) {
    if (this->m_params.count(param) != 1) return SimKit::Material::PT_UNMAPPED;
    else return this->m_params[param].mapping;
};

void SimKit::Material::get_material_parameter(const SimKit::Material::Parameter param, float out_constant[4]) {
    if (this->m_params.count(param) != 1) return;

    out_constant[0] = this->m_params[param].constant[0];
    out_constant[1] = this->m_params[param].constant[1];
    out_constant[2] = this->m_params[param].constant[2];
    out_constant[3] = this->m_params[param].constant[3];
};

void SimKit::Material::get_material_parameter(const SimKit::Material::Parameter param, std::string &out_attrib_name, int *out_attrib_id, SimKit::Material::ParamSwizzle *out_swiz) {
    if (this->m_params.count(param) != 1) return;

    out_attrib_name = this->m_params[param].attrib_name;
    *out_attrib_id = this->m_params[param].attrib_id;
    *out_swiz = this->m_params[param].swiz;
};

void SimKit::Material::get_material_parameter(const SimKit::Material::Parameter param, std::string &out_uvmap_name, int *out_uvmap_id, SimKit::Material::ParamSwizzle *out_swiz, SimKit::IVImage** out_vtex) {
    if (this->m_params.count(param) != 1) return;

    out_uvmap_name = this->m_params[param].attrib_name;
    *out_uvmap_id = this->m_params[param].attrib_id;
    *out_swiz = this->m_params[param].swiz;
    *out_vtex = this->m_params[param].vtex;
};

void SimKit::Material::unmap_material_parameter(const SimKit::Material::Parameter param) {
    this->m_params.erase(param);
};
