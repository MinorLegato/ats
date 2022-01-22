#pragma once

#define SR_MAX_POINT_LIGHTS 16

typedef struct {
    i16     x;
    i16     y;
} sr_uv;

typedef struct {
    v3          pos;
    v3          normal;
    sr_uv       uv;
    u32         color;
} sr_vertex_t;

typedef struct {
    gl_shader   shader;
    u32         type;

    u32         index;
    u32         count;
} sr_range_t;

typedef struct {
    v3      pos;

    v3      ambient;
    v3      diffuse;
    v3      specular;

    f32     constant;
    f32     linear;
    f32     quadratic;

    f32     range;
} sr_point_light_t;

typedef struct {
    u32     range;

    u32     pos;

    u32     ambient;
    u32     diffuse;
    u32     specular;

    u32     constant;
    u32     linear;
    u32     quadratic;
} sr_point_light_uniform_t;

static gl_shader        sr_shader;
static gl_shader        sr_basic_shader;
static gl_shader        sr_texture_shader;

static gl_shader        sr_ui_basic_shader;
static gl_shader        sr_ui_texture_shader;
static gl_shader        sr_ui_text_shader;

static gl_texture       sr_texture;

static u32              sr_framebuffer;
static gl_shader        sr_frame_shader;
static gl_array         sr_frame_array;

static u32              sr_frame_color;
static u32              sr_frame_position;
static u32              sr_frame_normal;

static gl_array         sr_array;
static sr_vertex_t      sr_current_vertex;

static u32              sr_vertex_count;
static sr_vertex_t      sr_vertex_array[1024 * 1024];

static b32              sr_depth_test = false;
static sr_range_t       sr_current_range;

static u32              sr_range_count;
static sr_range_t       sr_range_array[1024 * 1024];

static sr_point_light_uniform_t sr_lights[SR_MAX_POINT_LIGHTS];

static gl_texture sr_bitmap_texture;

static gl_shader_desc sr_shader_desc = {
    // vertex shader:
    GLSL_SHADER(
        layout (location = 0) in vec3 in_position;
        layout (location = 1) in vec3 in_normal;
        layout (location = 2) in vec2 in_uv;
        layout (location = 3) in vec4 in_color;

        out vec3 frag_pos;
        out vec3 frag_normal;
        out vec2 frag_uv;
        out vec4 frag_color;

        uniform mat4 pvm;

        void main() {
            frag_pos    = in_position;
            frag_normal = in_normal;
            frag_uv     = in_uv;
            frag_color  = in_color;

            gl_Position = pvm * vec4(in_position, 1);
        }),

        // fragment shader:
        GLSL_SHADER(
            layout (location = 0) out vec4 out_color;
            layout (location = 1) out vec3 out_position;
            layout (location = 2) out vec3 out_normal;

            in vec3 frag_pos;
            in vec3 frag_normal;
            in vec2 frag_uv;
            in vec4 frag_color;

            uniform sampler2D texture1;

            void main() {
                vec4 color = frag_color * texture2D(texture1, frag_uv / textureSize(texture1, 0));
                if (color.a == 0) discard;
                
                out_color       = color;
                out_position    = frag_pos;
                out_normal      = frag_normal;
            }),
};

static gl_shader_desc sr_texture_shader_desc = {
    // vertex shader:
    GLSL_SHADER(
        layout (location = 0) in vec3 in_position;
        layout (location = 1) in vec3 in_normal;
        layout (location = 2) in vec2 in_uv;
        layout (location = 3) in vec4 in_color;

        in vec3 frag_pos;
        in vec3 frag_normal;
        out vec2 frag_uv;
        out vec4 frag_color;

        uniform mat4 pvm;

        void main() {
            frag_uv     = in_uv;
            frag_color  = in_color;

            gl_Position = pvm * vec4(in_position, 1);
        }),

    // fragment shader:
    GLSL_SHADER(
        layout (location = 0) out vec4 out_color;
        layout (location = 1) out vec4 out_position;
        layout (location = 2) out vec4 out_normal;

        in vec2 frag_uv;
        in vec4 frag_color;

        uniform sampler2D texture1;

        void main() {
            vec4 color = frag_color * texture2D(texture1, frag_uv / textureSize(texture1, 0));
            if (color.a == 0) discard;

            out_color       = color;
            out_position    = vec4(0);
            out_normal      = vec4(0);
        }),
};

