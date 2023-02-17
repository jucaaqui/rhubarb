
init_source = """

#include <common>

void main() {
    float seed = float(id.x) + rand(float(id.y));

    vec2 pos = rand_vec2(seed);
    float angle = rand(seed) * 2.0*PI;

    imageStore(data, id, vec4(pos, angle, 1.0));
}

"""

slimemold_source = """

float change_angle(float angle, vec2 pos) {
    float sample0 = texture(world_sampler, pos
        + vec2(cos(angle + SENSOR_ANGLE) * SENSOR_LENGTH,
               sin(angle + SENSOR_ANGLE) * SENSOR_LENGTH)).r;

    float sample1 = texture(world_sampler, pos
        + vec2(cos(angle - SENSOR_ANGLE) * SENSOR_LENGTH,
               sin(angle - SENSOR_ANGLE) * SENSOR_LENGTH)).r;

    if(sample0 > sample1)
        return angle + TURN_SPEED;
    else
        return angle - TURN_SPEED;
}

void main() {
    vec4 dat = imageLoad(data, id);
    
    vec2 pos = dat.xy;
    float angle = dat.z;

    pos = mod(pos + vec2(cos(angle), sin(angle)) * SPEED, 1.0);

    angle = change_angle(angle, pos);

    // if(new_pos.x < 0.0 || new_pos.y < 0.0 || new_pos.x > 1.0 || new_pos.y > 1.0)
    //     angle = rand(float(cell_id)) * 2.0*PI;
    // // else
    // pos = new_pos;

    imageStore(data, id, vec4(pos, angle, 1.0));
    imageStore(world, ivec2(pos * world_size), vec4(vec3(1.0), 1.0));
}

"""

blur_source = """

void main() {
    imageStore(world, id, 
        vec4((imageLoad(world, id) - REDUCTION).xyz, 1.0));
}

"""

from . import rhubarb

import math
import imgui
from imgui.integrations.glfw import GlfwRenderer

def main():
    w = 1920 // 2 - 20*3
    win = Window((w, w), (20*2, 20*3))

    data = Texture("data", (1000, 1000))
    world = Texture("world", (1000, 1000))

    imgui.create_context()
    impl = GlfwRenderer(win.window)

    sensor_angle = Uniform("float", "SENSOR_ANGLE", math.radians(10.0))
    sensor_length = Uniform("float", "SENSOR_LENGTH", 0.05)
    turn_speed = Uniform("float", "TURN_SPEED", math.radians(10.0))
    speed = Uniform("float", "SPEED", 0.001)

    # blur_radius = Uniform("float", "BLUR_RADIUS", 0.05)
    reduction = Uniform("float", "REDUCTION", 0.1)

    init = Shader(init_source, data.size)
    slimemold = Shader(slimemold_source, data.size)
    blur = Shader(blur_source, world.size)

    init.dispatch()

    win.set_render_target(world)

    while not win.should_close():
        slimemold.dispatch()
        blur.dispatch()
        win.draw()

        impl.process_inputs()
        imgui.new_frame()
        imgui.begin("test")

        _, turn_speed.val       = imgui.slider_float("turn speed", turn_speed.val, 0.0, 1.0, "%.2f", 1.0)
        _, sensor_angle.val     = imgui.slider_float("sensor angle", sensor_angle.val, 0.0, 1.0, "%.2f", 1.0)
        _, sensor_length.val    = imgui.slider_float("sensor length", sensor_length.val, 0.0, 0.1, "%.4f", 1.0)
        _, speed.val            = imgui.slider_float("speed", speed.val, 0.0, 0.05, "%.4f", 1.0)
        _, reduction.val        = imgui.slider_float("reduction", reduction.val, 0.0, 0.5, "%.2f", 1.0)
        imgui.text(str(win.mspf))

        imgui.end()
        imgui.render()
        impl.render(imgui.get_draw_data())


        win.update()

if __name__ == "__main__": 
    main()
