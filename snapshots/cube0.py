
SHADER = """

#include <common>
#include <noise>
#include <sdf>
#include <raymarching>

float scene_sdf(vec3 p) {

    float scene = 1000.0;

    //scene = min(scene,
    //    sphere_sdf(fract(p) - vec3(0.5),
    //    rand(rand(floor(p))) * 0.4)
    //);

    //scene = min(scene, -box_sdf(fract(p) - vec3(0.5), vec3(0.6)));

    scene = min(scene,
        
        snoise(p.xy) - p.z

    );

    return scene;
}

vec4 get_col(vec2 uv) {
    vec2 project_plane = vec2(uv * 2.0 - 1.0);

    vec3 ro = vec3(time, 0.0, 5.0);// * rotateZ(time*2.0);
    vec3 rd = normalize(vec3(1.0, project_plane));

    vec3 p = ray_march(ro, rd);

    vec3 d = get_normal(p);

    vec4 col = vec4(d * 0.5 + 0.5, 1.0);

    if (scene_sdf(p) > 1.0)
        col = vec4(vec3(0.0), 1.0);

    return col;
}


void main() {
    vec2 uv = vec2(id) / vec2(target_size);
    vec2 pix_dim = 1.0 / vec2(target_size);

    //vec4 col = (
    //      get_col(uv + vec2(0.0, 0.0) * pix_dim),
    //    //+ get_col(uv + vec2(0.0, 0.5) * pix_dim)
    //    //+ get_col(uv + vec2(0.5, 0.0) * pix_dim)
    //    //+ get_col(uv + vec2(0.5, 0.5) * pix_dim)
    //);// * 0.25;

    imageStore(target, id, get_col(uv));
}

"""

from rhubarb.window import *
from rhubarb.shader import *
from rhubarb.texture import *
from rhubarb.video import *
from glfw.GLFW import *

from PIL import Image

def main():
    w = 1920 // 2 - 20*3
    win = Window((w, w), (20*2, 20*3))

    size = 1000
    target = Texture("target", (2000, 2000))

    time = Uniform("float", "time", 0.0)

    drip = Shader(SHADER, target.size)

    win.set_render_target(target)

    #vid = VideoOut(target.size, 30.0, "lines0.avi")

    while not win.should_close():
        
        time.val += 0.03
        drip.dispatch()
        win.draw()
        win.update()

        if time.val % 30.0 == 0:
            print(win.mspf)

    #    vid.write(target.get_data())

if __name__ == "__main__": 
    main()
