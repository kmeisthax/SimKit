#ifndef __SIMKIT_ANIMATION_H_
#define __SIMKIT_ANIMATION_H_

#include <simkit.h>

namespace SimKit {
    /* An Animation specifies the variance of a particular type (ParamType) over
     * the progression of another type (TimeType).
     * 
     * Multiple interpolation methods are provided for various use cases. The
     * most basic, EASE_IMMEDIATE, does no interpolation and merely sets the
     * parameter to the target parameter for the duration of the spline. This
     * interpolation method is supported for all types.
     * 
     * Linear interpolation is provided (EASE_LINEAR); it requires a type which
     * can be subtracted, divided, and multiplied.
     */
    template <typename ParamType, typename TimeType>
    class SIMKIT_API TAnimation { /* Template class */
    public:
        enum Easing {
            EASE_IMMEDIATE,
            EASE_LINEAR,
            EASE_BEIZER_QUADRATIC, //1 guide point
            EASE_BEIZER_CUBIC,     //2 guide points (most common)
            
            EASE_BEIZER_MAXPTS     //Not an actual mode.
                                   //Just here for internal purposes only.
        };
        
        struct Spline {
            TimeType duration;
            ParamType start, target;
            Easing type;
            ParamType control[2];
        };

        /* The loop behavior determines what happens to an animation when the
         * animation reaches the end of the set 
         */
        enum {
            LOOP_STOP,
            LOOP_REPEAT,
            LOOP_EXTRAPOLATE
        } loop_behavior;
        
        std::vector<Spline> keyframes;
        
        static ParamType basic_interpolate(const ParamType& start, const ParamType& target, const TimeType& target_time, const TimeType& current_time) {
            return start + ((target - start) / target_time) * current_time
        };
        
        static ParamType spline_interpolate(const Spline& in, const TimeType& current_time) {
            ParamType[EASE_BEIZER_MAXPTS] beizer_points;
            ParamType[EASE_BEIZER_MAXPTS] inter_points;
            int num_valid_pts = (int)in.type;
            
            switch (in.type) {
                case EASE_BEIZER_CUBIC:
                    beizer_points = {in.start, in.control[0], in.control[1], in.end};
                    break;
                case EASE_BEIZER_QUADRATIC:
                    beizer_points = {in.start, in.control[0], in.end};
                    break;
                case EASE_LINEAR:
                    return linear_interpolate(in.start, in.end, when);
                case EASE_IMMEDIATE:
                    return in.end;
            }
            
            for (int i = 0; i < num_valid_pts; i++) {
                inter_points[i] = linear_interpolate(beizer_points[i], beizer_points[i+1], when);
            }
            
            Spline recurse;
            recurse.type = in.type - 1;
            recurse.start = inter_points[0];
            recurse.end = inter_points[num_valid_pts];
            recurse.control[0] = inter_points[1];
            recurse.control[1] = inter_points[2];
            
            return spline_interpolate(recurse, when);
        };
    public:
        TAnimation(ParamType* target);
        ~TAnimation();
        
        void add_keyframe_constant(const TimeType duration, ParamType target) {
            Spline newspline;
            
            newspline.type = EASE_IMMEDIATE;
            newspline.target = target;
            newspline.duration = duration;
            
            this->keyframes.push_back(newspline);
        };
        
        void add_keyframe_linear(const TimeType duration, ParamType start, ParamType target) {
            Spline newspline;
            
            newspline.type = EASE_LINEAR;
            newspline.start = start;
            newspline.target = target;
            newspline.duration = duration;
            
            this->keyframes.push_back(newspline);
        };
        
        void add_keyframe_beizer(const TimeType duration, ParamType start, ParamType mid1, ParamType target) {
            Spline newspline;
            
            newspline.type = EASE_BEIZER_QUADRATIC;
            newspline.start = start;
            newspline.control[0] = mid1;
            newspline.target = target;
            newspline.duration = duration;
            
            this->keyframes.push_back(newspline);
        };
        
        void add_keyframe_beizer(const TimeType duration, ParamType start, ParamType mid1, ParamType mid2, ParamType target) {
            Spline newspline;
            
            newspline.type = EASE_BEIZER_QUADRATIC;
            newspline.start = start;
            newspline.control[0] = mid1;
            newspline.control[1] = mid2;
            newspline.target = target;
            newspline.duration = duration;
            
            this->keyframes.push_back(newspline);
        };
        
        ParamType get_future_value(const TimeType fwd_time) const {
            
        };
        
        void advance_time(const TimeType time);
    };
}

#endif
