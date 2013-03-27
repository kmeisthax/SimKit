#ifndef __SIMKIT_EMERGENCY_H_
#define __SIMKIT_EMERGENCY_H_

#include <simkit.h>
#include <string>

namespace SimKit {
    /* Emergency error handler.
     * 
     * Call this to notify a user of an extremely unrecoverable error that would
     * prevent other rendering paths from notifying the user of such things.
     */
    void SIMKIT_API EmergencyError(std::string& errtxt);

    /* Automatically report an SDL error.
     */
    void SIMKIT_API SDLEmergencyError(std::string& errtxt);
}

#endif
