#ifndef __SIMKIT_EVENTLISTENER_H_
#define __SIMKIT_EVENTLISTENER_H_

#include <simkit.h>
#include <string>
#include <SDL.h>

namespace SimKit {
    /* Interface class for event pump listeners.
     */
    class SIMKIT_API IEventListener {
    public:
        /* Delivers an SDL event to the listener.
         * 
         * The data inside the pointer will only be valid within the context of
         * the current event_callback call. If you want to retain the data,
         * store it somewhere else.
         */
        virtual void event_callback(const SDL_Event* info) = 0;
    };
}

#endif
