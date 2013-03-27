#ifndef __SIMKIT_TTFVFONT_H__
#define __SIMKIT_TTFVFONT_H__

#include <simkit.h>
#include <SDL.h>
#include <SDL_ttf.h>

namespace SimKit {
    /* TTFVFont is a simple wrapper around SDL_TTF's TTF_Font.
     */
    class SIMKIT_API TTFVFont : public virtual IVFont {
    private:
        TTF_Font* rfont;
    public:
        TTFVFont(TTF_Font* rfont);
        virtual ~TTFVFont();

        /* IVFont impl */
        virtual void get_font_metrics(int *out_height, int *out_ascent, int *out_descent, int *out_skip) = 0;
        virtual void get_glyph_data(const Uint32 ch, SDL_Rect *out_glyph_box, int *out_advance) = 0;
        virtual SDL_Surface* render_font(const Uint32 ch, const SDL_Color textcol, const SDL_Color bg) = 0;
        virtual SDL_Surface* render_font(const Uint32 ch, const SDL_Color textcol, const IVFont::TransparencyType bg) = 0;
        virtual SDL_Surface* render_font(const std::string txt, const SDL_Color textcol, const SDL_Color bg) = 0;
        virtual SDL_Surface* render_font(const std::string txt, const SDL_Color textcol, const IVFont::TransparencyType bg) = 0;
    };
}

#endif
