#pragma once

#include "ats_tool.h"

#define GLSL_SHADER(shader) "#version 330 core\n" #shader

const char* shader_vertex_simple =
"#version 430 core\n"
"layout (location = 0) in vec3 vp;\n"
"uniform mat4 PV;\n"
"uniform mat4 M;\n"
"void main() {\n"
"   gl_Position = PV * M * vec4(vp, 1.0);\n"
"}\n";

const char* shader_fragment_simple =
"#version 430 core\n"
"out vec4 frag_color;\n"
"uniform vec4 color;\n"
"void main() {\n"
"   frag_color = color;\n"
"}\n";

unsigned int ShaderCompile(const char* source, unsigned int type) {
    int success;

    char info_log[512];

    unsigned int shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        puts(info_log);

        exit(EXIT_FAILURE);
    }

    return shader;
}

unsigned int ShaderLinkProgramVF(unsigned int vertex_shader, unsigned int fragment_shader) {
    int     success;
    char    info_log[512];

    unsigned int shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        puts(info_log);

        exit(EXIT_FAILURE);
    }

    return shader_program;
}

unsigned int ShaderLinkProgramVFG(unsigned int vertex_shader, unsigned int fragment_shader, unsigned int geometry_shader) {
    int     success;
    char    info_log[512];

    unsigned int shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glAttachShader(shader_program, geometry_shader);

    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        puts(info_log);

        exit(EXIT_FAILURE);
    }

    return shader_program;
}

unsigned int ShaderLinkProgramV(unsigned int vertex_shader) {
    int success;
    char info_log[512];

    unsigned int shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);

    const char* varyings[] = { "pos1", "vel1" };
    glTransformFeedbackVaryings(shader_program, 2, varyings, GL_INTERLEAVED_ATTRIBS);

    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        puts(info_log);

        exit(EXIT_FAILURE);
    }

    return shader_program;
}

unsigned int ShaderLoadFromMemory(const char *vs, const char *fs, const char *gs) {
    unsigned int shader_program = 0;

    if (vs != NULL && fs != NULL && gs != NULL) {
        unsigned int vertex     = ShaderCompile(vs, GL_VERTEX_SHADER);
        unsigned int fragment   = ShaderCompile(fs, GL_FRAGMENT_SHADER);
        unsigned int geometry   = ShaderCompile(gs, GL_GEOMETRY_SHADER);

        shader_program  = ShaderLinkProgramVFG(vertex, fragment, geometry);

        glUseProgram(shader_program);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    } else if (vs != NULL && fs != NULL && gs == NULL) {
        unsigned int vertex     = ShaderCompile(vs, GL_VERTEX_SHADER);
        unsigned int fragment   = ShaderCompile(fs, GL_FRAGMENT_SHADER);

        shader_program  = ShaderLinkProgramVF(vertex, fragment);

        glUseProgram(shader_program);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    } else if (vs != NULL && fs == NULL) {
        unsigned int vertex = ShaderCompile(vs, GL_VERTEX_SHADER);

        shader_program = ShaderLinkProgramV(vertex);

        glUseProgram(shader_program);
        glDeleteShader(vertex);
    }

    return shader_program;
}

unsigned int ShaderLoadFromFiles(const char *vertex_path, const char *fragment_path, const char *geometry_path) {
    char *vs = NULL;
    char *fs = NULL;
    char *gs = NULL;

    if (vertex_path)    { vs = FileReadStr(vertex_path); }
    if (fragment_path)  { fs = FileReadStr(fragment_path); }
    if (geometry_path)  { gs = FileReadStr(geometry_path); }

    unsigned int program = ShaderLoadFromMemory(vs, fs, gs);

    if (vs) free(vs);
    if (fs) free(fs);
    if (gs) free(gs);

    return program;
}

void ShaderUse(unsigned int shader) {
    glUseProgram(shader);
}

unsigned int ShaderGetLocation(unsigned int shader, const char *var_name) {
    return glGetUniformLocation(shader, var_name);
}

void ShaderSeti(unsigned int shader, const char *loc, int n) {
    glUniform1i(glGetUniformLocation(shader, loc), n);
}

void ShaderSetf(unsigned int shader, const char *loc, float n) {
    glUniform1f(glGetUniformLocation(shader, loc), n);
}

void ShaderSet2f(unsigned int shader, const char *loc, float a, float b) {
    glUniform2f(glGetUniformLocation(shader, loc), a, b);
}

void ShaderSet3f(unsigned int shader, const char *loc, float a, float b, float c) {
    glUniform3f(glGetUniformLocation(shader, loc), a, b, c);
}

void ShaderSet4f(unsigned int shader, const char *loc, float a, float b, float c, float d) {
    glUniform4f(glGetUniformLocation(shader, loc), a, b, c, d);
}

void ShaderSet2v(unsigned int shader, const char *loc, const float *u) {
    glUniform2fv(glGetUniformLocation(shader, loc), 1, u);
}

void ShaderSet3v(unsigned int shader, const char *loc, const float *u) {
    glUniform3fv(glGetUniformLocation(shader, loc), 1, u);
}

void ShaderSet4v(unsigned int shader, const char *loc, const float *u) {
    glUniform4fv(glGetUniformLocation(shader, loc), 1, u);
}

void ShaderSetMat4(unsigned int shader, const char *loc, const float *m) {
    glUniformMatrix4fv(glGetUniformLocation(shader, loc), 1, GL_FALSE, m);
}

