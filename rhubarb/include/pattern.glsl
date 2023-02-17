float xadrez(vec2 p) {
    return abs(mod(p.x, 2.0) - 1.0) + abs(mod(p.y, 2.0) - 1.0) - 1.0;
}

float grid(vec2 p) {
    return abs(xadrez(p * rotate(radians(45))));
}

float hills(vec2 p) {
    return (sin(p.x) + sin(p.y)) * 0.5;
}

float copacabana(vec2 p, float A) {
    return sin(p.y - A * sin(p.x));
}