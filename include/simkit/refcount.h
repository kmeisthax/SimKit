#ifndef __SIMKIT_MRefCount_H_
#define __SIMKIT_MRefCount_H_

#include <simkit.h>

namespace SimKit {
    /* Represents an object with an internal reference count.
     * 
     * The reference count system is optional: you may create an object and
     * decide NOT to use references. However, if you do count references,
     * any scope which does not use inc_ref will be a weak reference.
     */
    class SIMKIT_API MRefCount : public virtual MDeleteHook { //Mixin class
        int rcount;
    protected:
        MRefCount();
    public:
        virtual ~MRefCount();

        /* Increment or decrement a reference as appropriate.
         * 
         * Dec_ref MAY delete the underlying object.
         */
        void inc_ref();
        void dec_ref();
    };
}

#endif
