#pragma once

static inline f32
sine_ease_in(f32 t) {
    return 1 - cosf((t * PI) / 2);
}

static inline f32
sine_ease_out(f32 t) {
    return sinf((t * PI) / 2);
}

static inline f32
sine_ease_in_out(f32 t) {
    return -0.5 * (cosf(PI * t) - 1);
}

static inline f32
quad_ease_in(f32 t) {
    return t * t;
}

static inline f32
quad_ease_out(f32 t) {
    return 1 - (1 - t) * (1 - t);
}

static inline f32
quad_ease_in_out(f32 t) {
    f32 k = -2 * t + 2;
    return (t < 0.5)? (2 * t * t) : (1 - 0.5 * k * k);
}

static inline f32
cubic_ease_in(f32 t) {
    return t * t * t;
}

static inline f32
cubic_ease_out(f32 t) {
    f32 k = 1 - t;
    return 1 - k * k * k;
}

static inline f32
cubic_ease_in_out(f32 t) {
    f32 k = -2 * t + 2;
    return (t < 0.5)? (4 * t * t * t) : (1 - 0.5 * k * k * k);
}

static inline f32
quart_ease_in(f32 t) {
    return t * t * t * t;
}

static inline f32
quart_ease_out(f32 t) {
    f32 k = 1 - t; 
    return 1 - k * k * k * k;
}

static inline f32
quart_ease_in_out(f32 t) {
    f32 k = -2 * t + 2;
    return (t < 0.5)? (8 * t * t * t * t) : (1 - 0.5 * k * k * k * k);
}

static inline f32
quint_ease_in(f32 t) {
    return t * t * t * t * t;
}

static inline f32
quint_ease_out(f32 t) {
    f32 k = 1 - t;
    return 1 - k * k * k * k * k;
}

static inline f32
quint_ease_in_out(f32 t) {
    f32 k = -2 * t + 2;
    return (t < 0.5)? (16 * t * t * t * t * t) : (1 - 0.5 * k * k * k * k * k);
}

static inline f32
expo_ease_in(f32 t) {
    return (t == 0)? 0 : powf(2, 10 * t - 10);
}

static inline f32
expo_ease_out(f32 t) {
    return (t == 1)? 1 : (1 - powf(2, -10 * t));
}

static inline f32
expo_ease_in_out(f32 t) {
    return (t == 0)? 0 : (t == 1)? 1 : t < 0.5? powf(2, 20 * t - 10) / 2 : (2 - powf(2, -20 * t + 10)) / 2;
}

static inline f32
circ_ease_in(f32 t) {
    return 1 - sqrt(1 - (t * t));
}

static inline f32
circ_ease_out(f32 t) {
    return sqrt(1 - (t - 1) * (t - 1));
}

static inline f32
circ_ease_in_out(f32 t) {
    f32 k = 2 * t;
    f32 l = -2 * t + 2;

    return (t < 0.5)? 0.5 * (1 - sqrt(1 - k * k)) : 0.5 * (sqrt(1 - l * l) + 1);
}

static inline f32
back_ease_in(f32 t) {
    f32 c1 = 1.70158;
    f32 c3 = c1 + 1;

    return c3 * t * t * t - c1 * t * t;
}

static inline f32
back_ease_out(f32 t) {
    f32 c1  = 1.70158;
    f32 c3  = c1 + 1;
    f32 k   = t - 1;

    return 1 + c3 * k * k * k + c1 * k * k;
}

static inline f32
back_ease_in_out(f32 t) {
    f32 c1 = 1.70158;
    f32 c2 = c1 * 1.525;

    return (t < 0.5)?
        0.5 * (powf(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) :
        0.5 * (pow(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2);
}

static inline f32
elastic_ease_in(f32 t) {
    f32 c4 = (2 * PI) / 3;

    return (t == 0)?
        0 :
        (t == 1)?
        1 :
        -powf(2, 10 * t - 10) * sinf((t * 10 - 10.75) * c4);
}

static inline f32
elastic_ease_out(f32 t) {
    f32 c4 = (2 * PI) / 3;

    return t == 0?
        0 :
        t == 1?
        1 :
        powf(2, -10 * t) * sinf((t * 10 - 0.75) * c4) + 1;
}

static inline f32
elastic_ease_inout(f32 t) {
    f32 c5 = (2 * PI) / 4.5;

    return t == 0?
        0 :
        t == 1?
        1 :
        t < 0.5 ?
        -0.5 * (powf(2, 20 * t - 10)  * sinf((20 * t - 11.125) * c5)) :
        +0.5 * (powf(2, -20 * t + 10) * sinf((20 * t - 11.125) * c5)) + 1;
}

static inline f32
bounce_ease_out(f32 t) {
    f32 n1 = 7.5625;
    f32 d1 = 2.75;

    if (t < 1 / d1) {
        return n1 * t * t;
    } else if (t < 2 / d1) {
        t -= 1.5 / d1;
        return n1 * t * t + 0.75;
    } else if (t < 2.5 / d1) {
        t -= 2.25 / d1;
        return n1 * t * t + 0.9375;
    } else {
        t -= 2.625 / d1;
        return n1 * t * t + 0.984375;
    }
}

static f32
bounce_ease_in(f32 t) {
    return 1 - bounce_ease_out(t);
}

static inline f32
bounce_ease_in_out(f32 t) {
    return t < 0.5?
    0.5 * (1 - bounce_ease_out(1 - 2 * t)) :
    0.5 * (1 + bounce_ease_out(2 * t - 1));
}

