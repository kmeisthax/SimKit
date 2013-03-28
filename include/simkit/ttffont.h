#ifndef __SIMKIT_TTFFont_H__
#define __SIMKIT_TTFFont_H__

#include <simkit.h>
#include <SDL.h>
#include <SDL_ttf.h>

namespace SimKit {
    /* TTFFont is a simple wrapper around SDL_TTF's TTF_Font.
     */
    class SIMKIT_API TTFFont : public virtual IFont {
    private:
        TTF_Font* rfont;
    public:
        TTFFont(TTF_Font* rfont);
        virtual ~TTFFont();

        /* IFont impl */
        virtual void get_font_metrics(int *out_height, int *out_ascent, int *out_descent, int *out_skip) = 0;
        virtual void get_glyph_data(const Uint32 ch, SDL_Rect *out_glyph_box, int *out_advance) = 0;
        virtual SDL_Surface* render_font(const Uint32 ch, const SDL_Color textcol, const SDL_Color bg) = 0;
        virtual SDL_Surface* render_font(const Uint32 ch, const SDL_Color textcol, const IFont::TransparencyType bg) = 0;
        virtual SDL_Surface* render_font(const std::string txt, const SDL_Color textcol, const SDL_Color bg) = 0;
        virtual SDL_Surface* render_font(const std::string txt, const SDL_Color textcol, const IFont::TransparencyType bg) = 0;
    };
}

#endif
