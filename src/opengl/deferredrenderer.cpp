#include <simkit.h>
#include <simkit/opengl.h>

SimKit::OpenGL::DeferredRenderer::

SimKit::OpenGL::DeferredRenderer::DeferredRenderer(SimKit::ILoader* ldr) : ldr(ldr), scene(NULL), view(NULL), wnd(NULL), ctxt(NULL) {};
SimKit::OpenGL::DeferredRenderer::~DeferredRenderer() {};

void SimKit::OpenGL::DeferredRenderer::setup_context() {
    std::istream* ssrc = this->ldr->open_resource("opengl/gbuffer_vertex.glsl");
    this->gbuf_vshader = this->compile_shader(GL_VERTEX_SHADER, ssrc);
    delete ssrc;
    
    ssrc = this->ldr->open_resource("opengl/gbuffer_frag.glsl");
    this->gbuf_fshader = this->compile_shader(GL_FRAGMENT_SHADER, ssrc);
    delete ssrc;

    ssrc = this->ldr->open_resource("opengl/null_vertex.glsl");
    this->null_vshader = this->compile_shader(GL_VERTEX_SHADER, ssrc);
    delete ssrc;
    
    ssrc = this->ldr->open_resource("opengl/deferred_frag.glsl");
    this->light_fshader = this->compile_shader(GL_FRAGMENT_SHADER, ssrc);
    delete ssrc;
    
    this->gbuf_program = glCreateProgram();
    glAttachShader(this->gbuf_program, this->gbuf_vshader);
    glAttachShader(this->gbuf_program, this->gbuf_fshader);
    glBindFragDataLocation(this->gbuf_program, 0, "gbuf_ambient_color");
    glBindFragDataLocation(this->gbuf_program, 1, "gbuf_diffuse_color");
    glBindFragDataLocation(this->gbuf_program, 2, "gbuf_specular_color");
    glBindFragDataLocation(this->gbuf_program, 3, "gbuf_phongparams");
    glBindFragDataLocation(this->gbuf_program, 4, "gbuf_normal");
    glLinkProgram(this->gbuf_program);
    
    this->light_program = glCreateProgram();
    glAttachShader(this->light_program, this->null_vshader);
    glAttachShader(this->light_program, this->light_fshader);
    glLinkProgram(this->light_program);
    
    glGenFramebuffers(1, &this->gbuf_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->gbuf_fbo);
    
    glGenTextures(1, &this->gbuf_ambient);
    glBindTexture(GL_TEXTURE_2D, this->gbuf_ambient);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F​, this->viewport.w, this->viewport.h, 0, GL_RGBA, GL_UNSIGNED_INT, NULL);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->gbuf_ambient, 0);
    
    glGenTextures(1, &this->gbuf_diffuse);
    glBindTexture(GL_TEXTURE_2D, this->gbuf_diffuse);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F​, this->viewport.w, this->viewport.h, 0, GL_RGBA, GL_UNSIGNED_INT, NULL);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->gbuf_diffuse, 0);
    
    glGenTextures(1, &this->gbuf_specular);
    glBindTexture(GL_TEXTURE_2D, this->gbuf_specular);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F​, this->viewport.w, this->viewport.h, 0, GL_RGBA, GL_UNSIGNED_INT, NULL);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->gbuf_specular, 0);
    
    glGenTextures(1, &this->gbuf_phongparams);
    glBindTexture(GL_TEXTURE_2D, this->gbuf_phongparams);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F​, this->viewport.w, this->viewport.h, 0, GL_RGBA, GL_UNSIGNED_INT, NULL);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, this->gbuf_phongparams, 0);
    
    glGenTextures(1, &this->gbuf_normal);
    glBindTexture(GL_TEXTURE_2D, this->gbuf_normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F​, this->viewport.w, this->viewport.h, 0, GL_RGBA, GL_UNSIGNED_INT, NULL);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, this->gbuf_normal, 0);
    
    glGenTextures(1, &this->gbuf_depth);
    glBindTexture(GL_TEXTURE_2D, this->gbuf_depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, this->viewport.w, this->viewport.h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->gbuf_normal, 0);
};

void SimKit::OpenGL::DeferredRenderer::set_render_target(SDL_Window* wnd) {
    SimKit::OpenGL::make_current(wnd, &this->ctxt);
    this->wnd = wnd;

    int w, h;
    SDL_GetWindowSize(this->wnd, &w, &h);
    this->viewport.x = 0;
    this->viewport.y = 0;
    this->viewport.w = w;
    this->viewport.h = h;
    
    this->setup_context();
};

virtual void set_render_target(SDL_Window* wnd, const SDL_Rect viewport) {
    SimKit::OpenGL::make_current(wnd, &this->ctxt);
    this->wnd = wnd;
    this->viewport = viewport;
    
    this->setup_context();
};


