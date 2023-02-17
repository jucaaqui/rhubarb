
float circle_sdf(vec2 p, float r) {
    return length(p) - r;
}

float copacabana(vec2 p, float A, float T, float k) {
    return mod(p.y + A * sin(2.0 * PI * p.x / T), k) / k;
}

float xadrez(vec2 p) {
    float x = mod(p.x, 1.0) - 0.5;
    float y = mod(p.y, 1.0) - 0.5;

    return min(max(x, y), -min(x, y));
}