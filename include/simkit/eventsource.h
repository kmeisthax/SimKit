#ifndef __SIMKIT_EVENTSOURCE_H_
#define __SIMKIT_EVENTSOURCE_H_

#include <simkit.h>
#include <string>
#include <vector>
#include <SDL.h>

namespace SimKit {
    class IEventListener;
    
    /* Reads events from the SDL Event Queue and provides callbacks to other classes.
     */
    class SIMKIT_API EventSource {
    private:
        struct EventSubscription {
            IEventListener* listener;
            bool all_events;
            Uint32 min_event, max_event;
        };
        std::vector<EventSubscription> subs;
    protected:
        /* Send an event to listeners.
         */
        void broadcast_event(SDL_Event* evt);
    public:
        EventSource();
        virtual ~EventSource();

        /* Subscribe an event listener to a particular range of SDL events.
         * 
         * Event listeners will recieve calls to their event_callback function
         * when valid events (as specified by min_event, max_event) are recieved
         * when the EventPump task runs.
         * 
         * An event listener may specify the range of event types they accept.
         * Events of type less than min_event or greater than max_event will not
         * be sent to the event listener. Calling the non-ranged version of this
         * function will result in the listener being called to process every
         * event, including user-defined events.
         * 
         * An invalid event range (max_event < min_event) will be ignored and
         * result in your event listener recieving no callbacks.
         * 
         * You may specify multiple event ranges by subscribing multiple times.
         * Your event listener WILL recieve multiple callbacks for the same
         * event if you specify overlapping ranges.
         * 
         * The order in which listeners recieve subscribed events depends on the
         * order in which subscribe_listener is called, however, the EventPump
         * is technically free to call event listeners in any order.
         */
        void subscribe_listener(IEventListener* listener);
        void subscribe_listener(IEventListener* listener, Uint32 min_event, Uint32 max_event);
        
        /* Unsubscribe an event listener previously subscribed using subscribe_listener.
         */
        void unsubscribe_listener(IEventListener* listener);
    };
}

#endif
