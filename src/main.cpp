
#include <cctype>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>
#include <charconv>

#define STB_IMAGE_IMPLEMENTATION 
#include <stb_image.h>

//extern "C" {
//#include <libavcodec/avcodec.h>
//}

#include "texture.h"
#include "shader.h"
#include "lexer.h"
#include "rhubarb.h"

void framebuffer_size_callback(GLFWwindow* window, 
                               int width, int height);

const char *VERT_SOURCE = R"(
#version 330 core
layout (location = 0) in vec2 a_xy;
layout (location = 1) in vec2 a_uv;
out vec2 uv;
void main() {
    gl_Position = vec4(a_xy, 0.0, 1.0);
    uv = a_uv;
}
)";

const char *FRAG_SOURCE = R"(
#version 330 core
uniform sampler2D target;
in vec2 uv;
void main() {
    gl_FragColor = texture(target, uv);
}
)";

int main() {
    std::ifstream file("sketches/test.glsl");
    if (file.fail()) std::printf("couldn't read file!");
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string source = buffer.str();

    auto tokens = rhu::tokenize(source);

    /*
    for (auto const &token : tokens)
        std::cout << token.type << "    "
                  << token.val  << "    "
                  << token.row  << '\n';
                  */

    //std::cout << sketch.width << "   " << sketch.height << "\n";
    int width = 800;
    int height = 800;

    glfwInit();
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(
            width, height, "rhubarb", NULL, NULL);


    if (window == NULL) {
        std::cout << "failed to create GLFW window!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "failed to initialize GLAD!\n";
        return -1;
    }
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, 1.0f, 1.0f,
         0.5f,  0.5f, 1.0f, 0.0f
    };

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), 
                 vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          4 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          4 * sizeof(float), (void*) 8);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    unsigned int vert_shader;
    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &VERT_SOURCE, NULL);
    glCompileShader(vert_shader);

    int ok;
    char info_log[512];
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(vert_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" 
            << info_log << "\n";
    }

    unsigned int frag_shader;
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &FRAG_SOURCE, NULL);
    glCompileShader(frag_shader);

    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(frag_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" 
            << info_log << "\n";
    }

    unsigned int shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vert_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    std::vector<rhu::Texture> textures;

    int  img_width, img_height, num_channels;
    unsigned char *data1 = stbi_load("res/mononoke.png", &img_width, 
                                     &img_height, &num_channels, 4);
    textures.push_back(rhu::new_texture("texture1", 0, data1,
                       rhu::Size { img_width, img_height, 1 }));
    stbi_image_free(data1);

    unsigned char *data2 = stbi_load("res/nekobus.png", &img_width, 
                                     &img_height, &num_channels, 4);
    textures.push_back(rhu::new_texture("texture2", 1, data2,
                       rhu::Size { img_width, img_height, 1 }));
    stbi_image_free(data2);

    rhu::Shader shader = rhu::new_shader(
            source, rhu::Size { img_width, img_height, 1 }, textures);

    glUseProgram(shader_program);
    glUniform1i(glGetUniformLocation(shader_program, "target"), 0);

    while(!glfwWindowShouldClose(window)) {
        rhu::dispatch(shader);

        glUseProgram(shader_program);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    save(textures[0], "test5.png");

    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader_program);
    
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, 
                               int width, int height) {
    glViewport(0, 0, width, height);
}  
