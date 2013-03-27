#include <simkit.h>

SimKit::MDeleteHook::IHandler::IHandler() {};
SimKit::MDeleteHook::IHandler::~IHandler() {};

SimKit::MDeleteHook::MDeleteHook() {};
SimKit::MDeleteHook::~MDeleteHook() {
    for (std::vector<SimKit::MDeleteHook::IHandler*>::iterator i = this->handlers.begin(); i != this->handlers.end(); i++) {
        *i.on_delete(this);
    }
};

void SimKit::MDeleteHook::register_dealloc_handler(SimKit::MDeleteHook::IHandler* handler) {
    this->handlers.push_back(handler);
};

void SimKit::MDeleteHook::unregister_dealloc_handler(SimKit::MDeleteHook::IHandler* handler) {
    for (std::vector<SimKit::MDeleteHook::IHandler*>::iterator i = this->handlers.begin(); i != this->handlers.end(); i++) {
        if (*i == handler) {
            i = this->handlers.erase(i);
            
            if (i == this->handlers.end()) break;
        }
    }
};
