
init_source = """

#include <common>

void main() {
    vec4 val = vec4(1.0, 1.0, 1.0, 1.0);

    if (rand(vec2(id)) < 0.1)
        val = vec4(0.0, 0.0, 0.0, 1.0);

    imageStore(world, id, val);
    imageStore(buff, id, val);
}

"""

next_source = """

bool is_alive(ivec2 coord) {
    return imageLoad(world, coord).r == 1.0;
}
    
void main() {
    int neighbors = 0;

    if (is_alive(id + ivec2(-1, -1))) neighbors++;
    if (is_alive(id + ivec2(-1,  0))) neighbors++;
    if (is_alive(id + ivec2(-1,  1))) neighbors++;
    if (is_alive(id + ivec2( 0, -1))) neighbors++;
    if (is_alive(id + ivec2( 0,  1))) neighbors++;
    if (is_alive(id + ivec2( 1, -1))) neighbors++;
    if (is_alive(id + ivec2( 1,  0))) neighbors++;
    if (is_alive(id + ivec2( 1,  1))) neighbors++;

    if (neighbors == 1 || neighbors >= 4)
        imageStore(buff, id, vec4(0.0, 0.0, 0.0, 1.0));
    else if (neighbors == 3)
        imageStore(buff, id, vec4(1.0, 1.0, 1.0, 1.0));
}

"""

swap_source = """
    
void main() {
    imageStore(world, id, imageLoad(buff, id));
}

"""

from rhubarb.window import *
from rhubarb.texture import *
from rhubarb.shader import *

def main():
    win = Window((900, 900), (40, 60))

    world = Texture("world", (1000, 1000))
    buff  = Texture("buff", world.size)

    init = Shader(init_source, world.size)
    next = Shader(next_source, world.size)
    swap = Shader(swap_source, world.size)

    win.set_render_target(world)
    init.dispatch()

    while not win.should_close():
        next.dispatch()
        swap.dispatch()
        win.draw()
        win.update()

if __name__ == "__main__":
    main()
