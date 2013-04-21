attribute vec4 vi_ambient;
attribute vec4 vi_diffuse;
attribute vec4 vi_specular;
attribute vec4 vi_phongparams; //vec4, in order: (unused, unused, unused, shininess)

attribute vec3 vi_normal;

attribute vec2 vi_uvmap_ambient;
attribute vec2 vi_uvmap_diffuse;
attribute vec2 vi_uvmap_specular;
attribute vec2 vi_uvmap_phongparams;
attribute vec2 vi_uvmap_normal;

uniform mat4 vi_modelspace;
uniform mat4 vi_viewspace;
uniform mat4 vi_projspace;
uniform mat4 vi_modelviewrot; //equal to modelview with rotations only

attribute vec4 vi_vertex;

varying vec4 fi_vertex_ambient;
varying vec4 fi_vertex_diffuse;
varying vec4 fi_vertex_specular;
varying vec4 fi_vertex_phongparams;

varying vec4 fi_vertex_normal;

varying vec2 fi_uvmap_ambient;
varying vec2 fi_uvmap_diffuse;
varying vec2 fi_uvmap_specular;
varying vec2 fi_uvmap_phongparams;
varying vec2 fi_uvmap_normal;

out mat4 fi_modelviewrot;

void main() {
    fi_vertex_ambient = vi_ambient;
    fi_vertex_diffuse = vi_diffuse;
    fi_vertex_specular = vi_specular;
    fi_vertex_phongparams = vi_phongparams;
    fi_vertex_normal = (vi_modelviewrot * vec4(vi_normal.xyz, 1.0)).xyz;
    gl_Position = vec4(vi_projspace * vi_viewspace * vi_modelspace * vi_vertex);
    
    fi_uvmap_ambient = vi_uvmap_ambient;
    fi_uvmap_diffuse = vi_uvmap_diffuse;
    fi_uvmap_specular = vi_uvmap_specular;
    fi_uvmap_phongparams = vi_uvmap_phongparams;
    fi_uvmap_normal = vi_uvmap_normal;
};
