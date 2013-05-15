#include <simkit.h>
#include <simkit/opengl.h>

#include <string>

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

void SimKit::OpenGL::DeferredRenderer::set_material_parameter(SimKit::Material* mat, SimKit::IVMeshData* vmesh, const float mesh_quality, SimKit::Material::Parameter param, GLuint program, const char* vertex_attrib, const char* uvmap_attrib, int image_unit) {
    if (mat->get_material_mappings(param) & SimKit::Material::PT_CONSTANT) {
        float c[4];
        mat->get_material_constant(param, c, NULL);
        glVertexAttrib4f(glGetAttribLocation(program, vertex_attrib), c[1], c[2], c[3], c[4]);
    } else if (mat->get_material_mappings(param) & SimKit::Material::PT_ATTRIB) {
        std::string attrib_name;
        int attrib_id;
        
        mat->get_material_attrib(param, attrib_name, attrib_id);
        
        GLuint gl_attribdata;
        bool attrib_is_valid;
        
        if (attrib_name != "" && SimKit::OpenGL::MeshCache::sys_meshcache.request_mesh_attribs(vmesh, mesh_quality, attrib_name, this->ctxt, &gl_attribdata, &attrib_is_valid) != IVMeshData::IRequest::REQUEST_COMPLETE) {
            return; //Rendering not possible! Da.
            //TODO: Clean up the GL state properly
        } else if (SimKit::OpenGL::MeshCache::sys_meshcache.request_mesh_attribs(vmesh, mesh_quality, attrib_id, this->ctxt, &gl_attribdata, &attrib_is_valid, NULL) != IVMeshData::IRequest::REQUEST_COMPLETE) {
            return;
        }
        
        if (attrib_is_valid) {
            glEnableVertexAttribArray(glGetAttribLocation(program, vertex_attrib));
            glBindBuffer(GL_ARRAY_BUFFER, gl_attribdata);
            glVertexAttribPointer(glGetAttribLocation(program, vertex_attrib), 4, GL_FLOAT, GL_FALSE, 0, 0);
        }
    } else if (mat->get_material_mappings(param) & SimKit::Material::PT_NORMAL) {
        GLuint gl_normaldata;
        bool normal_is_valid;
        
        if (SimKit::OpenGL::MeshCache::sys_meshcache.request_mesh_normals(vmesh, mesh_quality, this->ctxt, &gl_attribdata, &normal_is_valid, NULL) != IVMeshData::IRequest::REQUEST_COMPLETE) {
            return;
        }
        
        glEnableVertexAttribArray(glGetAttribLocation(program, vertex_attrib));
        glBindBuffer(GL_ARRAY_BUFFER, gl_normaldata);
        glVertexAttribPointer(glGetAttribLocation(program, vertex_attrib), 4, GL_FLOAT, GL_FALSE, 0, 0);
    } else {
        glVertexAttrib4f(glGetAttribLocation(program, vertex_attrib), 1.0f, 1.0f, 1.0f, 1.0f);
    }
    
    if (mat->get_material_mappings(param) & SimKit::Material::PT_IMAGE) {
        std::string uvmap_name;
        int uvmap_id;
        ParamSwizzle swiz;
        IVImage* vtex;
        
        mat->get_material_image(param, uvmap_name, &uvmap_id, &swiz, &vtex, NULL);
        
        GLuint gl_uvmapdata;
        bool uvmap_is_valid;
        
        if (uvmap_name != "" && SimKit::OpenGL::MeshCache::sys_meshcache.request_mesh_uvmaps(vmesh, mesh_quality, uvmap_name, this->ctxt, &gl_uvmapdata, &uvmap_is_valid, NULL) != SimKit::IVMeshData::IRequest::REQUEST_COMPLETE) {
            return;
        } else if (SimKit::OpenGL::MeshCache::sys_meshcache.request_mesh_uvmaps(vmesh, mesh_quality, uvmap_id, this->ctxt, &gl_uvmapdata, &uvmap_is_valid, NULL) != IVMeshData::IRequest::REQUEST_COMPLETE) {
            return;
        }
        
        GLuint gputex;
        SDL_Rect reqrect;
        float reqdpi;
        
        //TODO: more intelligent request rectangle and DPI selection
        vtex->get_image_size(&reqrect->w, &reqrect->h, &reqdpi, NULL);
        reqrect->x = 0;
        reqrect->y = 0;
        
        SDL_Rect texrect;
        float texdpi;
        
        if (SimKit::OpenGL::TextureCache::sys_texcache.request_texture_load(vtex, reqrect, reqdpi, this->ctxt, &gputex, &texrect, &texdpi) != SimKit::IVImage::IRequest::REQUEST_COMPLETE) {
            //Can't use this material right now
            return;
        }
        
        if (uvmap_is_valid) {
            glEnableVertexAttribArray(glGetAttribLocation(program, uvmap_attrib));
            glBindBuffer(GL_ARRAY_BUFFER, gl_uvmapdata);
            glVertexAttribPointer(glGetAttribLocation(program, uvmap_attrib), 2, GL_FLOAT, GL_FALSE, 0, 0);
            
            glActiveTexture(GL_TEXTURE0 + 0);
            glBindTexture(GL_TEXTURE2D, gputex);
        }
    }
}

