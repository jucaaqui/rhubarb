
#nframes 1000
#winsize 800 800
#maxfps 24
#fps 24
#output "videos/out*.mp4"
#target world

#saveonce world "images/slime/*.png"

#uniform vec3  camera_pos   0.00 1.00 0.01 1.00 0.00 1.00
#uniform float sensor_len   0.10 1.00
#uniform float audio        "res/3.wav"


/*

uniform float u_frame;
uniform vec2 u_mouse;
uniform float u_time;
uniform int u_nframes;

*/

#texture world "res/1.png"
#texture data  world
#texture tex   1000 1000 1000

#shaderonce world
#inlcude "glsl/common.glsl"

void main() {
    float t = float(i_frame) / float(LAST_FRAME);
    float seed = float(id.x) + rand(float(id.y));

    vec2 pos = rand_vec2(seed);
    float angle = rand(seed) * 2.0*PI;

    imageStore(data, id, vec4(pos, angle, 1.0));
}

#shaderend
#shader data

void main() {}

