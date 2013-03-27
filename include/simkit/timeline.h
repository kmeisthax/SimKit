#ifndef __SIMKIT_TIMELINE_H_
#define __SIMKIT_TIMELINE_H_

#include <simkit.h>
#include <string>

namespace SimKit {
    /* Represents a log of timed events that ITask(s) and client gameloop code
     * can use to track execution time.
     * 
     * As per ITask convention all times are in microseconds as usual.
     */
    class SIMKIT_API ITimeline {
    public:
        virtual ~ITimeline() = 0;

        /* == Basic performance information == 
         * 
         * This section of ITimeline records performance information on the
         * level of a "frame", i.e. a single execution of every task that makes
         * up your game or simulation.
         */
         
        /* Log the start and ends of a frame.
         */
        virtual void log_frame_start() = 0;
        virtual void log_frame_end() = 0;
        
        /* == Detailed performance information ==
         * 
         * This section of ITimeline records detailed, inside-the-frame
         * information about the various tasks which comprise a frame.
         */
        
        /* Log the start and ends of a rendering task.
         */
        virtual void log_task_start(ITask* task) = 0;
        virtual void log_task_end(ITask* task) = 0;
        
        /* Mark progress within a rendering task.
         */
        virtual void log_task_milestone(ITask* task, std::string milestone) = 0;
    };
}

#endif
