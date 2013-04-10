#ifndef __SIMKIT_LOADER_H_
#define __SIMKIT_LOADER_H_

#include <simkit.h>
#include <string>
#include <iostream>

namespace SimKit {
    /* Interface to resource loaders.
     */
    class SIMKIT_API ILoader {
    public:
        virtual ~ILoader() = 0;
        
        /* Open a named resource and return an istream which can read the contents of the resource.
         * 
         * If the resource does not exist, return NULL.
         * 
         * Resource names are specified as strings.
         */
        virtual std::istream* open_resource(std::string name, std::ios_base::openmode mode = std::ios_base::in) = 0;
    };
}

#endif
