#ifndef __SIMKIT_STDLOADER_H_
#define __SIMKIT_STDLOADER_H_

#include <simkit.h>
#include <string>
#include <fstream>

namespace SimKit {
    class ILoader;
    
    /* Basic resource loader that opens files normally.
     * 
     * One may set the base directory used to load resources.
     * Alternatively, the current working directory will be used.
     */
    class SIMKIT_API StdLoader : public virtual ILoader {
    private:
        const char* cwd;
    public:
        StdLoader();
        StdLoader(const char* cwd);
        virtual ~StdLoader();
        
        /* ILoader impl */
        virtual std::istream* open_resource(std::string name, std::ios_base::openmode mode = std::ios_base::in);
    };
}

#endif
