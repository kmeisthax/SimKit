#ifndef __SIMKIT_TEXTENCODING_H_
#define __SIMKIT_TEXTENCODING_H_

#include <simkit.h>
//Why am I including SDL here? Because Windows that's why
#include <SDL.h>
#include <string>
#include <vector>

namespace SimKit {
    /* == Platform dependent code. Functions defined in platform/textencoding.cpp == */
    
    /* Convert UTF-8 in a std::string to the platform-dependent widestring encoding.
     */
    std::wstring SIMKIT_API utf8_to_widestring(std::string& utf8);

    /* Revert the above, converting widestrings back to UTF-8 in std:string.
     */
    std::string SIMKIT_API widestring_to_utf8(std::wstring& pfwide);
    
    /* == Platform independent code. Functions defined in ./textencoding.cpp == */
    
    /* Convert UTF-8 in a std::string to a vector of UTF-32/code points.
     */
    std::vector<uint32_t> SIMKIT_API utf8_to_codepoints(std::string& utf8);

    /* Convert UTF-8 in a std::string to a vector of UTF-16 characters.
     */
    std::vector<uint16_t> SIMKIT_API utf8_to_vector16(std::string& utf8);

    /* Encode UTF-8 to a std::string from a vector of UTF-32/code points.
     */
    std::string SIMKIT_API utf8_from_codepoints(std::vector<uint32_t>& codepoints);

    /* Encode UTF-8 to a std::string from a vector of UTF-16 characters.
     */
    std::string SIMKIT_API utf8_from_vector16(std::vector<uint16_t>& utf16);
}

#endif
