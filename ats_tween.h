#pragma once

static f32 SineEaseIn(f32 t);
static f32 SineEaseOut(f32 t);
static f32 SineEaseInOut(f32 t);

static f32 QuadEaseIn(f32 t);
static f32 QuadEaseOut(f32 t);
static f32 QuadEaseInOut(f32 t);

static f32 CubicEaseIn(f32 t);
static f32 CubicEaseOut(f32 t);
static f32 CubicEaseInOut(f32 t);

static f32 QuartEaseIn(f32 t);
static f32 QuartEaseOut(f32 t);
static f32 QuartEaseInOut(f32 t);

static f32 QuintEaseIn(f32 t);
static f32 QuintEaseOut(f32 t);
static f32 QuintEaseInOut(f32 t);

static f32 ExpoEaseIn(f32 t);
static f32 ExpoEaseOut(f32 t);
static f32 ExpoEaseInOut(f32 t);

static f32 CircEaseIn(f32 t);
static f32 CircEaseOut(f32 t);
static f32 CircEaseInOut(f32 t);

static f32 BackEaseIn(f32 t);
static f32 BackEaseOut(f32 t);
static f32 BackEaseInOut(f32 t);

static f32 ElasticEaseIn(f32 t);
static f32 ElasticEaseOut(f32 t);
static f32 ElasticEaseInOut(f32 t);

static f32 BounceEaseIn(f32 t);
static f32 BounceEaseOut(f32 t);
static f32 BounceEaseInOut(f32 t);

// IMPL:

static f32 SineEaseIn(f32 t) {
    return 1 - cosf((t * PI) / 2);
}

static f32 SineEaseOut(f32 t) {
    return sinf((t * PI) / 2);
}

static f32 SineEaseInOut(f32 t) {
    return -0.5 * (cosf(PI * t) - 1);
}

static f32 QuadEaseIn(f32 t) {
    return t * t;
}

static f32 QuadEaseOut(f32 t) {
    return 1 - (1 - t) * (1 - t);
}

static f32 QuadEaseInOut(f32 t) {
    f32 k = -2 * t + 2;
    return (t < 0.5)? (2 * t * t) : (1 - 0.5 * k * k);
}

static f32 CubicEaseIn(f32 t) {
    return t * t * t;
}

static f32 CubicEaseOut(f32 t) {
    f32 k = 1 - t;
    return 1 - k * k * k;
}

static f32 CubicEaseInOut(f32 t) {
    f32 k = -2 * t + 2;
    return (t < 0.5)? (4 * t * t * t) : (1 - 0.5 * k * k * k);
}

static f32 QuartEaseIn(f32 t) {
    return t * t * t * t;
}

static f32 QuartEaseOut(f32 t) {
    f32 k = 1 - t; 
    return 1 - k * k * k * k;
}

static f32 QuartEaseInOut(f32 t) {
    f32 k = -2 * t + 2;
    return (t < 0.5)? (8 * t * t * t * t) : (1 - 0.5 * k * k * k * k);
}

static f32 QuintEaseIn(f32 t) {
    return t * t * t * t * t;
}

static f32 QuintEaseOut(f32 t) {
    f32 k = 1 - t;
    return 1 - k * k * k * k * k;
}

static f32 QuintEaseInOut(f32 t) {
    f32 k = -2 * t + 2;
    return (t < 0.5)? (16 * t * t * t * t * t) : (1 - 0.5 * k * k * k * k * k);
}

static f32 ExpoEaseIn(f32 t) {
    return (t == 0)? 0 : powf(2, 10 * t - 10);
}

static f32 ExpoEaseOut(f32 t) {
    return (t == 1)? 1 : (1 - powf(2, -10 * t));
}

static f32 ExpoEaseInOut(f32 t) {
    return (t == 0)? 0 : (t == 1)? 1 : t < 0.5? powf(2, 20 * t - 10) / 2 : (2 - powf(2, -20 * t + 10)) / 2;
}

static f32 CircEaseIn(f32 t) {
    return 1 - sqrt(1 - (t * t));
}

static f32 CircEaseOut(f32 t) {
    return sqrt(1 - (t - 1) * (t - 1));
}

static f32 CircEaseInOut(f32 t) {
    f32 k = 2 * t;
    f32 l = -2 * t + 2;

    return (t < 0.5)? 0.5 * (1 - sqrt(1 - k * k)) : 0.5 * (sqrt(1 - l * l) + 1);
}

static f32 BackEaseIn(f32 t) {
    f32 c1 = 1.70158;
    f32 c3 = c1 + 1;

    return c3 * t * t * t - c1 * t * t;
}

static f32 BackEaseOut(f32 t) {
    f32 c1  = 1.70158;
    f32 c3  = c1 + 1;
    f32 k   = t - 1;

    return 1 + c3 * k * k * k + c1 * k * k;
}

static f32 BackEaseInOut(f32 t) {
    f32 c1 = 1.70158;
    f32 c2 = c1 * 1.525;

    return (t < 0.5)?
        0.5 * (powf(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) :
        0.5 * (pow(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2);
}

static f32 ElasticEaseIn(f32 t) {
    f32 c4 = (2 * PI) / 3;

    return (t == 0)?
        0 :
        (t == 1)?
        1 :
        -powf(2, 10 * t - 10) * sinf((t * 10 - 10.75) * c4);
}

static f32 ElasticEaseOut(f32 t) {
    f32 c4 = (2 * PI) / 3;

    return t == 0?
        0 :
        t == 1?
        1 :
        powf(2, -10 * t) * sinf((t * 10 - 0.75) * c4) + 1;
}

static f32 ElasticEaseInOut(f32 t) {
    f32 c5 = (2 * PI) / 4.5;

    return t == 0?
        0 :
        t == 1?
        1 :
        t < 0.5 ?
        -0.5 * (powf(2, 20 * t - 10)  * sinf((20 * t - 11.125) * c5)) :
        +0.5 * (powf(2, -20 * t + 10) * sinf((20 * t - 11.125) * c5)) + 1;
}

static f32 BounceEaseIn(f32 t) {
    return 1 - BounceEaseOut(t);
}

static f32 BounceEaseOut(f32 t) {
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

static f32 BounceEaseInOut(f32 t) {
    return t < 0.5?
    0.5 * (1 - BounceEaseOut(1 - 2 * t)) :
    0.5 * (1 + BounceEaseOut(2 * t - 1));
}

