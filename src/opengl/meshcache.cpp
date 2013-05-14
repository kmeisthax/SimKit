#include <simkit.h>
#include <simkit/opengl.h>

#include <string>

SimKit::OpenGL::MeshCache::MeshCache() : {};
SimKit::OpenGL::MeshCache::~MeshCache() {};

GLuint SimKit::OpenGL::MeshCache::upload_data(SDL_GLContext ctxt, const float* data, const int count, const SimKit::TMeshCache<SDL_GLContext, GLuint, MeshCache>::MeshDataType type, const SimKit::IVMeshData::Usage usage, bool* out_success) {
    int err = SimKit::OpenGL::make_current(ctxt);
    
    if (err != 0) {
        std::string err = "Failed to make context current when uploading mesh data.\n";
        SimKit::SDLEmergencyError(err);
        if (out_success) *out_success = FALSE;
        return 0;
    }
    
    GLuint out_buf;
    glGenBuffers(1, &out_buf);
    
    GLenum bind_type = GL_ARRAY_BUFFER;
    switch (type) {
        case SimKit::TMeshCache<SDL_GLContext, GLuint, MeshCache>::INDEX_DATA:
            bind_type = GL_ELEMENT_ARRAY_BUFFER;
        default:
            break;
    }
    
    glBindBuffer(bind_type, out_buf);
    
    GLenum gl_usage;
    switch (usage) {
        case SimKit::IVMeshData::STATIC:
        default:
            gl_usage = GL_STATIC_DRAW;
            break;
        case SimKit::IVMeshData::DYNAMIC:
            gl_usage = GL_DYNAMIC_DRAW;
            break;
        case SimKit::IVMeshData::STREAM:
            gl_usage = GL_STREAM_DRAW;
            break;
    }
    
    glBufferData(bind_type, count * sizeof(float), data, gl_usage);
    glBindBuffer(bind_type, 0);
    if (out_success) *out_success = TRUE;
    return out_buf;
};

GLuint SimKit::OpenGL::MeshCache::upload_data(SDL_GLContext ctxt, const int* data, const int count, const SimKit::TMeshCache<SDL_GLContext, GLuint, MeshCache>::MeshDataType type, const SimKit::IVMeshData::Usage usage, bool* out_success) {
    int err = SimKit::OpenGL::make_current(ctxt);
    
    if (err != 0) {
        std::string err = "Failed to make context current when uploading mesh data.\n";
        SimKit::SDLEmergencyError(err);
        if (out_success) *out_success = FALSE;
        return 0;
    }
    
    GLuint out_buf;
    glGenBuffers(1, &out_buf);
    
    GLenum bind_type = GL_ARRAY_BUFFER;
    switch (type) {
        case SimKit::TMeshCache<SDL_GLContext, GLuint, MeshCache>::INDEX_DATA:
            bind_type = GL_ELEMENT_ARRAY_BUFFER;
        default:
            break;
    }
    
    glBindBuffer(bind_type, out_buf);
    
    GLenum gl_usage;
    switch (usage) {
        case SimKit::IVMeshData::STATIC:
        default:
            gl_usage = GL_STATIC_DRAW;
            break;
        case SimKit::IVMeshData::DYNAMIC:
            gl_usage = GL_DYNAMIC_DRAW;
            break;
        case SimKit::IVMeshData::STREAM:
            gl_usage = GL_STREAM_DRAW;
            break;
    }
    
    glBufferData(bind_type, count * sizeof(int), data, gl_usage);
    glBindBuffer(bind_type, 0);
    if (out_success) *out_success = TRUE;
    return out_buf;
};

GLuint SimKit::OpenGL::MeshCache::upload_data(SDL_GLContext ctxt, const float* data, const int count, const SimKit::TMeshCache<SDL_GLContext, GLuint, MeshCache>::MeshDataType type, const SimKit::IVMeshData::Usage usage, GLuint old_data, bool* out_success) {
    int err = SimKit::OpenGL::make_current(ctxt);
    
    if (err != 0) {
        std::string err = "Failed to make context current when uploading mesh data.\n";
        SimKit::SDLEmergencyError(err);
        if (out_success) *out_success = FALSE;
        return 0;
    }
    
    GLuint out_buf;
    if (glIsBuffer(old_data)) out_buf = old_data;
    else glGenBuffers(1, &out_buf);
    
    GLenum bind_type = GL_ARRAY_BUFFER;
    switch (type) {
        case SimKit::TMeshCache<SDL_GLContext, GLuint, MeshCache>::INDEX_DATA:
            bind_type = GL_ELEMENT_ARRAY_BUFFER;
        default:
            break;
    }
    
    glBindBuffer(bind_type, out_buf);
    
    GLenum gl_usage;
    switch (usage) {
        default:
        case SimKit::IVMeshData::STATIC:
            gl_usage = GL_STATIC_DRAW;
            break;
        case SimKit::IVMeshData::DYNAMIC:
            gl_usage = GL_DYNAMIC_DRAW;
            break;
        case SimKit::IVMeshData::STREAM:
            gl_usage = GL_STREAM_DRAW;
            break;
    }
    
    glBufferData(bind_type, count * sizeof(float), data, gl_usage);
    glBindBuffer(bind_type, 0);
    if (out_success) *out_success = TRUE;
    return out_buf;
};

GLuint SimKit::OpenGL::MeshCache::upload_data(SDL_GLContext ctxt, const int* data, const int count, const SimKit::TMeshCache<SDL_GLContext, GLuint, MeshCache>::MeshDataType type, const SimKit::IVMeshData::Usage usage, GLuint old_data, bool* out_success) {
    int err = SimKit::OpenGL::make_current(ctxt);

    if (err != 0) {
        std::string err = "Failed to make context current when uploading mesh data.\n";
        SimKit::SDLEmergencyError(err);
        if (out_success) *out_success = FALSE;
        return 0;
    }
    
    GLuint out_buf;
    if (glIsBuffer(old_data)) out_buf = old_data;
    else glGenBuffers(1, &out_buf);
    
    GLenum bind_type = GL_ARRAY_BUFFER;
    switch (type) {
        case SimKit::TMeshCache<SDL_GLContext, GLuint, MeshCache>::INDEX_DATA:
            bind_type = GL_ELEMENT_ARRAY_BUFFER;
        default:
            break;
    }
    
    glBindBuffer(bind_type, out_buf);
    
    GLenum gl_usage;
    switch (usage) {
        case SimKit::IVMeshData::STATIC:
        default:
            gl_usage = GL_STATIC_DRAW;
            break;
        case SimKit::IVMeshData::DYNAMIC:
            gl_usage = GL_DYNAMIC_DRAW;
            break;
        case SimKit::IVMeshData::STREAM:
            gl_usage = GL_STREAM_DRAW;
            break;
    }
    
    glBufferData(bind_type, count * sizeof(int), data, gl_usage);
    glBindBuffer(bind_type, 0);
    if (out_success) *out_success = TRUE;
    return out_buf;
};

void SimKit::OpenGL::MeshCache::destroy_data(SDL_GLContext ctxt, GLuint old_data) {
    int err = SimKit::OpenGL::make_current(ctxt);
    
    if (err != 0) {
        std::string err = "Failed to make context current when deleting mesh data.\n";
        SimKit::SDLEmergencyError(err);
        return;
    }
    
    glDeleteBuffers(1, &old_data);
};

SimKit::OpenGL::MeshCache SimKit::OpenGL::MeshCache::sys_meshcache;
