#include <simkit.h>

SimKit::IVMeshData::IVMeshData() {};
SimKit::IVMeshData::~IVMeshData() {};

SimKit::IVMeshData::Usage SimKit::IVMeshData::get_usage_frequency() {
    return SimKit::IVMeshData::STATIC;
};

SimKit::IVMeshData::IRequest::IRequest(SimKit::IVMeshData* parent) : SimKit::ITVResource<SimKit::IVMeshData>::IRequest(parent) {};
SimKit::IVMeshData::IRequest::IRequest(SimKit::IVMeshData* parent, SimKit::IVMeshData::IRequest::IListener* listen) : SimKit::ITVResource<SimKit::IVMeshData>::IRequest(parent, listen) {};
