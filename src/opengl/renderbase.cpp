#include <simkit.h>
#include <simkit/opengl.h>
#include <glload/gl_2_1.h>

/* Because of the lunacy that is OpenGL ES, there are two different parallel
 * versions of GLSL which are ALMOST equivalent! The differences are that GLES
 * numbers GLSL versions incorrectly, and requires precision hints that the
 * desktop version of OpenGL doesn't even support.
 * 
 * To avoid writing duplicate shaders we append these strings to the beginning
 * of every loaded shader here. This creates a unified GLSL environment.
 */
static const char* v_version_header_null = "\n";
static const char* v_version_header_desktop = "#version 120\n#define lowp\n#define highp\n#define mediump\n";
static const char* v_version_header_mobile  = "#version 100\n";

GLuint SimKit::OpenGL::RenderBase::compile_shader(GLenum type, std::istream* vi) {
    GLuint out = glCreateShader(type);
    
    vi->seekg(0, std::ios::end);
    int sourcelen = vi->tellg();
    
    #ifdef OPENGL_ES
    const char* v_version_header = v_version_header_mobile;
    #else
    const char* v_version_header = v_version_header_desktop;
    #endif
    
    char* v_source[2] = {(char*)v_version_header, new char[sourcelen]};
    
    vi->seekg(0, std::ios::beg);
    vi->read(v_source[1], sourcelen);
    v_source[1][sourcelen-1] = 0;
    
    glShaderSource(out, 2, v_source, NULL);
    glCompileShader(out);
    
    int status;
    glGetShaderiv(out, GL_COMPILE_STATUS, &status);
    if (status == FALSE) {
        std::string err = "GL shader failed to compile!\n";
		
		int logsize = 0;
		glGetShaderiv(out, GL_INFO_LOG_LENGTH, &logsize);
		char* infolog = new char[logsize+1];
		infolog[logsize] = 0;
		glGetShaderInfoLog(out, logsize, &logsize, infolog);
        err += infolog;
		delete[] infolog;
        
        err += "Resource name: ";
        err += name;
        err += "\n";
        
        err += v_source[0];
        err += v_source[1];
        err += "\n";
		
		SimKit::EmergencyError(err);
    }
    
    delete[] v_source[1];
    
    return out;
};

SimKit::OpenGL::RenderBase::HardwareMode SimKit::OpenGL::RenderBase::get_gl_mode() {
    #ifdef OPENGL_ES
        return SimKit::OpenGL::RenderBase::GL_ES;
    #else
        return SimKit::OpenGL::RenderBase::GL_DESKTOP;
    #endif
};
