#include <simkit.h>
#include "utf8.h"

std::vector<uint32_t> SimKit::utf8_to_codepoints(std::string& utf8) {
    std::vector<uint32_t> out;

    std::string::iterator valid_end = utf8::find_invalid(utf8.begin(), utf8.end());
    utf8::utf8to32(utf8.begin(), valid_end, std::back_inserter(out));
    
    return out;
};

std::vector<uint16_t> SimKit::utf8_to_vector16(std::string& utf8) {
    std::vector<uint16_t> out;

    std::string::iterator valid_end = utf8::find_invalid(utf8.begin(), utf8.end());
    utf8::utf8to16(utf8.begin(), valid_end, std::back_inserter(out));
    
    return out;
};

std::string SimKit::utf8_from_codepoints(std::vector<uint32_t>& codepoints) {
    std::string out;
    
    utf8::utf32to8(codepoints.begin(), codepoints.end(), std::back_inserter(out));
    
    return out;
};

std::string SimKit::utf8_from_vector16(std::vector<uint16_t>& utf16) {
    std::string out;
    
    utf8::utf16to8(utf16.begin(), utf16.end(), std::back_inserter(out));
    
    return out;
};
