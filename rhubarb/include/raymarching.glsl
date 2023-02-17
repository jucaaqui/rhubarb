
const int MAX_MARCHES = 500;
const float MAX_DIST = 100.0;
const float MIN_DIST = 0.001;

float scene_sdf(vec3 p);

vec3 ray_march(vec3 ro, vec3 rd) {
    vec3 p = ro;

    for(int i = 0; i < MAX_MARCHES; i++) {
        float d = scene_sdf(p);
        p = p + d * rd;
        if(d > MAX_DIST || d < MIN_DIST) break;
    }
    
    return p;
}

vec3 get_normal(vec3 p) {
    float d = scene_sdf(p);
    vec2 e = vec2(MIN_DIST, 0);

    vec3 n = d - vec3(
        scene_sdf(p - e.xyy),
        scene_sdf(p - e.yxy),
        scene_sdf(p - e.yyx)
    );
    
    return normalize(n);
}