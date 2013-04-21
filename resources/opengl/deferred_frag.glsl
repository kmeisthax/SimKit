uniform sampler2D gbuf_ambient_color;   //Read as GL_RGB. Alpha unused (for now)
uniform sampler2D gbuf_diffuse_color;
uniform sampler2D gbuf_specular_color;
uniform sampler2D gbuf_phongparams;     //RGB components unused (for now), alpha component is shininess
uniform sampler2D gbuf_depth;           //GL_DEPTH_COMPONENT texture, will be unprojected
uniform sampler2D gbuf_normal;          //Also GL_RGB

uniform sampler2D lighting_data; //n-by-at-least-3 texture. Vertical texels in order are:
                                 // View-space position of light (x,y,z,w)
                                 // RGB diffuse color of light, diffuse falloff distance
                                 // RGB specular color of light, specular falloff distance
                                 //Further components will not be used.
                                 //Storage format should be GL_RGBA32F.
                                 //Should not have mipmapping or aniso enabled.

uniform mat4 gbuf_unprojection;  //clip to view space matrix

void main() {
    vec3 ambient_frag = texture(gbuf_ambient_color, gl_FragCoord.xy);
    vec3 diffuse_frag = texture(gbuf_diffuse_color, gl_FragCoord.xy);
    vec3 specular_frag = texture(gbuf_specular_color, gl_FragCoord.xy);
    vec3 normal_frag = texture(gbuf_normal, gl_FragCoord.xy);
    float shiny = texture(gbuf_phongparams, gl_FragCoord.xy).w;
    float depth = texture(gbuf_depth, gl_FragCoord.xy);
    
    vec4 viewspace_coord = gbuf_unprojection * vec4(gl_FragCoord.xy, depth, 1.0);
    vec3 final_frag = ambient_frag;
    
    ivec2 light_data_spec = textureSize(lighting_data, 0);
    for (int i = 0; i < light_data_spec.x; i++) { //Basic (shadowless) phong mapping
        vec4 light_position = texelFetch(lighting_data, vec2(i, 0));
        vec4 light_diffuse = texelFetch(lighting_data, vec2(i, 1));
        vec4 light_specular = texelFetch(lighting_data, vec2(i, 2));
        
        vec3 to_light_vec = normalize(viewspace_coord - light_position).xyz;
        float light_distance = length(viewspace_coord - light_position);
        
        vec3 diffuse_light = diffuse_frag * dot(to_light_vec, normal_frag) * light_diffuse.xyz * (light_distance / light_diffuse.w);
        
        vec3 reflect_vec = 2 * dot(to_light_vec, normal_frag) * (normal_frag - to_light_vec);
        vec3 to_viewer_vec = normalize(-1.0 * viewspace_coord).xyz;
        vec3 specular_light = specular_frag * pow(dot(reflect_vec, to_viewer_vec), shiny) * light_specular.xyz * (light_distance / light_specular.w);
        
        final_frag = final_frag + diffuse_light + specular_light;
    }
    
    gl_Color = final_frag;
};
