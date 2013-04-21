varying vec4 fi_vertex_ambient;
varying vec4 fi_vertex_diffuse;
varying vec4 fi_vertex_specular;
varying vec4 fi_vertex_phongparams; //vec4, in order: (unused, unused, unused, shininess)

varying vec4 fi_vertex_normal;

varying vec2 fi_uvmap_ambient;
varying vec2 fi_uvmap_diffuse;
varying vec2 fi_uvmap_specular;
varying vec2 fi_uvmap_phongparams;
varying vec2 fi_uvmap_normal;

uniform sampler2D fi_texture_ambient;
uniform sampler2D fi_texture_diffuse;
uniform sampler2D fi_texture_specular;
uniform sampler2D fi_texture_phongparams;
uniform sampler2D fi_texture_normal;

in mat4 fi_modelviewrot; //equal to modelview with rotations only

out vec4 gbuf_ambient_color;
out vec4 gbuf_diffuse_color;
out vec4 gbuf_specular_color;
out vec4 gbuf_phongparams;
out vec3 gbuf_normal;

void main() {
    gbuf_ambient_color = fi_vertex_ambient * texture(fi_texture_ambient, fi_uvmap_ambient);
    gbuf_diffuse_color = fi_vertex_diffuse * texture(fi_texture_diffuse, fi_uvmap_diffuse);
    gbuf_specular_color = fi_vertex_specular * texture(fi_texture_specular, fi_uvmap_specular);
    gbuf_phongparams_color = fi_vertex_phongparams * texture(fi_texture_phongparams, fi_uvmap_phongparams);
    gbuf_normal_color = fi_vertex_normal * (fi_modelviewrot * texture(fi_texture_normal, fi_uvmap_normal)).xyz;
};
