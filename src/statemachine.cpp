#include <simkit.h>

SimKit::StateMachine::StateMachine(std::string name) : name(name), current_state(NULL) {};
SimKit::StateMachine::~StateMachine() {};

int SimKit::StateMachine::think(int time_differential) {
    if (this->current_state) this->current_state->advance_time(time_differential);

    return 0;
};

void SimKit::StateMachine::switch_state(ITask* new_state) {
    if (this->current_state) this->current_state->task_suspend();
    if (new_state) new_state->task_resume(this);
    this->current_state = new_state;
};

std::string SimKit::StateMachine::get_name() {
    return this->name;
};
