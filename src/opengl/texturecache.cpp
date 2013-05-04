#include <simkit.h>
#include <simkit/opengl.h>

#include <string>

SimKit::OpenGL::TextureCache::TextureCache() {};
SimKit::OpenGL::TextureCache::~TextureCache() {};

GLuint SimKit::OpenGL::TextureCache::upload_data(SDL_GLContext ctxt, const SDL_Surface* data, bool* out_success) {
    if (!data) return;
    
    int err = SimKit::OpenGL::make_current(ctxt);
    
    if (err != 0) {
        std::string err = "Failed to make context current when uploading texture data.\n";
        SimKit::SDLEmergencyError(err);
        if (out_success) *out_success = FALSE;
        return 0;
    }
    
    GLint tex_ifmt;
    GLint data_fmt;
    GLint data_type;
    //TODO: Palleted formats are NOT supported by OpenGL!
    //TODO: Make this more elegant
    switch (data->format) {
        case SDL_PIXELFORMAT_RGB332:
            data_fmt = GL_RGB;
            data_type = GL_UNSIGNED_BYTE_3_3_2;
            tex_ifmt = GL_R3_G3_B2;
            break;
        case SDL_PIXELFORMAT_RGB444:
            data_fmt = GL_RGB;
            data_type = GL_UNSIGNED_SHORT_4_4_4_4;
            tex_ifmt = GL_RGB4;
            break;
        case SDL_PIXELFORMAT_RGB555:
            data_fmt = GL_RGB;
            data_type = GL_UNSIGNED_SHORT_5_5_5_1;
            tex_ifmt = GL_RGB5;
            break;
        case SDL_PIXELFORMAT_BGR555:
            data_fmt = GL_BGR;
            data_type = GL_UNSIGNED_SHORT_5_5_5_1;
            tex_ifmt = GL_RGB5;
            break;
        case SDL_PIXELFORMAT_ARGB4444:
            data_fmt = GL_BGRA;
            data_type = GL_UNSIGNED_SHORT_4_4_4_4_REV;
            tex_ifmt = GL_RGBA4;
            break;
        case SDL_PIXELFORMAT_RGBA4444:
            data_fmt = GL_RGBA;
            data_type = GL_UNSIGNED_SHORT_4_4_4_4;
            tex_ifmt = GL_RGBA4;
            break;
        case SDL_PIXELFORMAT_ABGR4444:
            data_fmt = GL_RGBA;
            data_type = GL_UNSIGNED_SHORT_4_4_4_4_REV;
            tex_ifmt = GL_RGBA4;
            break;
        case SDL_PIXELFORMAT_BGRA4444:
            data_fmt = GL_BGRA;
            data_type = GL_UNSIGNED_SHORT_4_4_4_4;
            tex_ifmt = GL_RGBA4;
            break;
        case SDL_PIXELFORMAT_ARGB1555:
            data_fmt = GL_BGRA;
            data_type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
            tex_ifmt = GL_RGB5_A1;
            break;
        case SDL_PIXELFORMAT_RGBA5551:
            data_fmt = GL_RGBA;
            data_type = GL_UNSIGNED_SHORT_5_5_5_1;
            tex_ifmt = GL_RGB5_A1;
            break;
        case SDL_PIXELFORMAT_ABGR1555:
            data_fmt = GL_RGBA;
            data_type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
            tex_ifmt = GL_RGB5_A1;
            break;
        case SDL_PIXELFORMAT_BGRA5551:
            data_fmt = GL_BGRA;
            data_type = GL_UNSIGNED_SHORT_5_5_5_1;
            tex_ifmt = GL_RGB5_A1;
            break;
        case SDL_PIXELFORMAT_RGB565:
            data_fmt = GL_RGB;
            data_type = GL_UNSIGNED_SHORT_5_6_5;
            tex_ifmt = GL_RGB8; //GL does not support 565 internally :/
            break;
        case SDL_PIXELFORMAT_BGR565:
            data_fmt = GL_BGR;
            data_type = GL_UNSIGNED_SHORT_5_6_5;
            tex_ifmt = GL_RGB8;
            break;
        case SDL_PIXELFORMAT_RGB24:
            data_fmt = GL_RGB;
            data_type = GL_UNSIGNED_BYTE;
            tex_ifmt = GL_RGB8;
            break;
        case SDL_PIXELFORMAT_BGR24:
            data_fmt = GL_BGR;
            data_type = GL_UNSIGNED_BYTE;
            tex_ifmt = GL_RGB8;
            break;
        case SDL_PIXELFORMAT_RGB888:
            data_fmt = GL_BGR;
            data_type = GL_UNSIGNED_INT_8_8_8_8_REV;
            tex_ifmt = GL_RGB8;
            break;
        case SDL_PIXELFORMAT_RGBX8888:
            data_fmt = GL_RGB;
            data_type = GL_UNSIGNED_INT_8_8_8_8;
            tex_ifmt = GL_RGB8;
            break;
        case SDL_PIXELFORMAT_BGR888:
            data_fmt = GL_RGB;
            data_type = GL_UNSIGNED_INT_8_8_8_8_REV;
            tex_ifmt = GL_RGB8;
            break;
        case SDL_PIXELFORMAT_BGRX8888:
            data_fmt = GL_BGR;
            data_type = GL_UNSIGNED_INT_8_8_8_8;
            tex_ifmt = GL_RGB8;
            break;
        case SDL_PIXELFORMAT_ARGB8888:
            data_fmt = GL_BGRA;
            data_type = GL_UNSIGNED_INT_8_8_8_8_REV;
            tex_ifmt = GL_RGBA8;
            break;
        case SDL_PIXELFORMAT_RGBA8888:
            data_fmt = GL_RGBA;
            data_type = GL_UNSIGNED_INT_8_8_8_8;
            tex_ifmt = GL_RGBA8;
            break;
        case SDL_PIXELFORMAT_ABGR8888:
            data_fmt = GL_RGBA;
            data_type = GL_UNSIGNED_INT_8_8_8_8_REV;
            tex_ifmt = GL_RGBA8;
            break;
        case SDL_PIXELFORMAT_BGRA8888:
            data_fmt = GL_BGRA;
            data_type = GL_UNSIGNED_INT_8_8_8_8;
            tex_ifmt = GL_RGBA8;
            break;
        case SDL_PIXELFORMAT_ARGB2101010:
            data_fmt = GL_BGRA;
            data_type = GL_UNSIGNED_INT_2_10_10_10_REV;
            tex_ifmt = GL_RGB10_A2;
            break;
        default:
            //Unsupported format!
            if (out_success) *out_success = FALSE;
            return;
    }
    
    GLuint texname;
    glGenTextures(1, &texname);
    glBindTexture(GL_TEXTURE_2D, texname);
    
    if (SDL_MUSTLOCK(data)) SDL_LockSurface(data);
    glTexImage2D(GL_TEXTURE_2D, 0, tex_ifmt, data->w, data->h, 0, data_fmt, data_type, data->pixels);
    if (SDL_MUSTLOCK(data)) SDL_UnlockSurface(data);
    
    //TODO: Add support for mipmap generation & other texture parameters
    
    return texname;
};

void SimKit::OpenGL::TextureCache::destroy_data(SDL_GLContext ctxt, GLuint old_data) {
    int err = SimKit::OpenGL::make_current(ctxt);
    
    if (err != 0) {
        std::string err = "Failed to make context current when uploading texture data.\n";
        SimKit::SDLEmergencyError(err);
        if (out_success) *out_success = FALSE;
        return 0;
    }
    
    glDeleteTextures(1, &old_data);
};
