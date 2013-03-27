#include <simkit.h>
#include "utf8.h"

#define UNICODE
#define _UNICODE
#include "windows.h"

//Wide strings are utf16 on windows

void SimKit::EmergencyError(std::string& errtxt) {
    std::wstring werr = SimKit::utf8_to_widestring(errtxt);
    MessageBox(NULL, werr.c_str(), L"Emergency error", MB_OK | MB_ICONSTOP);
    std::wcout << werr;
}

void SimKit::SDLEmergencyError(std::string& errtxt) {
    errtxt += "\n";
    errtxt += SDL_GetError();
    
    SimKit::EmergencyError(errtxt);
}
