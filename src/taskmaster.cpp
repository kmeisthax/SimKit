#include <simkit.h>

#include <climits>

SimKit::TaskMaster::TaskMaster(std::string name) : name(name), tasks() {};
SimKit::TaskMaster::~TaskMaster() {};

void SimKit::TaskMaster::add_task(ITask* task) {
    if (task) {
        this->tasks.push_back(task);
    }
};

int SimKit::TaskMaster::think(int time_differential) {
    for (std::vector<SimKit::ITask*>::iterator i = this->tasks.begin(); i != this->tasks.end(); i++) {
        (*i)->advance_time(time_differential);
    }

    return 0;
};

std::string SimKit::TaskMaster::get_name() {
    return this->name;
};

void SimKit::TaskMaster::attach_to_timeline(SimKit::ITimeline* timeline) {
    this->timeline = timeline;
    
    for (std::vector<SimKit::ITask*>::iterator i = this->tasks.begin(); i != this->tasks.end(); i++) {
        (*i)->attach_to_timeline(timeline);
    }
};

void SimKit::TaskMaster::task_resume() {
    for (std::vector<SimKit::ITask*>::iterator i = this->tasks.begin(); i != this->tasks.end(); i++) {
        (*i)->task_resume(this);
    }
};

void SimKit::TaskMaster::task_suspend() {
    for (std::vector<SimKit::ITask*>::iterator i = this->tasks.begin(); i != this->tasks.end(); i++) {
        (*i)->task_suspend();
    }
};
