#ifndef __SIMKIT_EVENTPUMP_H_
#define __SIMKIT_EVENTPUMP_H_

#include <simkit.h>
#include <string>
#include <vector>
#include <SDL.h>

namespace SimKit {
    class ITask;
    class IEventListener;
    class EventSource;
    
    /* Reads events from the SDL Event Queue and provides callbacks to other classes.
     * 
     * Since there is only one SDL Event Queue per process, the EventPump is a
     * singleton. Use get_event_pump to get the process event pump.
     */
    class SIMKIT_API EventPump : public virtual EventSource, public virtual ITask {
    private:
        EventPump();
        virtual ~EventPump();
        
        static EventPump singleton_pump;
    protected:
        virtual int think(int time_differential);
    public:
        static EventPump* get_event_pump();
        std::string get_name();
    };
}

#endif
