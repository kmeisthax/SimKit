#ifndef __SIMKIT_IOSTREAMRWOPSADAPTER_H_
#define __SIMKIT_IOSTREAMRWOPSADAPTER_H_

#include <simkit.h>

namespace SimKit {
    /* Given space for an SDL_RWops struct, fill it's relevant function pointers
     * with information on how to read and write from the provided istream,
     * ostream, or iostream.
     */
    void construct_istream_rwops_adapter(SDL_RWops *context, std::istream *adapted);
    void construct_ostream_rwops_adapter(SDL_RWops *context, std::ostream *adapted);
    void construct_iostream_rwops_adapter(SDL_RWops *context, std::iostream *adapted);
}

#endif
