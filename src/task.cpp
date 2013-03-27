#include <simkit.h>

SimKit::ITask::ITask() : tdiff(0), timeline(NULL) {
};

SimKit::ITask::~ITask() {
};

void SimKit::ITask::advance_time(int microseconds) {
    if (this->timeline) this->timeline->log_task_start(this);
    
    this->tdiff += microseconds;
    this->tdiff = this->think(this->tdiff);
    
    if (this->timeline) this->timeline->log_task_end(this);
};

int SimKit::ITask::get_time_differential() {
    return this->tdiff;
};

void SimKit::ITask::attach_to_timeline(SimKit::ITimeline* timeline) {
    this->timeline = timeline;
};

void SimKit::ITask::get_differential_limits(int *out_mindiff, int *out_maxdiff) {
    if (out_mindiff) *out_mindiff = 0;
    if (out_maxdiff) *out_maxdiff = 0;
};

void SimKit::ITask::task_resume() {};
void SimKit::ITask::task_suspend() {};

void SimKit::ITask::task_resume(SimKit::ITask* parent) {
    this->task_resume();
};
