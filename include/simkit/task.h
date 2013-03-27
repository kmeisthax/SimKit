#ifndef __SIMKIT_TASK_H_
#define __SIMKIT_TASK_H_

#include <simkit.h>
#include <string>

namespace SimKit {
    class ITimeline;
    
    /* Represents a simulation task to be periodically updated.
     * 
     * Simulation tasks are responsible for advancing the state of a simulation
     * at particular timescales by taking discrete inputs of time and optionally
     * advancing the timescale as appropriate.
     * 
     * A simulation task may or may not advance time exactly according to the
     * current simulation timestep but may instead require it's own fixed
     * timestep - this is typical of physics engines which exhibit chaotic,
     * nondeterministic behavior when faced with varying timescales. Each Task
     * which does not advance to precisely the current time must instead keep
     * track of how much missed time it currently has.
     * 
     * Time is specified in microseconds by convention. No part of this class
     * actually enforces this and you may choose different units as needed.
     */
    class SIMKIT_API ITask {
    private:
        int tdiff;
    protected:
        /* Interface to child classes */
        
        /* Called by Task implementations to advance the simulation forward
         * when approriate amounts of time have been added.
         * 
         * The think function returns the new time_differential to use.*/
        virtual int think(int time_differential) = 0;
        ITimeline* timeline;
    public:
        ITask();
        virtual ~ITask();
        /* Mark that the simulation has moved forward by some amount of time.
         * 
         * The task may perform any amount of processing (including none) in
         * response to an advance_time call.
         */
        virtual void advance_time(int microseconds);
        
        /* Get the current time differential.
         * 
         * A simulation task may be behind or even ahead of the current time.
         * This function allows other parts of the simulation to be aware of
         * delayed or future simulation data.
         * 
         * Time differentials are specified as microseconds behind from the
         * implied current time. Time is added to the time differential whenever
         * advance_time is called, and removed when and if simulation processing
         * occurs on the task. (again, typically in advance_time)
         * 
         * The returned time differential may be negative, indicating that the
         * simulation is ahead of the implied current time.
         * 
         * If this task is part of another task, then the implied current time
         * is the parent's time differential. For example, if task A is the
         * parent of task B, and they report differentials of:
         * 
         *   A: 50us
         *   B: 20us
         * 
         * then task B is actually 70us behind.
         * 
         * Most parent tasks won't report a differential, though.
         */
        virtual int get_time_differential();

        /* Defines the limits of the simulation task's time differential.
         * 
         * A simulation task may have a minimum or maximum time differential
         * depending on how it responds to the advancement of time.
         * 
         * Time differentials are only allowed within the range of:
         *      mindiff <= diff <= maxdiff
         * 
         * or, alternatively specified as [mindiff, maxdiff].
         * 
         * Example limits:
         * 
         *  Variable-timestep rendering task            [0,0]
         *  Fixed-timestep physics tasks                [0, TIMESTEP]
         *                                              [-TIMESTEP/2, TIMESTEP/2]
         * 
         * The default implementation returns [0,0]. Override it if your task
         * runs ahead or behind of the game state.
         */
        virtual void get_differential_limits(int *out_mindiff, int *out_maxdiff);
        
        /* Get name of task.
         * 
         * Can be used for logging and reporting.
         */
        virtual std::string get_name() = 0;
        
        /* Change the timeline this task reports performance information to.
         */
        virtual void attach_to_timeline(ITimeline* timeline);

        /* Indicate to a task when processing on that task has started or ended.
         * Neither of these methods are mandatory; the default is to do nothing.
         * 
         * By default, a task that has been suspended can be resumed later.
         * 
         * A task may optionally be resumed under a parent task.
         * The default behavior of the overloaded task_resume is to call the
         * other version without a specified parent. You may override the
         * overloaded variation of task_resume in order to get the parent ptr.
         */
        virtual void task_resume();
        virtual void task_resume(ITask* parent);
        virtual void task_suspend();
    };
}

#endif
