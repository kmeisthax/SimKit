#ifndef __SIMKIT_TASKMASTER_H__
#define __SIMKIT_TASKMASTER_H__

#include <string>
#include <vector>

#include <simkit.h>

namespace SimKit {
    /* Concrete class for managing tasks; merges multiple tasks together.
     */
    class SIMKIT_API TaskMaster : public virtual ITask {
    private:
        std::vector<ITask*> tasks;
        std::string name;
    protected:
        /* ITask impl */
        virtual int think(int time_differential);
    public:
        TaskMaster(std::string name);
        virtual ~TaskMaster();

        virtual void add_task(ITask* task);
        
        /* ITask impl */
        virtual std::string get_name();
        virtual void attach_to_timeline(ITimeline* timeline);
        virtual void task_resume();
        virtual void task_suspend();
    };
}

#endif