static gl_shader_desc sr_text_shader_desc = {
    // vertex shader:
    GLSL_SHADER(
        layout (location = 0) in vec3 in_position;
        layout (location = 1) in vec3 in_normal;
        layout (location = 2) in vec2 in_uv;
        layout (location = 3) in vec4 in_color;

        out vec2 frag_uv;
        out vec4 frag_color;

        uniform mat4 pvm;

        void main() {
            frag_uv     = in_uv;
            frag_color  = in_color;

            gl_Position = pvm * vec4(in_position, 1);
        }),

    // fragment shader:
    GLSL_SHADER(
        layout (location = 0) out vec4 out_color;
        layout (location = 1) out vec4 out_position;
        layout (location = 2) out vec4 out_normal;

        in vec2 frag_uv;
        in vec4 frag_color;

        uniform sampler2D texture1;

        void main() {
            vec4 color = frag_color * texture2D(texture1, frag_uv / textureSize(texture1, 0));
            if (color.a == 0) discard;

            out_color       = color;
            out_position    = vec4(0);
            out_normal      = vec4(0);
        }),
};

static gl_shader_desc sr_basic_shader_desc = {
    // vertex shader:
    GLSL_SHADER(
        layout (location = 0) in vec3 in_position;
        layout (location = 1) in vec3 in_normal;
        layout (location = 2) in vec2 in_uv;
        layout (location = 3) in vec4 in_color;

        out vec4 frag_color;

        uniform mat4 pvm;

        void main() {
            frag_color  = in_color;
            gl_Position = pvm * vec4(in_position, 1);
        }),

    // fragment shader:
    GLSL_SHADER(
        layout (location = 0) out vec4 out_color;
        layout (location = 1) out vec4 out_position;
        layout (location = 2) out vec4 out_normal;

        in vec4 frag_color;

        void main() {
            out_color       = frag_color;
            out_position    = vec4(0);
            out_normal      = vec4(0);
        }),
}; 

static gl_shader_desc sr_frame_shader_desc = {
    .vs = GLSL_SHADER(
        layout (location = 0) in vec2 in_pos;
        layout (location = 1) in vec2 in_uv;

        out vec2 frag_uv;

        void main() {
            frag_uv         = in_uv;
            gl_Position     = vec4(in_pos, 0, 1);
        }),

    .fs = GLSL_SHADER(
        out vec4    out_color;
        in  vec2    frag_uv;

        struct Point_Light {
            float   range;

            vec3    pos;

            vec3    ambient;
            vec3    diffuse;
            vec3    specular;

            float   constant;
            float   linear;
            float   quadratic;
        };

        uniform vec3        view_pos;
        uniform Point_Light light[16];

        vec3 calculate_point_light(int i, vec3 color, vec3 frag_pos, vec3 frag_normal) {
            // ambient
            vec3    ambient     = light[i].ambient * color;

            // diffuse 
            vec3    norm        = normalize(frag_normal);
            vec3    light_dir   = normalize(light[i].pos - frag_pos);
            float   diff        = max(dot(norm, light_dir), 0.0);
            vec3    diffuse     = light[i].diffuse * diff * color;

            // specular
            vec3    view_dir    = normalize(view_pos - frag_pos);
            vec3    reflect_dir = reflect(-light_dir, norm);  
            float   spec        = pow(max(dot(view_dir, reflect_dir), 0.0), 1);
            vec3    specular    = light[i].specular * spec * color;  

            // attenuation
            float   distance    = length(light[i].pos - frag_pos);
            float   attenuation = 1.0 / (light[i].constant + light[i].linear * distance + light[i].quadratic * (distance * distance));    

            ambient  *= attenuation;
            diffuse  *= attenuation;
            specular *= attenuation;

            //float str = distance / light[i].range;
            return (ambient + diffuse + specular); // * (1 - str * str * str);
        }
 
        uniform sampler2D in_color;
        uniform sampler2D in_position;
        uniform sampler2D in_normal;
       
        void main() {
            vec4 color      = texture(in_color, frag_uv);
            vec3 position   = texture(in_position, frag_uv).rgb;
            vec3 normal     = texture(in_normal, frag_uv).rgb;

            vec3 result = vec3(0);
            for (int i = 0; i < 16; ++i) {
                if (light[i].range > 0) {
                    result += calculate_point_light(i, color.rgb, position, normal);
                }
            }

            out_color = vec4(result, 1);
        }),
};

