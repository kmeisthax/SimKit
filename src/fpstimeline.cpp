#include <simkit.h>

        int frame_begin;
        int frame_end;
        bool frame_recorded;
        static const int NUM_PAST_FRAMES = 30;
        int past_frames[NUM_PAST_FRAMES];
        int past_frames_ctr;

SimKit::FPSTimeline::FPSTimeline() : frame_begin(0), frame_end(0), frame_recorded(false), past_frames_ctr(0) {
    for (int i = 0; i < SimKit::FPSTimeline::NUM_PAST_FRAMES; i++) {
        this->past_frames[i] = 0;
    }
};

SimKit::FPSTimeline::~FPSTimeline() {
};

static const float MICROSECONDS_PER_SECOND = 1000000.0f;

void SimKit::FPSTimeline::log_frame_start() {
    this->frame_begin = SDL_GetPerformanceCounter();
    this->frame_recorded = false;
};

void SimKit::FPSTimeline::log_frame_end() {
    this->frame_end = SDL_GetPerformanceCounter();
    this->frame_recorded = true;

    this->past_frames[this->past_frames_ctr % SimKit::FPSTimeline::NUM_PAST_FRAMES] =
        ((float)(this->frame_end - this->frame_begin) / (float)SDL_GetPerformanceFrequency()) * MICROSECONDS_PER_SECOND;
    this->past_frames_ctr++;
};

//We don't do inside-the-frame performance information yet.
void SimKit::FPSTimeline::log_task_start(ITask* task) {};
void SimKit::FPSTimeline::log_task_end(ITask* task) {};
void SimKit::FPSTimeline::log_task_milestone(ITask* task, std::string milestone) {};

int SimKit::FPSTimeline::get_last_frametime() {
    return this->past_frames[this->past_frames_ctr % SimKit::FPSTimeline::NUM_PAST_FRAMES];
};

float SimKit::FPSTimeline::get_fps() {
    int num_valid_frames = this->past_frames_ctr > SimKit::FPSTimeline::NUM_PAST_FRAMES ?
        SimKit::FPSTimeline::NUM_PAST_FRAMES : this->past_frames_ctr;

    int past_frame_sum = 0;
    
    for (int i = 0; i < num_valid_frames; i++) {
        past_frame_sum += this->past_frames[i];
    }

    float avg_frametime = 1.0f;
    if (num_valid_frames > 0) avg_frametime = (float)past_frame_sum / (float)num_valid_frames;

    return MICROSECONDS_PER_SECOND / avg_frametime;
}
