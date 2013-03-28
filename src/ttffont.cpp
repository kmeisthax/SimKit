#include <simkit.h>

SimKit::TTFFont::TTFFont(TTF_Font* rfont) : rfont(rfont) {};
SimKit::TTFFont::~TTFFont() {
    TTF_CloseFont(this->rfont);
};

void SimKit::TTFFont::get_font_metrics(int *out_height, int *out_ascent, int *out_descent, int *out_skip) {
    if (out_height) *out_height = TTF_FontHeight(this->rfont);
    if (out_ascent) *out_ascent = TTF_FontAscent(this->rfont);
    if (out_descent) *out_descent = TTF_FontDescent(this->rfont);
    if (out_skip) *out_skip = TTF_FontLineSkip(this->rfont);
};

void SimKit::TTFFont::get_glyph_data(const Uint32 ch, SDL_Rect *out_glyph_box, int *out_advance) {
    int minx, maxx, miny, maxy, advance;
    TTF_GlyphMetrics(this->rfont, ch, &minx, &maxx, &miny, &maxy, &advance);
    if (out_advance) *out_advance = advance;
    if (out_glyph_box) {
        out_glyph_box->x = minx;
        out_glyph_box->y = -maxy;   //SDL uses top-left coords, OpenGL and FreeType use bottom-left coords
        out_glyph_box->w = maxx - minx;
        out_glyph_box->h = maxy - miny;
    }
};

SDL_Surface* SimKit::TTFFont::render_font(const Uint32 ch, const SDL_Color textcol, const SDL_Color bg) {
    return TTF_RenderGlyph_Shaded(this->rfont, ch, textcol, bg);
};

SDL_Surface* SimKit::TTFFont::render_font(const Uint32 ch, const SDL_Color textcol, const SimKit::IFont::TransparencyType bg) {
    switch (bg) {
        case SimKit::IFont::COLORKEY:
            return TTF_RenderGlyph_Solid(this->rfont, ch, textcol);
        case SimKit::IFont::ALPHA:
            return TTF_RenderGlyph_Blended(this->rfont, ch, textcol);
    }
};

SDL_Surface* SimKit::TTFFont::render_font(const std::string txt, const SDL_Color textcol, const SDL_Color bg) {
    return TTF_RenderUTF8_Shaded(this->rfont, txt.c_str(), textcol, bg);
};

SDL_Surface* SimKit::TTFFont::render_font(const std::string txt, const SDL_Color textcol, const SimKit::IFont::TransparencyType bg) {
    switch (bg) {
        case SimKit::IFont::COLORKEY:
            return TTF_RenderUTF8_Solid(this->rfont, txt.c_str(), textcol);
        case SimKit::IFont::ALPHA:
            return TTF_RenderUTF8_Blended(this->rfont, txt.c_str(), textcol);
    }
};
