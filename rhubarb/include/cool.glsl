void main() {
    vec2 uv = vec2(id) / vec2(target_size);
    float val = rgb2mono(texture(img_sampler, uv).rgb);

    vec2 d = val - vec2(
        rgb2mono(texture(img_sampler, uv - vec2(0.001, 0.0)).rgb), 
        rgb2mono(texture(img_sampler, uv - vec2(0.0, 0.001)).rgb));

    uv *= rotate(angle(d) * 2.0*PI);

    if(abs(mod(uv.x * 1.0, 2.0) - 1.0) < val)
        imageStore(target, id, vec4(vec3(1.0), 1.0));
    else
        imageStore(target, id, vec4(vec3(0.0), 1.0));

    // main0();
}