#ifndef __SIMKIT_H__
#define __SIMKIT_H__

/* MS-Windows specific DLL bullshit */
#ifdef WIN32
    #ifdef SIMKIT_EXPORTS
        #define SIMKIT_API __declspec(dllexport)
    #else
        #ifdef SIMKIT_STATIC
            #define SIMKIT_API
        #else
            #define SIMKIT_API __declspec(dllimport)
        #endif
    #endif
#else
    #define SIMKIT_API
#endif

/* -- * -- Gameloop organization and structure -- * -- */
#include "simkit/task.h"
#include "simkit/taskmaster.h"
#include "simkit/statemachine.h"

/* -- * -- Object mixins -- * -- */
#include "simkit/refcount.h"
#include "simkit/deletehook.h"

/* -- * -- Pointer types -- * -- */
#include "simkit/ref.h"

/* -- * -- Resource loading -- * -- */
#include "simkit/loader.h"          //Resource locators
#include "simkit/stdloader.h"
#include "simkit/vimage.h"          //Image renderers
#include "simkit/bitmapvimage.h"
#include "simkit/vfont.h"           //Font renderers
#include "simkit/ttfvfont.h"

/* -- * -- Error handling -- * -- */
#include "simkit/emergency.h"

/* -- * -- Text encoding handlers -- * -- */
#include "simkit/textencoding.h"

/* -- * -- Game performance tracking -- * -- */
#include "simkit/timeline.h"
#include "simkit/fpstimeline.h"

/* -- * -- Event handling -- * -- */
#include "simkit/eventsource.h"
#include "simkit/eventpump.h"
#include "simkit/eventlistener.h"

/* -- * -- Stupid glue logic for underlying C libs -- * -- */
#include "simkit/iostreamrwopsadapter.h"    //SDL/stream adapter

/* -- * -- Generic animation framework -- * -- */
#include "simkit/animation.hpp"
#include "simkit/animchannel.hpp"

#endif