static void sr_init_bitmap(void);

static void sr_init(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
    glClearDepth(1.0f);

    glDepthFunc(GL_LESS);

    glEnable(GL_DEPTH_TEST);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // game shader: (used in world space)
    {
        sr_basic_shader     = gl_shader_create(&sr_basic_shader_desc);
        sr_texture_shader   = gl_shader_create(&sr_texture_shader_desc);
        sr_shader           = gl_shader_create(&sr_shader_desc);
        sr_frame_shader     = gl_shader_create(&sr_frame_shader_desc);

        {
            gl_shader_use(sr_frame_shader);
            gl_uniform_i32(gl_shader_location(sr_frame_shader, "in_color"),     0);
            gl_uniform_i32(gl_shader_location(sr_frame_shader, "in_position"),  1);
            gl_uniform_i32(gl_shader_location(sr_frame_shader, "in_normal"),    2);

            char buffer[256];
            for (u32 i = 0; i < SR_MAX_POINT_LIGHTS; ++i) {
                sr_point_light_uniform_t* light = &sr_lights[i];

                sprintf(buffer, "light[%d].range", i);     light->range     = gl_shader_location(sr_frame_shader, buffer);
                sprintf(buffer, "light[%d].pos", i);       light->pos       = gl_shader_location(sr_frame_shader, buffer);
                sprintf(buffer, "light[%d].ambient", i);   light->ambient   = gl_shader_location(sr_frame_shader, buffer);
                sprintf(buffer, "light[%d].diffuse", i);   light->diffuse   = gl_shader_location(sr_frame_shader, buffer);
                sprintf(buffer, "light[%d].specular", i);  light->specular  = gl_shader_location(sr_frame_shader, buffer);
                sprintf(buffer, "light[%d].constant", i);  light->constant  = gl_shader_location(sr_frame_shader, buffer);
                sprintf(buffer, "light[%d].linear", i);    light->linear    = gl_shader_location(sr_frame_shader, buffer);
                sprintf(buffer, "light[%d].quadratic", i); light->quadratic = gl_shader_location(sr_frame_shader, buffer);
            }
        }
    }

    // ui shader: (used in screen space)
    {
        sr_ui_basic_shader      = gl_shader_create(&sr_basic_shader_desc);
        sr_ui_texture_shader    = gl_shader_create(&sr_texture_shader_desc);
        sr_ui_text_shader       = gl_shader_create(&sr_text_shader_desc);
    }

    gl_array_desc array_desc = ATS_INIT_ZERO;
    
    array_desc.layout[0] = ctor(gl_layout, 3, GL_FLOAT,           sizeof (sr_vertex_t), offsetof(sr_vertex_t, pos));
    array_desc.layout[1] = ctor(gl_layout, 3, GL_FLOAT,           sizeof (sr_vertex_t), offsetof(sr_vertex_t, normal), true);
    array_desc.layout[2] = ctor(gl_layout, 2, GL_SHORT,           sizeof (sr_vertex_t), offsetof(sr_vertex_t, uv));
    array_desc.layout[3] = ctor(gl_layout, 4, GL_UNSIGNED_BYTE,   sizeof (sr_vertex_t), offsetof(sr_vertex_t, color), true);

    sr_array = gl_array_create(&array_desc);
    gl_array_data(sr_array, NULL, sizeof (sr_vertex_t) * array_count(sr_vertex_array));

    sr_init_bitmap();

    // setup frame-buffer:
    {
        glGenFramebuffers(1, &sr_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, sr_framebuffer);

        glGenTextures(1, &sr_frame_color);
        glBindTexture(GL_TEXTURE_2D, sr_frame_color);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sr_frame_color, 0);

        glGenTextures(1, &sr_frame_position);
        glBindTexture(GL_TEXTURE_2D, sr_frame_position);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 2048, 2048, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, sr_frame_position, 0);

        glGenTextures(1, &sr_frame_normal);
        glBindTexture(GL_TEXTURE_2D, sr_frame_normal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 2048, 2048, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, sr_frame_normal, 0);

        u32 attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(3, attachments);

        u32 rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 2048, 2048);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        sr_frame_array = gl_array_create(&(gl_array_desc) {
            .layout = {
                [0] = { .size = 2, .type = GL_FLOAT, .stride = 4 * sizeof (f32) },
                [1] = { .size = 2, .type = GL_FLOAT, .stride = 4 * sizeof (f32), .offset = 2 * sizeof (f32) },
            },
        });

        f32 vertex_array[] = {
            -1, -1,     0, 0,
            +1, -1,     1, 0,
            +1, +1,     1, 1,
            +1, +1,     1, 1,
            -1, +1,     0, 1,
            -1, -1,     0, 0,
        };

        gl_array_data(sr_frame_array, vertex_array, sizeof (vertex_array));
    }
}

