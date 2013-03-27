#ifndef __SIMKIT_VFONT_H__
#define __SIMKIT_VFONT_H__

#include <simkit.h>
#include <SDL.h>

namespace SimKit {
    class MRefCount;
    
    /* A Virtual Font represents an object which can render glyphs onto SDL surfaces.
     * 
     */
    class SIMKIT_API IVFont : public virtual MRefCount {
    protected:
        IVFont();
    public:
        virtual ~IVFont() = 0;
        
        /* -- Line metrics data--
         * 
         * A single line of font text has four parameters called metrics
         * which specifies how to draw the font in relation to other fonts
         * on the screen.
         * 
         * =========== top of line ========================================
         *               ^                         ^   ^
         *    **         |   metric: ascent        |   |
         *   *  *        |                         |   |
         *   *  *        v          metric: height |   |
         * ---***----- baseline ------------------ | - | metric: skip -----
         *      *        ^                         |   |
         *   *  *        |   metric: descent       |   |
         *    **         v                         v   |
         * ----------- bottom of line ---------------- | ------------------
         *                                             v
         * =========== top of next line ===================================
         * 
         * All metrics are specified in integer pixels.
         * 
         * TODO: What if we're drawing Japanese or Chinese text vertically?
         */
        virtual void get_font_metrics(int *out_height, int *out_ascent, int *out_descent, int *out_skip) = 0;
        
        /* -- Glyph metrics data --
         *
         * A single glyph has different metrics from the whole line.
         * It takes up a bounding box which we specify relative to the baseline
         * and has an advance which specifies how far forward to draw the next
         * glyph.
         * 
         * The specified advance does not account for kerning. For accurate
         * kerning, you should render lines instead of glyphs.
         */
        virtual void get_glyph_data(const Uint32 ch, SDL_Rect *out_glyph_box, int *out_advance) = 0;
        
        /* -- Text rendering --
         * 
         * Text rendering can be either transparent or pre-blended onto a static
         * background color. Additionally, transparency of rendered glyphs can
         * be achieved through color keying (fast, not anti-aliasable) or alpha
         * channels (slow, supports anti-aliasing.) Using the COLORKEY or ALPHA
         * enum values in lieu of the background color will enable the requested
         * transparency.
         * 
         * Finally, you may render a whole string or a single codepoint.
         * Rendering a whole string allows the underlying font to kern text
         * properly.
         * 
         * TODO: How do we handle right-to-left text? (Hebrew, "Arabic")
         */
        enum TransparencyType { COLORKEY, ALPHA };
        virtual SDL_Surface* render_font(const Uint32 ch, const SDL_Color textcol, const SDL_Color bg) = 0;
        virtual SDL_Surface* render_font(const Uint32 ch, const SDL_Color textcol, const TransparencyType bg) = 0;
        virtual SDL_Surface* render_font(const std::string txt, const SDL_Color textcol, const SDL_Color bg) = 0;
        virtual SDL_Surface* render_font(const std::string txt, const SDL_Color textcol, const TransparencyType bg) = 0;
        
        /* -- Rendering options --
         * 
         * Extra styling may be added through the following options.
         * 
         * Please note that changing options may result in the metrics data for
         * fonts and glyphs being changed.
         */
    };
}

#endif
