#ifndef __SIMKIT_REF_H_
#define __SIMKIT_REF_H_

#include <simkit.h>
#include <SDL.h>
#include <map>
#include <string>

namespace SimKit {
    /* Object which represents the lifetime of a particular pointer to an object.
     * 
     * The underlying type must support inc_ref and dec_ref operations.
     * 
     * Deleting the ref MAY delete the underlying object. Therefore, you
     * you should keep the Ref for as long as you maintain references to the
     * object.
     * 
     * You may dereference the Ref to get the underlying object.
     */
    template <typename referenced_type>
    class SIMKIT_API TRef {
        referenced_type* parent;
    public:
        TRef() : parent(NULL) {
            
        };
        
        TRef(referenced_type& parent) : parent(&parent) {
            this->parent->inc_ref();
        };
        
        virtual ~Ref() {
            this->parent->dec_ref();
        };
        
        referenced_type& operator*() const {
            return *this->parent;
        };
        
        referenced_type& operator->() const {
            return *this->parent;
        };

        TRef<referenced_type> operator=(referenced_type& parent) {
            if (&parent == this->parent) return;
            
            if (this->parent) this->parent->dec_ref();
            this->parent = &parent;
            this->parent->inc_ref();
            
            return this;
        };

        TRef<referenced_type> operator=(TRef<referenced_type>& f) {
            return *this = *f.parent;
        };
    };
}

#endif