static void sr_color(u32 color) {
    sr_current_vertex.color = color;
}

static void sr_normal(f32 x, f32 y, f32 z) {
    sr_current_vertex.normal = v3(x, y, z);
}

static void sr_tex_coord(i32 x, i32 y) {
    sr_current_vertex.uv.x = x;
    sr_current_vertex.uv.y = y;
}

static void sr_vertex(f32 x, f32 y, f32 z) {
    sr_current_vertex.pos = v3(x, y, z);
    sr_vertex_array[sr_vertex_count++] = sr_current_vertex;
}

static void sr_set_light(u32 light_index, const sr_point_light_t* light) {
    const sr_point_light_uniform_t* location = &sr_lights[light_index];

    gl_shader_use(sr_frame_shader);

    gl_uniform_f32(location->range,     light->range);

    gl_uniform_v3(location->pos,        light->pos);
    gl_uniform_v3(location->ambient,    light->ambient);
    gl_uniform_v3(location->diffuse,    light->diffuse);
    gl_uniform_v3(location->specular,   light->specular);

    gl_uniform_f32(location->constant,  light->constant);
    gl_uniform_f32(location->linear,    light->linear);
    gl_uniform_f32(location->quadratic, light->quadratic);
}

static void sr_disable_all_lights(void) {
    gl_shader_use(sr_frame_shader);

    for (u32 i = 0; i < SR_MAX_POINT_LIGHTS; ++i) {
        const sr_point_light_uniform_t* location = &sr_lights[i];

        gl_uniform_f32(location->range,  0);
    }
}

static void sr_set_texture(gl_texture texture) {
    sr_texture = texture;
}

static void sr_begin(u32 primitive_type, gl_shader shader) {
    memset(&sr_current_range, 0, sizeof sr_current_range);

    sr_current_range.shader = shader;
    sr_current_range.type   = primitive_type;
    sr_current_range.index  = sr_vertex_count;
}

