float noise(float x) {
    float f = (rand(floor(x)) * 2 - 1) * fract(x) + rand(floor(x + 1000.0)) - 0.5;
    
    float m = (rand(floor(x + 1)) * 2 - 1);
    float g = m * fract(x) - m + rand(floor(x + 1001.0)) - 0.5;

    return (smoothstep(1.0, 0.0, fract(x)) * f + smoothstep(0.0, 1.0, fract(x)) * g) * 0.5 + 0.5;
}

vec3 noise_vec3(float x) {
    return vec3(
        noise(x + 100.0), 
        noise(x + 200.0), 
        noise(x + 300.0)
    );
}

// Simplex 2D noise
//source: https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
vec3 permute(vec3 x) { return mod(((x*34.0)+1.0)*x, 289.0); }

float snoise(vec2 v) {
    const vec4 C = vec4(0.211324865405187, 0.366025403784439,
            -0.577350269189626, 0.024390243902439);
    vec2 i  = floor(v + dot(v, C.yy) );
    vec2 x0 = v -   i + dot(i, C.xx);
    vec2 i1;
    i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
    vec4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;
    i = mod(i, 289.0);
    vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
    + i.x + vec3(0.0, i1.x, 1.0 ));
    vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy),
        dot(x12.zw,x12.zw)), 0.0);
    m = m*m ;
    m = m*m ;
    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;
    m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
    vec3 g;
    g.x  = a0.x  * x0.x  + h.x  * x0.y;
    g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    return 130.0 * dot(m, g);
}

float loop_noise(float t, float r, float offset) {
    float angle = 2.0 * PI * t;
    return snoise(vec2(cos(angle) * r, sin(angle) * r + offset)) * 0.5 + 0.5;
}

vec2 loop_noise_vec2(float t, float r, float offset) {
    float angle = 2.0 * PI * t;
    vec2 p = vec2(cos(angle) * r, sin(angle) * r + offset);

    return vec2(
        snoise(p + vec2(10.0)),
        snoise(p + vec2(20.0))
    );
}

vec3 loop_noise_vec3(float t, float r, float offset) {
    float angle = 2.0 * PI * t;
    vec2 p = vec2(cos(angle) * r, sin(angle) * r + offset);

    return vec3(
        snoise(p + vec2(10.0)),
        snoise(p + vec2(20.0)),
        snoise(p + vec2(30.0))
    );
}

mat3 loop_rotate(float t, float r, float offset) {
    return rotate(mod(loop_noise_vec3(t, r, offset) * 5.0, 1.0) * 2.0*PI);
}


vec2 noise_vec2(float x) {
    return vec2(
        noise(x + 1000.0), 
        noise(x + 2000.0)
    );
}


float norm_snoise(vec2 p) {
    return snoise(p) * 0.5 + 0.5;
}

