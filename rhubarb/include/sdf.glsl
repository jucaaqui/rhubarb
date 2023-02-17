
// raymarching SDFs //

float sphere_sdf(vec3 p, float r) {
    return length(p) - r;
}

float circle_sdf(vec2 p, float r) {
    return length(p) - r;
}

float box_sdf(vec3 p, vec3 b) {
    vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float torus_sdf(vec3 p, vec2 t) {
    vec2 q = vec2(length(p.xz)-t.x,p.y);
    return length(q)-t.y;
}

float line_sdf(vec3 p, vec3 a, vec3 b, float r) {
    vec3 pa = p - a, ba = b - a;
    float h = clamp(dot(pa,ba) / dot(ba,ba), 0.0, 1.0);
    return length(pa - ba*h) - r;
}

float prism_sdf(vec3 p, vec2 h) {
    vec3 q = abs(p);
    return max(q.z-h.y,max(q.x*0.866025+p.y*0.5,-p.y)-h.x*0.5);
}

float box_frame_sdf(vec3 p, vec3 b, float e) {
        p = abs(p  )-b;
    vec3 q = abs(p+e)-e;
    return min(min(
        length(max(vec3(p.x,q.y,q.z),0.0))+min(max(p.x,max(q.y,q.z)),0.0),
        length(max(vec3(q.x,p.y,q.z),0.0))+min(max(q.x,max(p.y,q.z)),0.0)),
        length(max(vec3(q.x,q.y,p.z),0.0))+min(max(q.x,max(q.y,p.z)),0.0));
}

// random SDFs

float random_sphere_sdf(vec3 p, float seed) {
    return sphere_sdf(
        (p + noise_vec3(seed) * 2.0 - 1.0),
        max(noise(seed) * 0.8 - 0.2, 0.05)
    );
}

float random_box_sdf(vec3 p, float seed) {
    return box_sdf(
        p * rotate(mod(noise_vec3(seed * 0.2 + 149.2) * 5.0, 1.0) * 2*PI) + (noise_vec3(seed + 701.4) * 2.0 - 1.0) * 0.8,
        vec3(
            max(noise(seed + 111.1) * 0.9 - 0.2, 0.05),
            max(noise(seed + 222.2) * 0.9 - 0.2, 0.05),
            max(noise(seed + 333.3) * 0.9 - 0.2, 0.05)
        )
    );
}

float random_torus_sdf(vec3 p, float seed) {
    return torus_sdf(
        (p * rotate(mod(noise_vec3(seed * 0.2 + 345.0) * 5.0, 1.0) * 2*PI) + noise_vec3(seed + 340.0) * 2.0 - 1.0),
        vec2(
            noise(seed + 444.4) * 0.7,
            max(noise(seed + 333.3) * 0.5 - 0.2, 0.05)
        )
    );
}

float random_line_sdf(vec3 p, float seed) { 
    return line_sdf(
        p,
        (noise_vec3(seed + 888.8) * 2.0 - 1.0) * 1.3,
        (noise_vec3(seed + 666.6) * 2.0 - 1.0) * 1.3,
        max(noise(seed) * 0.4 - 0.2, 0.05)
    );
}

float random_prism_sdf(vec3 p, float seed) { 
    return prism_sdf(
        p * rotate(mod(noise_vec3(seed * 0.2 + 521.1) * 5.0, 1.0) * 2*PI) + (noise_vec3(seed + 931.4) * 2.0 - 1.0) * 0.8,
        vec2(
            max(noise(seed + 222.2) - 0.2, 0.05),
            max(noise(seed + 333.3) - 0.2, 0.05)
        )
    );
}