#ifndef __SIMKIT_ANIMCHANNEL_H_
#define __SIMKIT_ANIMCHANNEL_H_

#include <simkit.h>

namespace SimKit {
    /* An Animation Channel provides a method of applying a compatible Animation
     * to an arbitrary region of memory.
     * 
     * An animation channel overlays already existing data in memory. You can
     * use it to directly manipulate that memory.
     * 
     * ParamType must support certain operators in order to be compatible with
     * AnimChannel. The operators required depend on the interpolation algorithm
     * (aka the type of "easing"). See the Easing enum for details. At the very
     * least ParamType must support operator= so that it can be written into the
     * target we are animating.
     * 
     * TimeType is constrained to types which implement the following operators:
     * 
     *  operator+, operator-, operator<=
     * 
     * where operator+ and operator- form a group over TimeType, and said group
     * is partially ordered.
     */
    template <typename ParamType, typename TimeType>
    class SIMKIT_API TAnimChannel { /* Template class */
    private:
        
        ParamType* target;
    public:
        TAnimChannel(ParamType* target);
        ~TAnimChannel();
        
        void update(const TimeType time);
    };
}

#endif
