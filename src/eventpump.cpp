#include <simkit.h>

SimKit::EventPump::EventPump() {};
SimKit::EventPump::~EventPump() {};

int SimKit::EventPump::think(int time_differential) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) this->broadcast_event(&e);
    
    return 0;
}

std::string SimKit::EventPump::get_name() {
    std::string out("EventPump");
    return out;
};

SimKit::EventPump SimKit::EventPump::singleton_pump;

SimKit::EventPump* SimKit::EventPump::get_event_pump() {
    return &singleton_pump;
};
