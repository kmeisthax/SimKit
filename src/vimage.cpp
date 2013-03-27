#include <simkit.h>

SimKit::IVImage::IVImage() {};
SimKit::IVImage::~IVImage() {};

SimKit::IVImage::IRequest::IRequest(SimKit::IVImage* parent) : SimKit::ITVResource<SimKit::IVImage>::IRequest(parent) {};
SimKit::IVImage::IRequest::IRequest(SimKit::IVImage* parent, SimKit::IVImage::IRequest::IListener* listen) : SimKit::ITVResource<SimKit::IVImage>::IRequest(parent, listen) {};
