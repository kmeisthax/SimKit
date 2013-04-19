#include <simkit.h>
#include <simkit/opengl.h>

SimKit::OpenGL::DeferredRenderer::

SimKit::OpenGL::DeferredRenderer::DeferredRenderer(SimKit::ILoader* ldr) : ldr(ldr), scene(NULL), view(NULL), wnd(NULL), ctxt(NULL) {};
SimKit::OpenGL::DeferredRenderer::~DeferredRenderer() {};

void SimKit::OpenGL::DeferredRenderer::set_render_target(SDL_Window* wnd) {
//TODO: Clean up previous window & context if any
    SimKit::OpenGL::make_current(wnd, &this->ctxt);

    int w, h;
    SDL_GetWindowSize(wnd, &w, &h);
    
    glGenFramebuffers(1, &this->gbuf_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->gbuf_fbo);
//gbuf_ambient, gbuf_diffuse, gbuf_specular, gbuf_phongparams, gbuf_depth, gbuf_normal
    glGenTextures(1, &this->gbuf_ambient);
    glBindTexture(GL_TEXTURE_2D, this->gbuf_ambient);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F​, w, h, 0, GL_RGBA, GL_UNSIGNED_INT, NULL);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->gbuf_ambient, 0);
};

virtual void set_render_target(SDL_Window* wnd, const SDL_Rect viewport) = 0;

