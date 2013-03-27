#include <simkit.h>

SimKit::MRefCount::MRefCount() : rcount(0) {};
SimKit::MRefCount::~MRefCount() {};

void SimKit::MRefCount::inc_ref() {
    this->rcount++;
};

void SimKit::MRefCount::dec_ref() {
    if (this->rcount <= 0) delete this;
    else this->rcount--;
};