static void sr_end(void) {
    sr_current_range.count = sr_vertex_count - sr_current_range.index;
    sr_range_array[sr_range_count++] = sr_current_range;
}

static void sr_begin_frame(v3 view_pos, m4 pvm) {
    m4 ortho = m4_ortho(0, platform.width, platform.height, 0, -1, 1);

    {
        gl_shader_use(sr_ui_basic_shader);
        gl_uniform_m4(gl_shader_location(sr_ui_basic_shader, "pvm"), ortho);

        gl_shader_use(sr_ui_texture_shader);
        gl_uniform_m4(gl_shader_location(sr_ui_texture_shader, "pvm"), ortho);

        gl_shader_use(sr_ui_text_shader);
        gl_uniform_m4(gl_shader_location(sr_ui_text_shader, "pvm"), ortho);
    }

    {
        gl_shader_use(sr_basic_shader);
        gl_uniform_m4(gl_shader_location(sr_basic_shader, "pvm"), pvm);

        gl_shader_use(sr_texture_shader);
        gl_uniform_m4(gl_shader_location(sr_texture_shader, "pvm"), pvm);

        gl_shader_use(sr_shader);
        gl_uniform_m4(gl_shader_location(sr_shader, "pvm"), pvm);

        gl_shader_use(sr_frame_shader);
        gl_uniform_v3(gl_shader_location(sr_frame_shader, "view_pos"), view_pos);
    }
}

static void sr_end_frame(void) {
    // render to framebuffer
    {
        glBindFramebuffer(GL_FRAMEBUFFER, sr_framebuffer);

        glBindTexture(GL_TEXTURE_2D, sr_frame_color);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, platform.width, platform.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glBindTexture(GL_TEXTURE_2D, sr_frame_position);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, platform.width, platform.height, 0, GL_RGBA, GL_FLOAT, NULL);

        glBindTexture(GL_TEXTURE_2D, sr_frame_normal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, platform.width, platform.height, 0, GL_RGBA, GL_FLOAT, NULL);
        
        glActiveTexture(GL_TEXTURE0);
        gl_texture_bind(&sr_texture);

        glActiveTexture(GL_TEXTURE1);
        gl_texture_bind(&sr_bitmap_texture);

        glBindVertexArray(sr_array.vao);
        gl_array_data(sr_array, sr_vertex_array, sizeof (sr_vertex_t) * sr_vertex_count);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        for (u32 i = 0; i < sr_range_count; ++i) {
            const sr_range_t* range = &sr_range_array[i];

            gl_shader_use(range->shader);
            glDrawArrays(range->type, range->index, range->count);
        }
    }
        
    // render the frambuffer:
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sr_frame_color);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, sr_frame_position);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, sr_frame_normal);

        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gl_shader_use(sr_frame_shader);

        glBindVertexArray(sr_frame_array.vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    sr_vertex_count = 0;
    sr_range_count = 0;
    sr_disable_all_lights();
}

static void sr_billboard(r2i tex_rect, v3 pos, v2 rad, v3 normal, u32 color, v3 right, v3 up) {
    f32 ax = pos.x - right.x * rad.x - up.x * rad.y;
    f32 ay = pos.y - right.y * rad.x - up.y * rad.y;
    f32 az = pos.z - right.z * rad.x - up.z * rad.y;

    f32 bx = pos.x + right.x * rad.x - up.x * rad.y;
    f32 by = pos.y + right.y * rad.x - up.y * rad.y;
    f32 bz = pos.z + right.z * rad.x - up.z * rad.y;

    f32 cx = pos.x + right.x * rad.x + up.x * rad.y;
    f32 cy = pos.y + right.y * rad.x + up.y * rad.y;
    f32 cz = pos.z + right.z * rad.x + up.z * rad.y;

    f32 dx = pos.x - right.x * rad.x + up.x * rad.y;
    f32 dy = pos.y - right.y * rad.x + up.y * rad.y;
    f32 dz = pos.z - right.z * rad.x + up.z * rad.y;

    sr_color(color);
    sr_normal(normal.x, normal.y, normal.z);

    sr_tex_coord(tex_rect.min.x, tex_rect.max.y); sr_vertex(ax, ay, az);
    sr_tex_coord(tex_rect.max.x, tex_rect.max.y); sr_vertex(bx, by, bz);
    sr_tex_coord(tex_rect.max.x, tex_rect.min.y); sr_vertex(cx, cy, cz);

    sr_tex_coord(tex_rect.max.x, tex_rect.min.y); sr_vertex(cx, cy, cz);
    sr_tex_coord(tex_rect.min.x, tex_rect.min.y); sr_vertex(dx, dy, dz);
    sr_tex_coord(tex_rect.min.x, tex_rect.max.y); sr_vertex(ax, ay, az);
}

