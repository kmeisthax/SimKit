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
#include "simkit/ref.hpp"

/* -- * -- Concrete resource management -- * -- */
#include "simkit/loader.h"          //Resource locators
#include "simkit/stdloader.h"

/* -- * -- Virtual resource management -- * -- */
#include "simkit/vresource.hpp"     //Base class

#include "simkit/imagecache.h"      //Virtual textures/images
#include "simkit/vimage.h"
#include "simkit/bitmapvimage.h"    //SDL_image loader

#include "simkit/meshcache.hpp"     //Virtual mesh data
#include "simkit/vmeshdata.h"

/* -- * -- Font rendering -- * -- */
#include "simkit/fontloader.h"
#include "simkit/font.h"           
#include "simkit/ttffont.h"        //SDL_TTF font loader

/* -- * -- 3D scene structuring -- * -- */
#include "simkit/scene.h"
#include "simkit/scenenode.h"
#include "simkit/renderable.h"
#include "simkit/material.h"

/* -- * -- Screen rendering -- * -- */
#include "simkit/renderer.h"
#include "simkit/scenerenderer.h"   //Subclass which uses the default 3D scene structure

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
