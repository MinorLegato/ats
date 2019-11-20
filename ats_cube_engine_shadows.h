#pragma once

#define OPENGL_MODERN
#include "../ats/ats_platform_glfw.h"
#include "../ats/ats_shaders.h"

#define SHADOW_WIDTH    (2048) 
#define SHADOW_HEIGHT   (2048) 

typedef struct {
    float   position[4];
    float   normal[4];
    float   color[4];
    float   texture[2];
} Vertex;

// Layout:  vertex : normal : color : texture
static const Vertex vertex_array_cube[] = {
    // back face
    { { -1.0f, -1.0f, -1.0f, 1.0f },  { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }, // bottom-left
    { {  1.0f,  1.0f, -1.0f, 1.0f },  { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-right
    { {  1.0f, -1.0f, -1.0f, 1.0f },  { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // bottom-right         
    { {  1.0f,  1.0f, -1.0f, 1.0f },  { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-right
    { { -1.0f, -1.0f, -1.0f, 1.0f },  { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }, // bottom-left
    { { -1.0f,  1.0f, -1.0f, 1.0f },  { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // top-left
    // front face
    { { -1.0f, -1.0f,  1.0f, 1.0f },  { 0.0f,  0.0f,  1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }, // bottom-left
    { {  1.0f, -1.0f,  1.0f, 1.0f },  { 0.0f,  0.0f,  1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // bottom-right
    { {  1.0f,  1.0f,  1.0f, 1.0f },  { 0.0f,  0.0f,  1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-right
    { {  1.0f,  1.0f,  1.0f, 1.0f },  { 0.0f,  0.0f,  1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-right
    { { -1.0f,  1.0f,  1.0f, 1.0f },  { 0.0f,  0.0f,  1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // top-left
    { { -1.0f, -1.0f,  1.0f, 1.0f },  { 0.0f,  0.0f,  1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }, // bottom-left
    // left face
    { { -1.0f,  1.0f,  1.0f, 1.0f },  { -1.0f,  0.0f,  0.0f, 0.0f },  { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // top-right
    { { -1.0f,  1.0f, -1.0f, 1.0f },  { -1.0f,  0.0f,  0.0f, 0.0f },  { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-left
    { { -1.0f, -1.0f, -1.0f, 1.0f },  { -1.0f,  0.0f,  0.0f, 0.0f },  { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // bottom-left
    { { -1.0f, -1.0f, -1.0f, 1.0f },  { -1.0f,  0.0f,  0.0f, 0.0f },  { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // bottom-left
    { { -1.0f, -1.0f,  1.0f, 1.0f },  { -1.0f,  0.0f,  0.0f, 0.0f },  { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }, // bottom-right
    { { -1.0f,  1.0f,  1.0f, 1.0f },  { -1.0f,  0.0f,  0.0f, 0.0f },  { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // top-right
    // right face
    { { 1.0f,  1.0f,  1.0f, 1.0f },   { 1.0f,  0.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // top-left
    { { 1.0f, -1.0f, -1.0f, 1.0f },   { 1.0f,  0.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // bottom-right
    { { 1.0f,  1.0f, -1.0f, 1.0f },   { 1.0f,  0.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-right         
    { { 1.0f, -1.0f, -1.0f, 1.0f },   { 1.0f,  0.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // bottom-right
    { { 1.0f,  1.0f,  1.0f, 1.0f },   { 1.0f,  0.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // top-left
    { { 1.0f, -1.0f,  1.0f, 1.0f },   { 1.0f,  0.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }, // bottom-left     
    // bottom face
    { { -1.0f, -1.0f, -1.0f, 1.0f },  { 0.0f, -1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // top-right
    { {  1.0f, -1.0f, -1.0f, 1.0f },  { 0.0f, -1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-left
    { {  1.0f, -1.0f,  1.0f, 1.0f },  { 0.0f, -1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // bottom-left
    { {  1.0f, -1.0f,  1.0f, 1.0f },  { 0.0f, -1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // bottom-left
    { { -1.0f, -1.0f,  1.0f, 1.0f },  { 0.0f, -1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }, // bottom-right
    { { -1.0f, -1.0f, -1.0f, 1.0f },  { 0.0f, -1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // top-right
    // top face
    { { -1.0f,  1.0f, -1.0f, 1.0f },  { 0.0f,  1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // top-left
    { {  1.0f,  1.0f , 1.0f, 1.0f },  { 0.0f,  1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // bottom-right
    { {  1.0f,  1.0f, -1.0f, 1.0f },  { 0.0f,  1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-right     
    { {  1.0f,  1.0f,  1.0f, 1.0f },  { 0.0f,  1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // bottom-right
    { { -1.0f,  1.0f, -1.0f, 1.0f },  { 0.0f,  1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // top-left
    { { -1.0f,  1.0f,  1.0f, 1.0f },  { 0.0f,  1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }  // bottom-left  
};

// Layout:  vertex : normal
static const Vertex vertex_array_square[] = {
    { { -1.0f, -1.0f, 0.0f, 1.0f },     { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }, // bottom-left
    { {  1.0f,  1.0f, 0.0f, 1.0f },     { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-right
    { {  1.0f, -1.0f, 0.0f, 1.0f },     { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // bottom-right         
    { {  1.0f,  1.0f, 0.0f, 1.0f },     { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-right
    { { -1.0f, -1.0f, 0.0f, 1.0f },     { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }, // bottom-left
    { { -1.0f,  1.0f, 0.0f, 1.0f },     { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // top-left
};

static const char* ce_shader_vertex = GLSL_SHADER(
    layout (location = 0) in vec4 vp;
    layout (location = 1) in vec4 np;
    layout (location = 2) in vec4 vc;
    layout (location = 3) in vec2 tx;
    layout (location = 4) in vec4 C;
    layout (location = 5) in mat4 M;

    uniform mat4 PV;

    out vec4 color;

    void main() {
        gl_Position = PV * M * vp;
        color       = C;
    }
);

static const char* ce_shader_fragment = GLSL_SHADER(
    out vec4 frag_color;
    in  vec4 color;
    
    void main() {
        frag_color            = color;
    }
);

static const char* ce_shader_vertex_light = GLSL_SHADER(
    layout (location = 0) in vec4 vp;
    layout (location = 1) in vec4 np;
    layout (location = 2) in vec4 vc;
    layout (location = 3) in vec2 tx;
    layout (location = 4) in vec4 C;
    layout (location = 5) in mat4 M;

    out vec3 frag_pos;
    out vec3 normal;
    out vec2 tex_coords;
    out vec4 frag_pos_light_space;
    out vec4 color;

    uniform mat4 PV;
    uniform mat4 light_space_matrix;

    void main() {
        vec4 mpos               = M * vp;
        gl_Position             = PV * mpos;
        frag_pos                = mpos.xyz;
        normal                  = normalize((M * np).xyz);
        tex_coords              = tx;
        frag_pos_light_space    = light_space_matrix * vec4(frag_pos, 1.0);
        color                   = C * vc;
    }
);

static const char* ce_shader_fragment_light = GLSL_SHADER(
    out vec4 frag_color;

    in vec3 frag_pos;
    in vec3 normal;
    in vec2 tex_coords;
    in vec4 frag_pos_light_space;
    in vec4 color;

    uniform sampler2D shadow_map;
    uniform sampler2D color_map;

    uniform vec3 light_pos;
    uniform vec3 view_pos;
    //
    uniform vec3 light_color;

    float shadowCalculation(vec3 proj_coords) {
        if(proj_coords.z > 1.0)
            return 0.0;

        float current_depth = proj_coords.z;

        vec3 normal     = normalize(normal);
        vec3 light_dir  = normalize(light_pos - frag_pos);
        float bias      = max(0.05 * (1.0 - dot(normal, light_dir)), 0.005);

        float shadow = current_depth - bias > texture(shadow_map, proj_coords.xy).r? 1.0 : 0.0;

        return shadow;
    }

    vec4 colorCalculation(vec3 proj_coords) {
        vec4 closest_depth  = texture(color_map, proj_coords.xy);
        return closest_depth;
    }
   
    void main() {
        float   ambient_str     = 0.2;
        vec3    ambient         = ambient_str * light_color;
        vec3    light_dir       = normalize(light_pos - frag_pos);
        float   diff            = max(dot(normal, light_dir), 0.0);
        vec3    diffuse         = diff * light_color;
        float   specular_str    = 0.1;
        vec3    view_dir        = normalize(view_pos - frag_pos);
        vec3    reflect_dir     = reflect(-light_dir, normal);
        float   spec            = pow(max(dot(view_dir, reflect_dir), 0.0), 2);
        vec3    specular        = specular_str * spec * light_color;

        vec3 proj_coords = 0.5 * (frag_pos_light_space.xyz / frag_pos_light_space.w) + 0.5;

        float   shadow          = shadowCalculation(proj_coords);
        vec4    caster          = colorCalculation(proj_coords);

        vec3 result = color.rgb * (ambient + (1.0 - 0.7 * caster.a * shadow) * (diffuse + specular));

        frag_color = vec4(result, color.a);
    }
);

static const char* ce_depth_vertex = GLSL_SHADER(
    layout (location = 0) in vec4 vp;
    layout (location = 4) in vec4 C;
    layout (location = 5) in mat4 M;

    out vec4 color;

    uniform mat4 light_space_matrix;

    void main() {
        gl_Position = light_space_matrix * M * vp;
        color       = C;
    }
);

static const char* ce_depth_fragment = GLSL_SHADER(
    in  vec4 color;
    out vec4 frag_color;

    void main() {
        gl_FragDepth    = gl_FragCoord.z;
        frag_color      = color;
    }
);

// ----------------------------------------------------------------------------------------------- //

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb/stb_image.h" 

typedef struct Texture {
    unsigned    id;
    int         width;
    int         height;
} Texture;

static Texture textureLoad(const char* texture_path, int is_smooth) {
    Texture         texture     = {0};
    int             channels    = 0;
    unsigned char*  pixels      = NULL;

    pixels = stbi_load(texture_path, &texture.width, &texture.height, &channels, 0);

    assert(pixels);

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(pixels);

    return texture;
}

static void textureBind(const Texture* texture) {
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

// ----------------------------------------------------------------------------------------------- //

static int ce_shader;
static int ce_shader_no_light;
static int ce_shader_depth;

static unsigned ce_vao_cube;
static unsigned ce_vbo_cube;
static unsigned ce_vbo_cube_data;

static unsigned ce_depth_fbo;
static unsigned ce_depth_map;

static unsigned ce_color_map;

typedef struct RObject {
    float     color[4];
    float     model[16];
} RObject;

static float ce_projection_view[16]    = { 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f };
static float ce_light_space_matrix[16] = { 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f };
static float ce_view_pos[3]            = { 0.0f, 0.0f, 0.0f };
static float ce_light_pos[3]           = { 0.0f, 0.0f, 0.0f };
static float ce_light_color[3]         = { 1.0f, 1.0f, 1.0f };

static int     render_count;
static RObject render_queue[1024 * 1024];

void ceSetLight(float x, float y, float z, float r, float g, float b) {
    ce_light_pos[0] = x;
    ce_light_pos[1] = y;
    ce_light_pos[2] = z;

    ce_light_color[0] = r;
    ce_light_color[1] = g;
    ce_light_color[2] = b;
}

void ceSetView(
        float pos_x,  float pos_y,  float pos_z,
        float look_x, float look_y, float look_z,
        float up_x,   float up_y,   float up_z,
        float fov,
        float near_plane,
        float far_plane) {
    float pos[3]  = { pos_x,  pos_y,  pos_z  };
    float look[3] = { look_x, look_y, look_z };
    float up[3]   = { up_x,   up_y,   up_z   };

    Float4x4PerspectiveView(ce_projection_view, pos, look, up, fov, platform.aspect_ratio, near_plane, far_plane);

    ce_view_pos[0] = pos[0];
    ce_view_pos[1] = pos[1];
    ce_view_pos[2] = pos[2];
}

void ceSetView2D(
        float pos_x,  float pos_y,  float pos_z,
        float look_x, float look_y, float look_z,
        float up_x,   float up_y,   float up_z,
        float rad_x,  float rad_y,
        float near_plane,
        float far_plane) {
    float pos[3]  = { pos_x,  pos_y,  pos_z  };
    float look[3] = { look_x, look_y, look_z };
    float up[3]   = { up_x,   up_y,   up_z   };
    float rad[2]  = { rad_x, rad_y };

    Float4x4OrthoView(ce_projection_view, pos, look, up, rad, near_plane, far_plane);

    ce_view_pos[0] = pos[0];
    ce_view_pos[1] = pos[1];
    ce_view_pos[2] = pos[2];
}

void ceSetLightView(
        float pos_x,  float pos_y,  float pos_z,
        float look_x, float look_y, float look_z,
        float up_x,   float up_y,   float up_z,
        float fov,
        float near_plane,
        float far_plane) {
    float pos[3]  = { pos_x,  pos_y,  pos_z  };
    float look[3] = { look_x, look_y, look_z };
    float up[3]   = { up_x,   up_y,   up_z   };

    float light_projection[16];
    float light_view[16];

    Float4x4Perspective(light_projection, TO_RAD(fov), platform.aspect_ratio, near_plane, far_plane);
    Float4x4LookAt(light_view, pos, look, up);
    Float4x4Mul(ce_light_space_matrix, light_projection, light_view);
}

void ceSetVertexArray(const Vertex* vertex_array, int vertex_count) {
    glBindBuffer(GL_ARRAY_BUFFER, ce_vbo_cube);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof *vertex_array, vertex_array, GL_STATIC_DRAW);
}

void ceSetRObjectArray(const RObject* robj_array, int robj_count) {
    glBindBuffer(GL_ARRAY_BUFFER, ce_vbo_cube_data);
    glBufferData(GL_ARRAY_BUFFER, robj_count * sizeof *robj_array, robj_array, GL_STATIC_DRAW);
}

void ceRender(int type, const Vertex* vertex_array, int vertex_count, const RObject* robj_array, int robj_count) {
    ceSetVertexArray(vertex_array, vertex_count);
    ceSetRObjectArray(robj_array, robj_count);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // depth render:
    {
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);

        ShaderUse(ce_shader_depth);
        ShaderSetMat4(ce_shader_depth, "light_space_matrix", ce_light_space_matrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, ce_depth_fbo);

        unsigned int attachments[] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT };
        glDrawBuffers(2, attachments);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawArraysInstanced(type, 0, vertex_count, robj_count);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    glCullFace(GL_BACK);
    // normal render:
    {
        glEnable(GL_BLEND);
        glEnable(GL_ALPHA_TEST);

        glViewport(0, 0, platform.width, platform.height);

        ShaderUse(ce_shader);

        ShaderSetMat4(ce_shader,  "light_space_matrix", ce_light_space_matrix);
        ShaderSetMat4(ce_shader,  "PV",                 ce_projection_view);
        ShaderSet3v(ce_shader,    "view_pos",           ce_view_pos);
        ShaderSet3v(ce_shader,    "light_pos",          ce_light_pos);
        ShaderSet3v(ce_shader,    "light_color",        ce_light_color);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ce_depth_map);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ce_color_map);

        glDrawArraysInstanced(type, 0, vertex_count, robj_count);
    }

    glDisable(GL_CULL_FACE);
}

void ceRenderCubes(void) {
    ceRender(GL_TRIANGLES, vertex_array_cube, 36, render_queue, render_count);
    render_count = 0;
}

void ceRenderNoLight(int type, const Vertex* vertex_array, int vertex_count, const RObject* robj_array, int robj_count) {
    // normal render:
    {
        glViewport(0, 0, platform.width, platform.height);

        ShaderUse(ce_shader_no_light);

        ShaderSetMat4(ce_shader, "PV", ce_projection_view);

        // render:
        ceSetVertexArray(vertex_array, vertex_count);
        ceSetRObjectArray(robj_array, robj_count);

        glDrawArraysInstanced(type, 0, vertex_count, robj_count);
    }
}

void ceRenderSquares() {
    ceRenderNoLight(GL_TRIANGLES, vertex_array_square, 6, render_queue, render_count);
    render_count = 0;
}

void cePushCube(float pos_x, float pos_y, float pos_z, float scale, float r, float g, float b, float a) {
    RObject robj = {
        { r, g, b, a },
        // model:
        {
            scale,  0.0f,   0.0f,   0.0f,
            0.0f,   scale,  0.0f,   0.0f,
            0.0f,   0.0f,   scale,  0.0f,
            pos_x,  pos_y,  pos_z,  1.0f
        }
    };

    render_queue[render_count++] = robj;
}

void cePushCubeQuat(float pos_x, float pos_y, float pos_z, float scale, float qx, float qy, float qz, float qw, float r, float g, float b, float a) {
    float quat[4] = { qx, qy, qz, qw }, rot[16];

    RObject robj = {
        { r, g, b, a },
        // model:
        {
            scale,  0.0f,   0.0f,   0.0f,
            0.0f,   scale,  0.0f,   0.0f,
            0.0f,   0.0f,   scale,  0.0f,
            pos_x,  pos_y,  pos_z,  1.0f
        }
    };

    Float4x4FromQuat(rot, quat);
    Float4x4Mul(robj.model, robj.model, rot);
    //
    render_queue[render_count++] = robj;
}

void cePushBox(float pos_x, float pos_y, float pos_z, float rad_x, float rad_y, float rad_z, float r, float g, float b, float a) {
    RObject robj = {
        { r, g, b, a },
        // model:
        {
            rad_x,  0.0f,   0.0f,   0.0f,
            0.0f,   rad_y,  0.0f,   0.0f,
            0.0f,   0.0f,   rad_z,  0.0f,
            pos_x,  pos_y,  pos_z,  1.0f
        }
    };

    render_queue[render_count++] = robj;
}

void cePushCubeRot(float pos_x, float pos_y, float pos_z, float scale, float rot, float r, float g, float b, float a) {
    float s = sinf(rot);
    float c = cosf(rot);

    RObject robj = {
        { r, g, b, a },
        {
             c * scale, s * scale,  0.0f,   0.0f,
            -s * scale, c * scale,  0.0f,   0.0f,
             0.0f,      0.0f,       scale,  0.0f,
             pos_x,     pos_y,      pos_z,  1.0f
        }
    };

    render_queue[render_count++] = robj;
}

void cePushCubeRotMat(float pos_x, float pos_y, float pos_z, float scale, float Rxx, float Rxy, float Ryx, float Ryy, float r, float g, float b, float a) {
    RObject robj = {
        { r, g, b, a },
        {
            Rxx * scale,    Rxy * scale,    0.0f,       0.0f,
            Ryx * scale,    Ryy * scale,    0.0f,       0.0f,
            0.0f,           0,              scale,      0.0f,
            pos_x,          pos_y,          pos_z,      1.0f
        }
    };

    render_queue[render_count++] = robj;
}

void cePushBoxRot(float pos_x, float pos_y, float pos_z, float rad_x, float rad_y, float rad_z, float rot, float r, float g, float b, float a) {
    float s = sinf(rot);
    float c = cosf(rot);

    RObject robj = {
        { r, g, b, a },
        // model:
        {
             c * rad_x, s * rad_x,  0.0f,   0.0f,
            -s * rad_y, c * rad_y,  0.0f,   0.0f,
             0.0f,      0.0f,       rad_z,  0.0f,
             pos_x,     pos_y,      pos_z,  1.0f
        }
    };

    render_queue[render_count++] = robj;
}

void cePushBoxRotMat(float pos_x, float pos_y, float pos_z, float rad_x, float rad_y, float rad_z, float Rxx, float Rxy, float Ryx, float Ryy, float r, float g, float b, float a) {
    RObject robj = {
        { r, g, b, a },
        // model:
        {
            Rxx * rad_x,    Rxy * rad_x,    0.0f,   0.0f,
            Ryx * rad_y,    Ryy * rad_y,    0.0f,   0.0f,
            0.0f,           0.0f,           rad_z,  0.0f,
            pos_x,          pos_y,          pos_z,  1.0f
        }
    };

    render_queue[render_count++] = robj;
}

void cePushLine(const float p0[2], const float p1[2], float z, float rad, const float color[4]) {
    float line[2]     = { p1[0] - p0[0], p1[1] - p0[1] };
    float line_length = Float2Len(line);
    float line_pos[2] = { p0[0] + line[0] * 0.5f, p0[1] + line[1] * 0.5f };
    float axis[2]     = { 0.0f, -1.0f };
    float rot         = Float2GetAngle(axis, line);
    float pos[3]      = { line_pos[0], line_pos[1], z };
    float scale[3]    = { rad, 0.5f * line_length, rad };

    cePushBoxRot(pos[0], pos[1], pos[2], scale[0], scale[1], scale[2], rot, color[0], color[1], color[2], color[3]);
}

void cePushLineLoop(const float pos[2], float z, float rad, const float color[4], const float (*point_array)[2], int point_count) {
    float a[2], b[2];

    for (int i = 0; i < point_count; ++i) {
        int j = (i + 1) % point_count;

        a[0] = pos[0] + point_array[i][0];
        a[1] = pos[1] + point_array[i][1];

        b[0] = pos[0] + point_array[j][0];
        b[1] = pos[1] + point_array[j][1];

        cePushLine(a, b, z, rad, color);
    }
}

void ceInit(void) {
    ce_shader           = ShaderLoadFromMemory(ce_shader_vertex_light, ce_shader_fragment_light, NULL);
    ce_shader_no_light  = ShaderLoadFromMemory(ce_shader_vertex, ce_shader_fragment, NULL);

    // init opengl:
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0f);
        glDepthFunc(GL_LESS);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        glAlphaFunc(GL_GREATER, 0.0);
        glEnable(GL_ALPHA_TEST);
    }

    ShaderUse(ce_shader);

    // setup vbo/vao:
    {
        glGenVertexArrays(1, &ce_vao_cube);
        glGenBuffers(1, &ce_vbo_cube);
        glGenBuffers(1, &ce_vbo_cube_data);

        glBindVertexArray(ce_vao_cube);

        // cube:
        {
            glBindBuffer(GL_ARRAY_BUFFER, ce_vbo_cube);
            //
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);

            // position : normal : color
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)offsetof(Vertex, position));
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)offsetof(Vertex, normal));
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)offsetof(Vertex, color));
            glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)offsetof(Vertex, texture));
        }

        // cube models:
        {
            glBindBuffer(GL_ARRAY_BUFFER, ce_vbo_cube_data);

            // color:
            glEnableVertexAttribArray(4);

            glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, sizeof (RObject), (void*)(offsetof(RObject, color)));
            glVertexAttribDivisor(4, 1);

            // model:
            glEnableVertexAttribArray(5);
            glEnableVertexAttribArray(6);
            glEnableVertexAttribArray(7);
            glEnableVertexAttribArray(8);

            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof (RObject), (void*)(offsetof(RObject, model) + 0 * sizeof (float[4])));
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof (RObject), (void*)(offsetof(RObject, model) + 1 * sizeof (float[4])));
            glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof (RObject), (void*)(offsetof(RObject, model) + 2 * sizeof (float[4])));
            glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof (RObject), (void*)(offsetof(RObject, model) + 3 * sizeof (float[4])));

            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);
            glVertexAttribDivisor(7, 1);
            glVertexAttribDivisor(8, 1);
        }
    }

    // init shadow map stuff
    {
        ce_shader_depth = ShaderLoadFromMemory(ce_depth_vertex, ce_depth_fragment, NULL);

        glGenFramebuffers(1, &ce_depth_fbo);
        // create depth texture
        glGenTextures(1, &ce_depth_map);
        glBindTexture(GL_TEXTURE_2D, ce_depth_map);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, ce_depth_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ce_depth_map, 0);

        //glReadBuffer(GL_NONE);

    //}

    // init shadow map stuff
    //{
        //ce_shader_color = shader_load_from_memory(ce_color_vertex, ce_color_fragment);

        //glGenFramebuffers(1, &ce_color_fbo);
        // create color texture
        glGenTextures(1, &ce_color_map);
        glBindTexture(GL_TEXTURE_2D, ce_color_map);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        // attach color texture as FBO's color buffer
        //
        //glBindFramebuffer(GL_FRAMEBUFFER, ce_color_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ce_color_map, 0);
        //glDrawBuffer(GL_FRONT);
        //glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        ShaderUse(ce_shader);
        ShaderSeti(ce_shader, "shadow_map", 0);
        ShaderSeti(ce_shader, "color_map",  1);
    }
}

// ===================================================== CE BITMAP ================================================== //

#define BITMAP_COUNT    (256)

static const unsigned long long bitascii[BITMAP_COUNT] = {
    0x0000000000000000,
    0x7e8199bd81a5817e,
    0x7effe7c3ffdbff7e,
    0x00081c3e7f7f7f36,
    0x00081c3e7f3e1c08,
    0x1c086b7f7f1c3e1c,
    0x1c082a7f7f3e1c08,
    0x0000183c3c180000,
    0xffffe7c3c3e7ffff,
    0x003c664242663c00,
    0xffc399bdbd99c3ff,
    0x1e333333bef0e0f0,
    0x187e183c6666663c,
    0x070f0e0c0c7c6c7c,
    0x03377666667e667e,
    0x0018dbff3cffdb18,
    0x00061e7e7e1e0600,
    0x0060787e7e786000,
    0x183c7e18187e3c18,
    0x0036003636363636,
    0x00d8d8d8dedbdbfe,
    0x3e613c66663c867c,
    0x007e7e0000000000,
    0xff183c7e187e3c18,
    0x001818187e3c1800,
    0x00183c7e18181800,
    0x0010307e7e301000,
    0x00080c7e7e0c0800,
    0x00007e0606000000,
    0x002466ffff662400,
    0x007e7e3c3c181800,
    0x0018183c3c7e7e00,
    0x0000000000000000,
    0x00180018183c3c18,
    0x0000000000143636,
    0x0036367f367f3636,
    0x00183e603c067c18,
    0x0063660c18336300,
    0x006e333b6e1c361c,
    0x00000000000c1818,
    0x00180c0606060c18,
    0x00060c1818180c06,
    0x0000361c7f1c3600,
    0x000018187e181800,
    0x0c18180000000000,
    0x000000007e000000,
    0x0018180000000000,
    0x0003070e1c387060,
    0x003c666e7e76663c,
    0x007e181818181e18,
    0x007e660c3860663c,
    0x003c66603860663c,
    0x0030307f33363c38,
    0x003c6660603e067e,
    0x003c66663e060c38,
    0x000c0c183060667e,
    0x003c66663c66663c,
    0x001c30607c66663c,
    0x0018180018180000,
    0x0c18180018180000,
    0x0030180c060c1830,
    0x0000007e007e0000,
    0x00060c1830180c06,
    0x001800183060663c,
    0x003c06767676663c,
    0x0066667e66663c18,
    0x003f66663e66663f,
    0x003c66030303663c,
    0x003f36666666363f,
    0x007f46161e16467f,
    0x000f06161e16467f,
    0x007c66730303663c,
    0x006666667e666666,
    0x003c18181818183c,
    0x001e333330303078,
    0x006766361e366667,
    0x007f66460606060f,
    0x006363636b7f7763,
    0x006363737b6f6763,
    0x001c36636363361c,
    0x000f06063e66663f,
    0x00703c766666663c,
    0x0067361e3e66663f,
    0x003c6670380e663c,
    0x003c181818185a7e,
    0x007e666666666666,
    0x00183c6666666666,
    0x0063777f6b636363,
    0x006363361c366363,
    0x003c18183c666666,
    0x007f63460c19337f,
    0x003c0c0c0c0c0c3c,
    0x006070381c0e0703,
    0x003c30303030303c,
    0x0000000063361c08,
    0x7e00000000000000,
    0x0000000000301818,
    0x006e333e301e0000,
    0x003d6666663e0607,
    0x003c6606663c0000,
    0x006e33333e303038,
    0x003c067e663c0000,
    0x001e0c0c1e0c6c38,
    0x1f303e33336e0000,
    0x006766666e360607,
    0x003c1818181c0018,
    0x1c363030303c0030,
    0x0067361e36660607,
    0x003c18181818181c,
    0x0063636b7f370000,
    0x00666666663e0000,
    0x001e3333331e0000,
    0x0f063e66663b0000,
    0x78303e33336e0000,
    0x001e0c6c6c360000,
    0x003e603c067c0000,
    0x00182c0c0c3e0c08,
    0x006e333333330000,
    0x00183c6666660000,
    0x00367f6b63630000,
    0x0063361c36630000,
    0x3e607c6666660000,
    0x007e4c18327e0000,
    0x00380c0c060c0c38,
    0x0018181800181818,
    0x000e18183018180e,
    0x0000000000003b6e,
    0x007e66663c180000,
    0x060c1e330303331e,
    0x007e333333003300,
    0x003c067e663c1830,
    0x00fc667c603cc37e,
    0x007e333e301e0033,
    0x007e333e301e0c06,
    0x00fc667c603c663c,
    0x0c183c6606663c00,
    0x003c067e663cc37e,
    0x003c067e663c0066,
    0x003c067e663c180c,
    0x003c1818181c0066,
    0x003c1818181c633e,
    0x003c1818181c180c,
    0x00667e66663c1866,
    0x00667e663c182418,
    0x003f061e063f0c18,
    0x007e337e307e0000,
    0x007333337f33367c,
    0x003c66663c00663c,
    0x003c66663c006600,
    0x003c66663c00180c,
    0x007e33333300331e,
    0x007e333333000c06,
    0x3e607e6666006600,
    0x003e6363633e0063,
    0x003c666666660066,
    0x18187e03037e1818,
    0x003f67060f26361c,
    0x18187e187e3c6666,
    0x00721a321e22221e,
    0x000e1b187e18d870,
    0x007e333e301e0c18,
    0x003c1818181c1830,
    0x003c66663c003060,
    0x007e333333001830,
    0x003333331f003b6e,
    0x00333b3f37003b6e,
    0x00007e007c36363c,
    0x00007e003c66663c,
    0x003c66060c180018,
    0x000006067e000000,
    0x000060607e000000,
    0xf03366cc7b3363c3,
    0xc0f3f6ecdb3363c3,
    0x183c3c1818001800,
    0x0000cc663366cc00,
    0x00003366cc663300,
    0x8822882288228822,
    0x55aa55aa55aa55aa,
    0xeebbeebbeebbeebb,
    0x1818181818181818,
    0x1818181f1f181818,
    0x181f1f18181f1f18,
    0x6666666767666666,
    0x6666667f7f000000,
    0x181f1f18181f1f00,
    0x6667676060676766,
    0x6666666666666666,
    0x66676760607f7f00,
    0x007f7f6060676766,
    0x0000007f7f666666,
    0x001f1f18181f1f18,
    0x1818181f1f000000,
    0x000000f8f8181818,
    0x000000ffff181818,
    0x181818ffff000000,
    0x181818f8f8181818,
    0x000000ffff000000,
    0x181818ffff181818,
    0x18f8f81818f8f818,
    0x666666e6e6666666,
    0x00fefe0606e6e666,
    0x66e6e60606fefe00,
    0x00ffff0000e7e766,
    0x66e7e70000ffff00,
    0x66e6e60606e6e666,
    0x00ffff0000ffff00,
    0x66e7e70000e7e766,
    0x00ffff0000ffff18,
    0x000000ffff666666,
    0x18ffff0000ffff00,
    0x666666ffff000000,
    0x000000fefe666666,
    0x00f8f81818f8f818,
    0x18f8f81818f8f800,
    0x666666fefe000000,
    0x666666ffff666666,
    0x18ffff1818ffff18,
    0x0000001f1f181818,
    0x181818f8f8000000,
    0xffffffffffffffff,
    0xffffffff00000000,
    0x0f0f0f0f0f0f0f0f,
    0xf0f0f0f0f0f0f0f0,
    0x00000000ffffffff,
    0x00006e3b3b6e0000,
    0x000c3e663e663c00,
    0x0000060606667e00,
    0x0066363636367f00,
    0x007e460c0c467e00,
    0x001c3636367c0000,
    0x03063e6666660000,
    0x001818183b6e0000,
    0x007e183c663c187e,
    0x001c36363e36361c,
    0x007736366363361c,
    0x003c66663c180c38,
    0x00003e6b6b3e0000,
    0x03063e6b6b3e1830,
    0x00380c063e060c38,
    0x0066666666663c00,
    0x00007e007e007e00,
    0x003f000c0c3f0c0c,
    0x003f00060c180c06,
    0x003f00180c060c18,
    0x0606060636361c00,
    0x001c363630303030,
    0x001818007e001818,
    0x00003b6e003b6e00,
    0x000000001c36361c,
    0x0000001818000000,
    0x0000001800000000,
    0x00181c1612101070,
    0x000000003636361e,
    0x000000003c18301c,
    0x007e7e7e7e7e7e00,
    0x007e424242427e00
};

void ceBitmapRenderRect(float x, float y, float z, float sx, float sy, float r, float g, float b, float a) {
    cePushBox(
            x, y, z, 
            0.5f * sx, 0.5f * sy, 0.01,
            r, g, b, a);
}

// @TODO: make less shit!!
void cePushAscii(unsigned char c, float px, float py, float pz, float x_scale, float y_scale, float r, float g, float b, float a) {
    unsigned long long n = bitascii[c];

    for (int j = 0; j < 8; ++j) {
        for (int i = 0; i < 8; ++i) {
            int bit = j * 8 + i;

            if (CHKBIT(n, bit)) {
                ceBitmapRenderRect(px + i * x_scale, py - j * y_scale, pz, x_scale, -y_scale, r, g, b, a);
            }
        }
    }
}

void cePushString(const char* str, float x, float y, float z, float scale_x, float scale_y, float r, float g, float b, float a) {
    for (int i = 0; str[i] != '\0'; i++)
        cePushAscii(str[i], x + i * 8 * scale_x, y, z, scale_x, scale_y, r, g, b, a);
}

void cePushStringFormat(float x, float y, float z, float rad, float r, float g, float b, float a, const char* fmt, ...) {
    va_list list;
    char    buffer[64];

    va_start(list, fmt);
    vsnprintf(buffer, 64, fmt, list);
    cePushString(buffer, x, y, z, rad, rad, r, g, b, a);
    va_end(list);
}


void cePushInt(int n, float x, float y, float z, float scale_x, float scale_y, float r, float g, float b, float a) {
    char buffer[64];

    sprintf(buffer, "%d", n);
    cePushString(buffer, x, y, z, scale_x, scale_y, r, g, b, a);
}

void cePushF32(float n, float x, float y, float z, float scale_x, float scale_y, float r, float g, float b, float a) {
    char buffer[64];

    sprintf(buffer, "%.1f", n);
    cePushString(buffer, x, y, z, scale_x, scale_y, r, g, b, a);
}

void cePushStringBox(const char* str, float x, float y, float z, float w, float h, float r, float g, float b, float a) {
    int slen    = strlen(str);
    float x_scale = w / ((float)slen * 8.0f);
    float y_scale = h / 8.0f;

    for (int i = 0; i < slen; i++) {
        cePushAscii(str[i], x + i * 8 * x_scale, y, z, x_scale, y_scale, r, g, b, a);
    }
}

void cePushNumberBox(int n, float x, float y, float z, float w, float h, float r, float g, float b, float a) {
    char buffer[32];

    sprintf(buffer, "%d", n);
    cePushStringBox(buffer, x, y, z, w, h, r, g, b, a);
}

