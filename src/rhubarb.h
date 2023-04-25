
#pragma once

#include <vector>
#include <string>

#include "texture.h"

namespace rhu {

struct Sketch {
    int width;
    int height;
    std::string_view target;
    std::vector<Texture> textures;
    std::vector<std::string_view> shader_sources;
};

}