void SimKit::OpenGL::DeferredRenderer::process_renderable(SimKit::Material* mat, SimKit::IVMeshData* vmesh) {
    float mesh_quality; //in the future: support mesh LODs. Currently we just use the max quality parameter.
    vmesh->get_quality_range(NULL, &mesh_quality, NULL);
    
    if (mat) {
        this->set_material_parameter(mat, vmesh, mesh_quality, SimKit::Material::PARAM_AMBIENT, this->gbuf_program, "vi_ambient", "vi_uvmap_ambient", 0);
        this->set_material_parameter(mat, vmesh, mesh_quality, SimKit::Material::PARAM_DIFFUSE, this->gbuf_program, "vi_diffuse", "vi_uvmap_diffuse", 1);
        this->set_material_parameter(mat, vmesh, mesh_quality, SimKit::Material::PARAM_SPECULAR, this->gbuf_program, "vi_specular", "vi_uvmap_specular", 2);
        this->set_material_parameter(mat, vmesh, mesh_quality, SimKit::Material::PARAM_SHININESS, this->gbuf_program, "vi_phongparams", "vi_uvmap_phongparams", 3);
        this->set_material_parameter(mat, vmesh, mesh_quality, SimKit::Material::PARAM_NORMAL, this->gbuf_program, "vi_normal", "vi_uvmap_normal", 4);
    } else {    //No material? Render everything white!
        glVertexAttrib4f(glGetAttribLocation(this->gbuf_program, "vi_ambient"), 1.0f, 1.0f, 1.0f, 1.0f);
        glVertexAttrib4f(glGetAttribLocation(this->gbuf_program, "vi_diffuse"), 1.0f, 1.0f, 1.0f, 1.0f);
        glVertexAttrib4f(glGetAttribLocation(this->gbuf_program, "vi_specular"), 1.0f, 1.0f, 1.0f, 1.0f);
        glVertexAttrib4f(glGetAttribLocation(this->gbuf_program, "vi_phongparams"), 1.0f, 1.0f, 1.0f, 1.0f);
        glVertexAttrib3f(glGetAttribLocation(this->gbuf_program, "vi_normal"), 1.0f, 1.0f, 1.0f);
        glVertexAttrib2f(glGetAttribLocation(this->gbuf_program, "vi_uvmap_ambient"), 1.0f, 1.0f);
        glVertexAttrib2f(glGetAttribLocation(this->gbuf_program, "vi_uvmap_diffuse"), 1.0f, 1.0f);
        glVertexAttrib2f(glGetAttribLocation(this->gbuf_program, "vi_uvmap_specular"), 1.0f, 1.0f);
        glVertexAttrib2f(glGetAttribLocation(this->gbuf_program, "vi_uvmap_phongparams"), 1.0f, 1.0f);
        glVertexAttrib2f(glGetAttribLocation(this->gbuf_program, "vi_uvmap_normal"), 1.0f, 1.0f);
    }
    
    GLuint gverts, gindicies;
    int vertcnt, indexcnt;
    
    GLenum gprimtype;
    SimKit::IVMeshData::IRequest::PrimType primtype,
    
    if (SimKit::OpenGL::MeshCache::sys_meshcache.request_mesh_verticies(vmesh, mesh_quality, this->ctxt, &gverts, &vertcnt) != SimKit::IVMeshData::IRequest::REQUEST_COMPLETE) {
        return; //can't process renderable today
    }
    
    if (SimKit::OpenGL::MeshCache::sys_meshcache.request_mesh_indicies(vmesh, mesh_quality, this->ctxt, &gindicies, &indexcnt, &primtype) != SimKit::IVMeshData::IRequest::REQUEST_COMPLETE) {
        return; //you will not render to space today
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gindicies);
    glBindBuffer(GL_ARRAY_BUFFER, gverts);
    
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(vertcnt);
    
    switch (primtype) {
        default:
        case SimKit::IVMeshData::IRequest::TRIANGLES:
            gprimtype = GL_TRIANGLES;
            break;
        case SimKit::IVMeshData::IRequest::TRIANGLE_STRIPS:
            gprimtype = GL_TRIANGLE_STRIP;
            break;
        case SimKit::IVMeshData::IRequest::TRIANGLE_FANS:
            gprimtype = GL_TRIANGLE_FAN;
            break;
    }
    
    glDrawElements(gprimtype, indexcnt, GL_UNSIGNED_INT, NULL);
};
