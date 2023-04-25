LAST_FRAME: -1;
WINDOW_SIZE: 800, 800;
MAX_FPS: 24;

float sensor_angle: 1.00;
float sensor_len:   0.10;
float turn_speed:   1.00;
float speed:        0.01;
float reduction:    0.10;
float audio:        "res/3.wav";

image2D world: "res/1.png";
image2D data:  world; // creates a texture of the same size as world
image3D tex:   1000, 1000, 1000;

shader setup: "

#inlcude "glsl/common.glsl"

void main() {

    float t = float(i_frame) / float(LAST_FRAME);

    float seed = float(id.x) + rand(float(id.y));

    vec2 pos = rand_vec2(seed);
    float angle = rand(seed) * 2.0*PI;


    imageStore(data, id, vec4(pos, angle, 1.0));
}

";

INIT: {
    setup.dispatch(tex);
};

DRAW: {
    reduction.gui(0.0, 1.0);
    slimemold.dispatch(tex);
    blur.dispatch(tex);

    data.save("output/test.png");
};

FINI: {
    data.save("output/test1.png");
};


/*

test.rh
test.rhu
test.rbb
test.rbrb
test.rhubarb

*/

