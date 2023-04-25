
#include "texture.h"
#include <cstdlib>
#include <iostream>
#include <stb_image_write.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace rhu {

// expects data to be in RGBA format
Texture new_texture(std::string_view name, unsigned int unit, 
                    unsigned char *data, Size size) {
    Texture texture;
    texture.name = name;
    texture.unit = unit;
    texture.size = size;

    
    if (size.x != 1 && size.y == 1 && size.z == 1)
        texture.target = GL_TEXTURE_1D;
    if (size.x != 1 && size.y != 1 && size.z == 1)
        texture.target = GL_TEXTURE_2D;
    if (size.x != 1 && size.y != 1 && size.z != 1){
        texture.target = GL_TEXTURE_3D;
    }

    glActiveTexture(GL_TEXTURE0 + unit);
    glGenTextures(1, &texture.id);
    glBindTexture(texture.target, texture.id);

    glTexParameteri(texture.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(texture.target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (texture.target == GL_TEXTURE_1D) glTexImage1D(
            texture.target, 0, GL_RGBA32F, size.x, 
            0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    if (texture.target == GL_TEXTURE_2D) glTexImage2D(
            texture.target, 0, GL_RGBA32F, size.x, size.y,
            0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    if (texture.target == GL_TEXTURE_3D) glTexImage3D(
            texture.target, 0, GL_RGBA32F, size.x, size.y, size.z,
            0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glBindImageTexture(unit, texture.id, 0, GL_TRUE, 
                       0, GL_READ_WRITE, GL_RGBA32F);

    return texture;
}

// get buffer of texture data, remember to free output!
unsigned char *get_data(Texture texture) {
    unsigned char *data = (unsigned char *) malloc(
            texture.size.x * texture.size.y * texture.size.z
            * 4 * sizeof(unsigned char));
    glBindTexture(texture.target, texture.id);
    glGetTexImage(texture.target, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(texture.target, 0);

    return data;
}

void save(Texture texture, std::string path) {
    unsigned char *data = get_data(texture);
    glGetTexImage(texture.target, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_write_png(path.c_str(), texture.size.x, texture.size.y, 
                   4, data, 4 * texture.size.x);
    free(data);
}
}
