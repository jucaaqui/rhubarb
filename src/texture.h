
#pragma once

#include <glad/glad.h>
#include <string>
#include <functional>
#include <iostream>

namespace rhu {

struct Size {
    int x;
    int y;
    int z;
};

struct Texture {
    std::string name;
    unsigned int id;
    unsigned int target;
    unsigned int unit;
    Size size;
};

Texture new_texture(std::string_view name, unsigned int unit, 
                    unsigned char *data, Size size);
unsigned char *get_data(Texture texture, unsigned int format);
void save(Texture texture, std::string path);

}

