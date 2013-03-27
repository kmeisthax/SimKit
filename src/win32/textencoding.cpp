#include <simkit.h>
#include "utf8.h"

//Win32 widestrings are UTF-16

std::wstring SimKit::utf8_to_widestring(std::string& utf8) {
    std::wstring out;

    std::string::iterator valid_end = utf8::find_invalid(utf8.begin(), utf8.end());
    utf8::utf8to16(utf8.begin(), valid_end, std::back_inserter(out));
    
    return out;
};

std::string SimKit::widestring_to_utf8(std::wstring& pfwide) {
    std::string out;
    
    utf8::utf16to8(pfwide.begin(), pfwide.end(), std::back_inserter(out));
    
    return out;
};