static void sr_texture_box(r2i rect, r3 box, u32 color) {
    sr_color(color);
    
    sr_normal(0, 0, -1);
    sr_tex_coord(rect.min.x, rect.max.y); sr_vertex(box.min.x, box.min.y, box.min.z);
    sr_tex_coord(rect.max.x, rect.min.y); sr_vertex(box.max.x, box.max.y, box.min.z);
    sr_tex_coord(rect.max.x, rect.max.y); sr_vertex(box.max.x, box.min.y, box.min.z);
    sr_tex_coord(rect.max.x, rect.min.y); sr_vertex(box.max.x, box.max.y, box.min.z);
    sr_tex_coord(rect.min.x, rect.max.y); sr_vertex(box.min.x, box.min.y, box.min.z);
    sr_tex_coord(rect.min.x, rect.min.y); sr_vertex(box.min.x, box.max.y, box.min.z);
    
    sr_normal(0, 0, +1);
    sr_tex_coord(rect.min.x, rect.max.y); sr_vertex(box.min.x, box.min.y, box.max.z);
    sr_tex_coord(rect.max.x, rect.max.y); sr_vertex(box.max.x, box.min.y, box.max.z);
    sr_tex_coord(rect.max.x, rect.min.y); sr_vertex(box.max.x, box.max.y, box.max.z);
    sr_tex_coord(rect.max.x, rect.min.y); sr_vertex(box.max.x, box.max.y, box.max.z);
    sr_tex_coord(rect.min.x, rect.min.y); sr_vertex(box.min.x, box.max.y, box.max.z);
    sr_tex_coord(rect.min.x, rect.max.y); sr_vertex(box.min.x, box.min.y, box.max.z);
    
    sr_normal(-1, 0, 0);
    sr_tex_coord(rect.max.x, rect.min.y); sr_vertex(box.min.x, box.max.y, box.max.z);
    sr_tex_coord(rect.max.x, rect.max.y); sr_vertex(box.min.x, box.max.y, box.min.z);
    sr_tex_coord(rect.min.x, rect.max.y); sr_vertex(box.min.x, box.min.y, box.min.z);
    sr_tex_coord(rect.min.x, rect.max.y); sr_vertex(box.min.x, box.min.y, box.min.z);
    sr_tex_coord(rect.min.x, rect.min.y); sr_vertex(box.min.x, box.min.y, box.max.z);
    sr_tex_coord(rect.max.x, rect.min.y); sr_vertex(box.min.x, box.max.y, box.max.z);
    
    sr_normal(+1, 0, 0);
    sr_tex_coord(rect.min.x, rect.max.y); sr_vertex(box.max.x, box.min.y, box.min.z);
    sr_tex_coord(rect.max.x, rect.min.y); sr_vertex(box.max.x, box.max.y, box.max.z);
    sr_tex_coord(rect.min.x, rect.min.y); sr_vertex(box.max.x, box.min.y, box.max.z);
    sr_tex_coord(rect.max.x, rect.min.y); sr_vertex(box.max.x, box.max.y, box.max.z);
    sr_tex_coord(rect.min.x, rect.max.y); sr_vertex(box.max.x, box.min.y, box.min.z);
    sr_tex_coord(rect.max.x, rect.max.y); sr_vertex(box.max.x, box.max.y, box.min.z);

    sr_normal(0, -1, 0);
    sr_tex_coord(rect.min.x, rect.max.y); sr_vertex(box.min.x, box.min.y, box.min.z);
    sr_tex_coord(rect.max.x, rect.max.y); sr_vertex(box.max.x, box.min.y, box.min.z);
    sr_tex_coord(rect.max.x, rect.min.y); sr_vertex(box.max.x, box.min.y, box.max.z);
    sr_tex_coord(rect.max.x, rect.min.y); sr_vertex(box.max.x, box.min.y, box.max.z);
    sr_tex_coord(rect.min.x, rect.min.y); sr_vertex(box.min.x, box.min.y, box.max.z);
    sr_tex_coord(rect.min.x, rect.max.y); sr_vertex(box.min.x, box.min.y, box.min.z);
    
    sr_normal(0, +1, 0);
    sr_tex_coord(rect.min.x, rect.max.y); sr_vertex(box.min.x, box.max.y, box.min.z);
    sr_tex_coord(rect.max.x, rect.min.y); sr_vertex(box.max.x, box.max.y, box.max.z);
    sr_tex_coord(rect.max.x, rect.max.y); sr_vertex(box.max.x, box.max.y, box.min.z);
    sr_tex_coord(rect.max.x, rect.min.y); sr_vertex(box.max.x, box.max.y, box.max.z);
    sr_tex_coord(rect.min.x, rect.max.y); sr_vertex(box.min.x, box.max.y, box.min.z);
    sr_tex_coord(rect.min.x, rect.min.y); sr_vertex(box.min.x, box.max.y, box.max.z);
}

