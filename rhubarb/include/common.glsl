const float PI = 3.14159265359;

float rand(float x) {
    return fract(sin(x + 5173.34) * 1022985.92);
}

float rand(vec2 v) {
    return fract(sin(v.x - rand(v.y) + 117.34) * 6262.46);
}

float rand(vec3 v) {
    return fract(sin(v.x - rand(v.y) + 348.42*v.z) * 2341.34);
}

vec3 rand3(vec3 v) {
    return vec3(
        rand(v.x),
        rand(v.y),
        rand(v.z)
    );
}

vec3 rand_vec3(float x) {
    return vec3(
        fract(sin(x + 583.38) * 9859.23),
        fract(sin(x + 178.79) * 1527.22),
        fract(sin(x + 946.54) * 4033.16)
    );
}

vec2 rand_vec2(float x) {
    return vec2(
        fract(sin(x + 263.39) * 4519),
        fract(sin(x + 928.76) * 2852)
    );
}



mat3 rotateX(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(1, 0, 0),
        vec3(0, c, -s),
        vec3(0, s, c)
    );
}

// Rotation matrix around the Y axis.
mat3 rotateY(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(c, 0, s),
        vec3(0, 1, 0),
        vec3(-s, 0, c)
    );
}

// Rotation matrix around the Z axis.
mat3 rotateZ(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(c, -s, 0),
        vec3(s, c, 0),
        vec3(0, 0, 1)
    );
}

mat3 rotate(vec3 theta) {
    return rotateX(theta.x) * rotateY(theta.y) * rotateZ(theta.z);
}

float smin( float a, float b, float k ) {
    float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
    return mix(b, a, h) - k * h * (1.0 - h);
}

float smoothmix(float a, float b, float x) {
    return (b - a) * (3*x*x - 2*x*x*x) + a;
}

vec2 rand_vec(vec2 p) {
    float theta = 2 * PI * rand(rand(p.x) - p.y);
    return vec2(cos(theta), sin(theta));
}

mat2 rotate(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat2(
        vec2(c,-s),
        vec2(s, c)
    );
}

// https://stackoverflow.com/questions/15095909/from-rgb-to-hsv-in-opengl-glsl
// All components are in the range [0…1], including hue.
vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 rgb2hsv(vec3 c) {
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

float max3(vec3 v) {
    return max(max(v.x, v.y), v.z);
}

float min3(vec3 v) {
    return min(min(v.x, v.y), v.z);
}

float rgb2mono(vec3 c) {
    // return dot(c * vec3(0.2126, 0.7152, 0.0722), vec3(1.0));
    return dot(c * vec3(0.299, 0.587, 0.114), vec3(1.0));
    // return dot(c * vec3(0.3, 0.59, 0.11), vec3(1.0));
}

float smax(float a, float b, float k) {
    return -smin(-a, -b, k);
}

float sxor(float a, float b, float k) {
    float m = -smin(a, b, k);
    
    return smax(
        -smax(a, b, k), 
         smin(a, b, k), 
        k
    );
}

// tsl colourspace
// source: https://en.wikipedia.org/wiki/TSL_color_space
vec3 rgb2tsl(vec3 rgb) {
    float R = rgb.x / (dot(rgb, vec3(1.0))) - 1.0 / 3.0;
    float G = rgb.y / (dot(rgb, vec3(1.0))) - 1.0 / 3.0;
    
    // calculate tint
    float t;
    if(G > 0.0)
        t = (0.5 / PI) * atan(R / G) + 0.25;
    else if(G < 0.0)
        t = (0.5 / PI) * atan(R / G) + 0.75;
    else
        t = 0.0;
    
    //calculate saturation
    float s = sqrt(1.8 * (R*R + G*G));

    //calculate lightness
    float l = dot(rgb * vec3(0.2126, 0.7152, 0.0722), vec3(1.0));

    return vec3(t, s, l);
}

vec3 tsl2rgb(vec3 tsl) {
    float x = tan(2.0*PI * tsl.x - 0.5*PI);
    
    float G;
    if(tsl.x > 0.5)
        G = -sqrt(5.0 / (9.0*(x*x + 1.0))) * tsl.y;
    else if(tsl.x < 0.5)
        G = sqrt(5.0 / (9.0*(x*x + 1.0))) * tsl.y;
    else
        G = 0.0;

    float R;
    if(tsl.x == 0.0)
        R = sqrt(5.0) / 3.0 * tsl.y;
    else
        R = x * G;

    float r = R + 1.0 / 3.0;
    float g = G + 1.0 / 3.0;

    float k = tsl.z / (0.2126*r + 0.7152*g + 0.0722);

    return vec3(k * r, k * g, k * (1.0 - r - g));
}

float lerp(float p0, float p1, float t) {
    return p0 * t + p1 * (1 - t);
}
