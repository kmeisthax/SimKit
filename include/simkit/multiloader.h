#ifndef __SIMKIT_MULTILOADER_H_
#define __SIMKIT_MULTILOADER_H_

#include <simkit.h>
#include <string>
#include <iostream>
#include <vector>

namespace SimKit {
    class ILoader;
    
    /* Resource loader that allows multiple resource loaders to be queried at once.
     * 
     * The first resource loader to provide a valid resource will be used.
     */
    class SIMKIT_API MultiLoader : public virtual ILoader {
        std::vector<std::pair<int, ILoader*> > m_loaders;
    public:
        MultiLoader();
        virtual ~MultiLoader();

        /* Add a new loader.
         * 
         * The weight specifies the ordering that loaders are queried by.
         * Lighter (lower weight) loaders are queried before heavier loaders.
         * 
         * Loaders are not owned by the multiloader and must be disposed separately.
         */
        virtual void add_subloader(ILoader* ldr, int weight);
        
        /* ILoader impl */
        virtual std::istream* open_resource(std::string name, std::ios_base::openmode mode = std::ios_base::in);
    };
};

#endif
