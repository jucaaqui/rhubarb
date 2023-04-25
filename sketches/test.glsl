
void main() {
    vec2 uv = id / texture1_size;

    if (length(uv - vec2(0.5)) > 0.3)
        imageStore(texture1, id, texture(s_texture2, id * 0.002));
}