static void sr_texture_rect(r2i tex_rect, r2 rect, f32 z, u32 color) {
    sr_color(color);
    
    sr_normal(0, 0, +1);
    sr_tex_coord(tex_rect.min.x, tex_rect.max.y); sr_vertex(rect.min.x, rect.min.y, z);
    sr_tex_coord(tex_rect.max.x, tex_rect.max.y); sr_vertex(rect.max.x, rect.min.y, z);
    sr_tex_coord(tex_rect.max.x, tex_rect.min.y); sr_vertex(rect.max.x, rect.max.y, z);
    sr_tex_coord(tex_rect.max.x, tex_rect.min.y); sr_vertex(rect.max.x, rect.max.y, z);
    sr_tex_coord(tex_rect.min.x, tex_rect.min.y); sr_vertex(rect.min.x, rect.max.y, z);
    sr_tex_coord(tex_rect.min.x, tex_rect.max.y); sr_vertex(rect.min.x, rect.min.y, z);
}

static void sr_box(r3 box, u32 color) {
    sr_color(color);
    
    sr_normal(0, 0, -1);
    sr_vertex(box.min.x, box.min.y, box.min.z);
    sr_vertex(box.max.x, box.max.y, box.min.z);
    sr_vertex(box.max.x, box.min.y, box.min.z);
    sr_vertex(box.max.x, box.max.y, box.min.z);
    sr_vertex(box.min.x, box.min.y, box.min.z);
    sr_vertex(box.min.x, box.max.y, box.min.z);
    
    sr_normal(0, 0, +1);
    sr_vertex(box.min.x, box.min.y, box.max.z);
    sr_vertex(box.max.x, box.min.y, box.max.z);
    sr_vertex(box.max.x, box.max.y, box.max.z);
    sr_vertex(box.max.x, box.max.y, box.max.z);
    sr_vertex(box.min.x, box.max.y, box.max.z);
    sr_vertex(box.min.x, box.min.y, box.max.z);
    
    sr_normal(-1, 0, 0);
    sr_vertex(box.min.x, box.max.y, box.max.z);
    sr_vertex(box.min.x, box.max.y, box.min.z);
    sr_vertex(box.min.x, box.min.y, box.min.z);
    sr_vertex(box.min.x, box.min.y, box.min.z);
    sr_vertex(box.min.x, box.min.y, box.max.z);
    sr_vertex(box.min.x, box.max.y, box.max.z);
    
    sr_normal(+1, 0, 0);
    sr_vertex(box.max.x, box.min.y, box.min.z);
    sr_vertex(box.max.x, box.max.y, box.max.z);
    sr_vertex(box.max.x, box.min.y, box.max.z);
    sr_vertex(box.max.x, box.max.y, box.max.z);
    sr_vertex(box.max.x, box.min.y, box.min.z);
    sr_vertex(box.max.x, box.max.y, box.min.z);

    sr_normal(0, -1, 0);
    sr_vertex(box.min.x, box.min.y, box.min.z);
    sr_vertex(box.max.x, box.min.y, box.min.z);
    sr_vertex(box.max.x, box.min.y, box.max.z);
    sr_vertex(box.max.x, box.min.y, box.max.z);
    sr_vertex(box.min.x, box.min.y, box.max.z);
    sr_vertex(box.min.x, box.min.y, box.min.z);
    
    sr_normal(0, +1, 0);
    sr_vertex(box.min.x, box.max.y, box.min.z);
    sr_vertex(box.max.x, box.max.y, box.max.z);
    sr_vertex(box.max.x, box.max.y, box.min.z);
    sr_vertex(box.max.x, box.max.y, box.max.z);
    sr_vertex(box.min.x, box.max.y, box.min.z);
    sr_vertex(box.min.x, box.max.y, box.max.z);
}

