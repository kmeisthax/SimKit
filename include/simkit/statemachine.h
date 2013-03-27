#ifndef __SIMKIT_STATEMACHINE_H_
#define __SIMKIT_STATEMACHINE_H_

#include <simkit.h>
#include <string>

namespace SimKit {
    class ITask;
    
    /* Represents a task which can process one of several different tasks,
     * depending on what state is currently selected.
     */
    class SIMKIT_API StateMachine : virtual private ITask {
    private:
        ITask* current_state;
        std::string name;
    protected:
        /* ITask impl */
        virtual int think(int time_differential) = 0;
    public:
        StateMachine(std::string name);
        virtual ~StateMachine();
        
        /* Select a new task.
         */
        virtual void switch_state(ITask* new_state);
        
        /* ITask impl */
        virtual std::string get_name();
    };
}

#endif
