#ifndef __SIMKIT_FONTLOADER_H__
#define __SIMKIT_FONTLOADER_H__

#include <simkit.h>

namespace SimKit {
    /* A FontLoader is the source for all fonts in the program.
     */
    class SIMKIT_API FontLoader {
    private:
        FontLoader();
    public:
        FontLoader& get_font_loader();
        
        /* Add a font to the font loader's internal font database.
         * 
         * The resource name must resolve to a valid TTF resource when loaded
         * with the provided resource loader.
         */
        void add_font_resource(const std::string resource_name);
        
        /* Create a font rendering context referring to a specific font.
         *
         * Screen_dpi is a parameter which scales points. The default parameter
         * makes points equivalent to pixels; however, most monitors are higher
         * DPI than 1980s era Macintosh screens and you should change this.
         */
        IVFont* load_font(const std::string font_name, const float points, const float screen_dpi = 72.0f);
    };
}

#endif
