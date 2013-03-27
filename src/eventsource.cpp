#include <simkit.h>

SimKit::EventSource::EventSource() {};
SimKit::EventSource::~EventSource() {};

void SimKit::EventSource::subscribe_listener(IEventListener* listener) {
    for (std::vector<SimKit::EventSource::EventSubscription>::iterator i = this->subs.begin(); i != this->subs.end(); i++) {
        if (i->listener == listener) i = this->subs.erase(i);
    }

    SimKit::EventSource::EventSubscription sub;
    sub.listener = listener;
    sub.all_events = true;
    this->subs.push_back(sub);
};

void SimKit::EventSource::subscribe_listener(IEventListener* listener, Uint32 min_event, Uint32 max_event) {
    if (min_event > max_event) return;
    
    SimKit::EventSource::EventSubscription sub;
    sub.listener = listener;
    sub.all_events = false;
    sub.min_event = min_event;
    sub.max_event = max_event;
    
    this->subs.push_back(sub);
};

void SimKit::EventSource::unsubscribe_listener(IEventListener* listener) {
    for (std::vector<SimKit::EventSource::EventSubscription>::iterator i = this->subs.begin(); i != this->subs.end(); i++) {
        if (i->listener == listener) {
            this->subs.erase(i);
            //i = this->subs.begin();

            if (i == this->subs.end()) break;
        }
    }
};

void SimKit::EventSource::broadcast_event(SDL_Event* evt) {
    for (std::vector<SimKit::EventSource::EventSubscription>::iterator i = this->subs.begin(); i != this->subs.end(); i++) {
        if (i->all_events || (i->min_event <= evt->type && i->max_event >= evt->type)) {
            i->listener->event_callback(evt);
        }
    }
};