static void sr_rect(r2 rect, f32 z, u32 color) {
    sr_color(color);
    sr_normal(0, 0, +1);

    sr_vertex(rect.min.x, rect.min.y, z);
    sr_vertex(rect.max.x, rect.min.y, z);
    sr_vertex(rect.max.x, rect.max.y, z);
    sr_vertex(rect.max.x, rect.max.y, z);
    sr_vertex(rect.min.x, rect.max.y, z);
    sr_vertex(rect.min.x, rect.min.y, z);
}

#define SR_BITMAP_COUNT (256)

static const u64 sr_bitascii[SR_BITMAP_COUNT] = {
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

static void sr_init_bitmap(void) {
    u32 pixels[8][SR_BITMAP_COUNT * 8] = {0};

    for (int i = 0; i < SR_BITMAP_COUNT; ++i) {
        for (int y = 0; y < 8; ++y) for (int x = 0; x < 8; ++x) {
            u64 bit = y * 8 + x;

            if (sr_bitascii[i] & (1ull << bit)) {
                pixels[7 - y][8 * i + x] = 0xffffffff;
            }
        }
    }

    sr_bitmap_texture = gl_texture_create(pixels, SR_BITMAP_COUNT * 8, 8, false);

    gl_shader_use(sr_ui_text_shader);
    gl_uniform_i32(gl_shader_location(sr_ui_text_shader, "texture1"), 1);
}

static void sr_ascii(u8 c, f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color) {
    r2i tex_rect    = r2i(v2i(c * 8, 0), v2i(c * 8 + 8, 8));
    r2  rect        = r2(v2(x, y), v2(x + sx, y + sy));

    sr_texture_rect(tex_rect, rect, z, color);
}

static void sr_string(const char *str, f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color) {
    for (int i = 0; str[i] != '\0'; i++) {
        sr_ascii(str[i], x + i * sx, y, z, sx, sy, color);
    }
}

static void sr_string_format(f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color, const char* fmt, ...) {
    va_list list;
    char buffer[256];

    va_start(list, fmt);

    vsnprintf(buffer, 256, fmt, list);
    sr_string(buffer, x, y, z, sx, sy, color);

    va_end(list);
}

