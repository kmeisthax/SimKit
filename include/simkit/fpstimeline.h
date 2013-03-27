#ifndef __SIMKIT_FPSTIMELINE_H_
#define __SIMKIT_FPSTIMELINE_H_

#include <simkit.h>
#include <string>

namespace SimKit {
    /* Implementation of Timeline that measures FPS and other performance data.
     */
    class SIMKIT_API FPSTimeline : public virtual ITimeline {
        int frame_begin;
        int frame_end;
        bool frame_recorded;

        /* Statistical data for the last 30 frames.
         * 
         * Stored as a ring buffer of millisecond counts.
         */
        static const int NUM_PAST_FRAMES = 30;
        int past_frames[NUM_PAST_FRAMES];
        int past_frames_ctr;
    public:
        FPSTimeline();
        virtual ~FPSTimeline();

        /* Performance statistics information */
        
        /* Get the number of microseconds the last frame took. */
        virtual int get_last_frametime();
        
        /* Get the number of frames executed in the last second. */
        virtual float get_fps();
        
        /* ITimeline impl */
        virtual void log_frame_start();
        virtual void log_frame_end();
        virtual void log_task_start(ITask* task);
        virtual void log_task_end(ITask* task);
        virtual void log_task_milestone(ITask* task, std::string milestone);
    };
}

#endif
