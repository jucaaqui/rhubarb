
#include <cmath>
#include <cstdio>
#include <iostream>
#define FMT_HEADER_ONLY
#include <fmt/core.h>

#include "shader.h"
#include "texture.h"

namespace rhu {

Shader new_shader(std::string source_code, Size num_inv, 
                  std::vector<Texture> const &textures) {

    Shader shader;
    std::string header = "#version 460 core\n";

    if (num_inv.x != 1 && num_inv.y == 1 && num_inv.z == 1) {
        shader.num_groups.x = std::ceil(num_inv.x / 8.0);
        shader.num_groups.y = 1;
        shader.num_groups.z = 1;
        header += "layout ("
            "local_size_x = 8, "
            "local_size_y = 1, "
            "local_size_z = 1) in;\n"
            "int id = int(gl_GlobalInvocationID.x);\n";
    }
    if (num_inv.x != 1 && num_inv.y != 1 && num_inv.z == 1) {
        shader.num_groups.x = std::ceil(num_inv.x / 8.0);
        shader.num_groups.y = std::ceil(num_inv.y / 8.0);
        shader.num_groups.z = 1;
        header += "layout ("
            "local_size_x = 8, "
            "local_size_y = 8, "
            "local_size_z = 1) in;\n"
            "ivec2 id = ivec2(gl_GlobalInvocationID.xy);\n";
    }
    if (num_inv.x != 1 && num_inv.y != 1 && num_inv.z != 1) {
        shader.num_groups.x = std::ceil(num_inv.x / 8.0);
        shader.num_groups.y = std::ceil(num_inv.y / 8.0);
        shader.num_groups.z = std::ceil(num_inv.z / 8.0);
        header += "layout ("
            "local_size_x = 8, "
            "local_size_y = 8, "
            "local_size_z = 8) in;\n"
            "ivec3 id = ivec3(gl_GlobalInvocationID.xyz);\n";
    }

    for (int i = 0; i < (int) textures.size(); ++i) {
        int dim = textures[i].target != GL_TEXTURE_1D ? 
                  textures[i].target != GL_TEXTURE_2D ? 3 : 2 : 1;

        header += fmt::format(
            "layout (binding = {}, rgba32f) uniform image{}D {};\n",
            i, dim, textures[i].name);
        header += fmt::format(
            "{} {}_size = {};\n",
            dim != 1 ? dim != 2 ? "vec3" : "vec2" : "float",
            textures[i].name,
            dim != 1 ? dim != 2 ? 
                fmt::format("vec3({}, {}, {})", textures[i].size.x, 
                                                textures[i].size.y,
                                                textures[i].size.z) : 
                fmt::format("vec2({}, {})",     textures[i].size.x, 
                                                textures[i].size.y) : 
                fmt::format("float({})",        textures[i].size.x));
        header += fmt::format(
            "layout (binding = {}) uniform sampler{}D s_{};\n",
            i, dim, textures[i].name);
    }

    header += "#line 1\n";

    source_code = header + source_code;

    std::cout << source_code;

    std::cout << source_code;

    const char *source_c_str = source_code.c_str();

    unsigned int compute_id = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute_id, 1, &source_c_str, NULL);
    glCompileShader(compute_id);

    int ok;
    char info_log[512];
    glGetShaderiv(compute_id, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(compute_id, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" 
            << info_log << std::endl;
        shader.error = info_log;
    }

    shader.id = glCreateProgram();
    glAttachShader(shader.id, compute_id);
    glDeleteShader(compute_id);
    glLinkProgram(shader.id);

    return shader;
}

void dispatch(Shader shader) {
    glUseProgram(shader.id);
    glDispatchCompute(shader.num_groups.x, 
                      shader.num_groups.y, 
                      shader.num_groups.z);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void specify_textures(Shader shader, int num_textures, Texture *textures) {
    for (int i = 0; i < num_textures; i++) {
        glUseProgram(shader.id);
        glUniform1i(glGetUniformLocation(
                    shader.id, textures[i].name.c_str()), 
                    textures[i].unit);
        glUniform1i(glGetUniformLocation(
                    shader.id, ("s_" + textures[i].name).c_str()), 
                    textures[i].unit);
    }
}
}
