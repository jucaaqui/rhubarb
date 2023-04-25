
#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include "texture.h"

namespace rhu {

struct Shader {
    unsigned int id;
    std::string error;
    Size num_groups;
};

Shader new_shader(std::string source_code, Size num_inv, 
                  std::vector<Texture> const &textures);
void dispatch(Shader shader);

}
