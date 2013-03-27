#ifndef __SIMKIT_DELETEHOOK_H_
#define __SIMKIT_DELETEHOOK_H_

#include <simkit.h>
#include <vector>

namespace SimKit {
    /* Represents an object whose deletion can be hooked at runtime.
     */
    class SIMKIT_API MDeleteHook { //Mixin class
    public:
        class IHandler {
        protected:
            IHandler();
        public:
            virtual void on_delete(MDeleteHook* killed) = 0;
        }
    private:
        std::vector<IHandler*> handlers;
    protected:
        MDeleteHook();
    public:
        virtual ~MDeleteHook();
        
        /* Event handler for deletion of reference-counted objects.
         */
        void register_delete_handler(IHandler* handler);
        void unregister_delete_handler(IHandler* handler);
    };
}

#endif
