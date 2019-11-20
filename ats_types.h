#pragma once

// ================================================= MATH TYPES ============================================== //

typedef union v2 {
    struct { float x, y; };
    float array[2];
} v2;

typedef union v3 {
    struct { float x, y, z; };
    struct { float r, g, b; };
    struct { v2 xy; };
    float array[3];
} v3;

typedef union v4 {
    struct { float x, y, z, w; };
    struct { float r, g, b, a; };
    struct { v3 xyz; };
    struct { v3 rgb; };
    struct { v2 xy; };
    float array[4];
} v4;

typedef union m2 {
    struct { v2 x, y; };
    float array[4];
} m2;

typedef union m3 {
    struct { v3 x, y, z; };
    float array[9];
} m3;

typedef union m4 {
    struct { v4 x, y, z, w; };
    float array[16];
} m4;

// -------------------------------------------- 2D --------------------------------------- //

static v2 V2Neg(v2 a)
{
    return (v2) { -a.x, -a.y };
}

static v2 V2Add(v2 a, v2 b)
{
    return (v2) { a.x + b.x, a.y + b.y };
}

static v2 V2Sub(v2 a, v2 b)
{
    return (v2) { a.x - b.x, a.y - b.y };
}

static v2 V2Mul(v2 a,  v2 b)
{
    return (v2) { a.x * b.x, a.y * b.y };
}

static v2 V2Scale(v2 a, float s)
{
    return (v2) { a.x * s, a.y * s };
}

static float V2Dot(v2 a, v2 b)
{
    return a.x * b.x + a.y * b.y;
}

static float V2Len(v2 v)
{
    return Sqrt(v.x * v.x + v.y * v.y);
}

static float V2LenSq(v2 v)
{
    return v.x * v.x + v.y * v.y;
}

static float V2DistSq(v2 a, v2 b)
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return dx * dx + dy * dy;
}

static float V2Dist(v2 a, v2 b)
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;

    return Sqrt(dx * dx + dy * dy);
}

static v2 V2Norm(v2 v)
{
    float k = RSqrt(v.x * v.x + v.y * v.y);

    return V2Scale(v, k);
}

static v2 V2Lerp(v2 a, v2 b, float t)
{
    return (v2) {
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y)
    };
}

static v2 V2Proj(v2 a, v2 b)
{
    float ab_bb = (a.x * b.x + a.y * b.y) / (b.x * b.x + b.y * b.y);

    return (v2) {
        b.x * ab_bb,
        b.y * ab_bb
    };
}

static bool V2CircleIntersect(v2 p0, float r0, v2 p1, float r1)
{
    float dx = p1.x - p0.x;
    float dy = p1.y - p0.y;

    float r = r0 + r1;

    return (dx * dx + dy * dy) <  r * r;
}

static bool V2SegmentIsIntersectingCircle(v2 start, v2 end, v2 pos, float rad)
{
    v2 a = V2Sub(start, pos);
    v2 b = V2Sub(end, start);

    if ((a.x * a.x + a.y * a.y) > (b.x * b.x + b.y * b.y))
        return 0;

    v2 seg = V2Sub(end, start);
    v2 cir = V2Sub(pos, start);

    float dot_sc = seg.x * cir.x + seg.y * cir.y;

    if (dot_sc < 0.0f)
        return 0;

    float proj = dot_sc / (seg.x * seg.x + seg.y * seg.y);

    seg = V2Scale(seg, proj);
    seg = V2Sub(seg, cir);

    return (seg.x * seg.x + seg.y * seg.y) < (rad * rad);
}

// ------------------------------- v3 ------------------------------------ //

static v3 v3Neg(v3 a)
{
    return (v3) { -a.x, -a.y, -a.z };
}

static v3 V3Add(v3 a, v3 b) 
{
    return (v3) { a.x + b.x, a.y + b.y, a.z + b.z };
}

static v3 V3Sub(v3 a, v3 b) 
{
    return (v3) { a.x - b.x, a.y - b.y, a.z - b.z };
}

static v3 V3Mul(v3 a, v3 b)
{ 
    return (v3) { a.x * b.x, a.y * b.y, a.z * b.z };
}

static v3 V3Scale(v3 a, float s)
{
    return (v3) { a.x * s, a.y * s, a.z * s };
}

static float V3Dot(v3 a, v3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static float V3Len(v3 v)
{
    return Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

static float V3LenSq(v3 v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

static float V3DistSq(v3 a, v3 b)
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float dz = b.z - a.z;

    return dx * dx + dy * dy + dz * dz;
}

static float V3Dist(v3 a, v3 b)
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float dz = b.z - a.z;

    return Sqrt(dx * dx + dy * dy + dz * dz);
}

static v3 V3Norm(v3 v)
{
    float k = RSqrt(v.x * v.x + v.y * v.y + v.z * v.z);

    return V3Scale(v, k);
}

static v3 V3Lerp(v3 a, v3 b, float t)
{
    return (v3) {
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y),
        a.z + t * (b.z - a.z)
    };
}

static v3 V3Proj(v3 a, v3 b)
{
    float ab_bb = (a.x * b.x + a.y * b.y + a.z * b.z) / (b.x * b.x + b.y * b.y + b.z * b.z);

    return (v3) {
        b.x * ab_bb,
        b.y * ab_bb,
        b.z * ab_bb,
    };
}

// ------------------------------- vec4 ------------------------------------

static v4 V4Neg(v4 a)
{
    return (v4) { -a.x, -a.y, -a.z, -a.w };
}

static v4 V4Add(v4 a, v4 b)
{
    return (v4) { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

static v4 V4Sub(v4 a, v4 b)
{
    return (v4) { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

static v4 V4Mul(v4 a, v4 b)
{
    return (v4) { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}

static v4 V4Scale(v4 a, float s)
{
    return (v4) { a.x * s, a.y * s, a.z * s, a.w * s };
}

static float V4Dot(v4 a, v4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

static float V4Len(v4 v)
{
    return Sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

static float V4LenSq(v4 v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}

static float V4DistSq(v4 a, v4 b)
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float dz = b.z - a.z;
    float dw = b.w - a.w;

    return dx * dx + dy * dy + dz * dz + dw * dw;
}

static float V4Dist(v4 a, v4 b)
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float dz = b.z - a.z;
    float dw = b.w - a.w;

    return Sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}

static v4 V4Norm(v4 v)
{
    float k = RSqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);

    return (v4) {
        v.x * k,
        v.y * k,
        v.z * k,
        v.w * k
    };
}

static v4 V4Lerp(v4 a, v4 b, float t)
{
    return (v4) {
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y),
        a.z + t * (b.z - a.z),
        a.w + t * (b.w - a.w)
    };
}

static v4 V4Proj(v4 a, v4 b)
{
    float ab_bb = (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w) / (b.x * b.x + b.y * b.y + b.z * b.z + b.w * b.w);

    return (v4) {
        b.x * ab_bb,
        b.y * ab_bb,
        b.z * ab_bb,
        b.w * ab_bb
    };
}

// --------------------------------- vecN ---------------------------------- //

static float V2Det(v2 a, v2 b)
{
    return a.x * b.y - b.x * a.y;
}

static float V2GetAngle(v2 a, v2 b)
{
    float det = a.x * b.y - b.x * a.y;
    float dot = a.x * b.x + a.y * b.y;

    return atan2f(det, dot);
}

static v2 V2Rotate(v2 a, float rot)
{
    float c = cosf(rot);
    float s = sinf(rot);

    return (v2) {
        a.x * c - a.y * s,
        a.x * s + a.y * c
    };
}

static v3 V3Cross(v3 a, v3 b)
{
    return (v3) {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

static v2 V2Reflect(v2 v, v2 n)
{
	float p  = 2.0f * V2Dot(v, n);

    return (v2) {
        v.x - p * n.x,
        v.y - p * n.y
    };
}

static v3 V3Reflect(v3 v, v3 n)
{
	float p = 2.0f * V3Dot(v, n);

    return (v3) {
        v.x = v.x - p * n.x,
        v.y = v.y - p * n.y,
        v.z = v.z - p * n.z
    };
}

static v2 V2Spline(float f, v2 a, v2 b, v2 c, v2 d)
{
	float i = 1.0f - f;

    return (v2) {
        ((d.x * f + c.x * i) * f + (c.x * f + b.x * i) * i) * f + ((c.x * f + b.x * i) * f + (b.x * f + a.x * i) * i) * i,
        ((d.y * f + c.y * i) * f + (c.y * f + b.y * i) * i) * f + ((c.y * f + b.y * i) * f + (b.y * f + a.y * i) * i) * i
    };
}

static v3 V3Spline(float f, v3 a, v3 b, v3 c, v3 d)
{
	float i = 1.0f - f;

    return (v3) {
        ((d.x * f + c.x * i) * f + (c.x * f + b.x * i) * i) * f + ((c.x * f + b.x * i) * f + (b.x * f + a.x * i) * i) * i,
        ((d.y * f + c.y * i) * f + (c.y * f + b.y * i) * i) * f + ((c.y * f + b.y * i) * f + (b.y * f + a.y * i) * i) * i,
        ((d.z * f + c.z * i) * f + (c.z * f + b.z * i) * i) * f + ((c.z * f + b.z * i) * f + (b.z * f + a.z * i) * i) * i
    };
}

static v4 V4Spline(float f, v4 a, v4 b, v4 c, v4 d)
{
	float i = 1.0f - f;

    return (v4) {
        ((d.x * f + c.x * i) * f + (c.x * f + b.x * i) * i) * f + ((c.x * f + b.x * i) * f + (b.x * f + a.x * i) * i) * i,
        ((d.y * f + c.y * i) * f + (c.y * f + b.y * i) * i) * f + ((c.y * f + b.y * i) * f + (b.y * f + a.y * i) * i) * i,
        ((d.z * f + c.z * i) * f + (c.z * f + b.z * i) * i) * f + ((c.z * f + b.z * i) * f + (b.z * f + a.z * i) * i) * i,
        ((d.w * f + c.w * i) * f + (c.w * f + b.w * i) * i) * f + ((c.w * f + b.w * i) * f + (b.w * f + a.w * i) * i) * i
    };
}

static v2 V2Flatten(v2 pos, v2 normal)
{
	float f = pos.x * normal.x + pos.y * normal.y;

    return (v2) {
        pos.x - normal.x * f,
        pos.y - normal.y * f
    };
}

static v3 V3Flatten(v3 pos, v3 normal)
{
	float f = pos.x * normal.x + pos.y * normal.y + pos.z * normal.z;

    return (v3) {
        pos.x - normal.x * f,
        pos.y - normal.y * f,
        pos.z - normal.z * f
    };
}

static v4 V4Cross(v4 a, v4 b)
{
    return (v4) {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
        1.f
    };
}

static v2 V2LineCenter(v2 a, v2 b)
{
    return (v2) {
        0.5f * (a.x + b.x),
        0.5f * (a.y + b.y)
    };
}

static v2 V2LinesIntersectPoint(v2 a0, v2 a1, v2 b0, v2 b1)
{
    v2 r;

	r.x  = (a0.x * a1.y  -  a0.y * a1.x) * (b0.x - b1.x) - (b0.x * b1.y - b0.y * b1.x) * (a0.x - a1.x);
	r.x /= (a0.x - a1.x) * (b0.y - b1.y) - (a0.y - a1.y) * (b0.x - b1.x);
	r.y  = (a0.x * a1.y  -  a0.y * a1.x) * (b0.y - b1.y) - (b0.x * b1.y - b0.y * b1.x) * (a0.y - a1.y);
	r.y /= (a0.x - a1.x) * (b0.y - b1.y) - (a0.y - a1.y) * (b0.x - b1.x);

    return r;
}

static bool V2LinesContainPoint(v2 point, v2 a0, v2 a1, v2 b0, v2 b1)
{
    v2 c0 = { 0.5f * (a0.x + a1.x), 0.5f * (a0.y + a1.y) };
    v2 c1 = { 0.5f * (b0.x + b1.x), 0.5f * (b0.y + b1.y) };

    float r0 = 0.5f * V2Dist(a0, a1);
    float r1 = 0.5f * V2Dist(b0, b1);

    return V2DistSq(point, c0) < (r0 * r0) && V2DistSq(point, c1) < (r1 * r1);
}

static bool V2LinesIntersectTest(v2 a0, v2 a1, v2 b0, v2 b1)
{
    v2 intersect = V2LinesIntersectPoint(a0, a1, b0, b1);

    return V2LinesContainPoint(intersect, a0, a1, b0, b1);
}

// ---------------------------------------- matrix 2D --------------------------------------- //

static m2 M2Identity()
{
    return (m2) {
        1, 0,
        0, 1
    };
}

static m2 M2Mul(m2 a, m2 b)
{
    return (m2) {
        a.x.x * b.x.x + a.y.x * b.x.y,
        a.x.y * b.x.x + a.y.y * b.x.y,
        a.x.x * b.y.x + a.y.x * b.y.y,
        a.x.y * b.y.x + a.y.y * b.y.y,
    };
}

static v2 M2Mulv(m2 R, v2 v)
{
    return (v2) {
        R.x.x * v.x + R.y.x * v.y,
        R.x.y * v.x + R.y.y * v.y
    };
}

static m2 M2Rotate(float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);

    return (m2) { c, s, -s, c };
}

static m2 M2Scale(float sx, float sy)
{
    return (m2) { sx, 0.0f, 0.0f, sy };
}

// ---------------------------------------- matrix 3D --------------------------------------- //

static v3 M3Mulv(m3 M, v3 v)
{
    return (v3) {
        M.x.x * v.x + M.y.x * v.y + M.z.x * v.z,
        M.x.y * v.x + M.y.y * v.y + M.z.y * v.z,
        M.x.z * v.x + M.y.z * v.y + M.z.z * v.z
    };
}


// Rodrigues rotation formula
static m3 M3Rotate(v3 axis, float angle)
{
    float cosv  = cosf(angle);
    v3 sa       = V3Scale(axis, sinf(angle));
    v3 omca     = V3Scale(axis, (1.0f - cosv));

    return (m3) {
        omca.x * axis.x + cosv, omca.x * axis.y - sa.z, omca.x * axis.z + sa.y,
        omca.y * axis.x + sa.z, omca.y * axis.y + cosv, omca.y * axis.z - sa.x,
        omca.z * axis.x - sa.y, omca.z * axis.y + sa.x, omca.z * axis.z + cosv
    };
}

// ---------------------------------------- matrix 4D --------------------------------------- //

static m4 M4Identity()
{
    return (m4) {
        1.0f,   0.0f,   0.0f,   0.0f,
        0.0f,   1.0f,   0.0f,   0.0f,
        0.0f,   0.0f,   1.0f,   0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    };
}

static m4 M4Transpose(m4 N)
{
    return (m4) {
        N.x.x, N.x.y, N.x.z, N.x.w,
        N.y.x, N.y.y, N.y.z, N.y.w,
        N.z.x, N.z.y, N.z.z, N.z.w,
        N.w.x, N.w.y, N.w.z, N.w.w
    };
}

static m4 M4Translate(float x, float y, float z)
{
    return (m4) {
        1.0f,   0.0f,   0.0f,   0.0f,
        0.0f,   1.0f,   0.0f,   0.0f,
        0.0f,   0.0f,   1.0f,   0.0f,
        x,      y,      z,      1.0f
    };
}

static m4 M4Scale(float x, float y, float z)
{
    return (m4) {
        x,      0.0f,   0.0f,   0.0f,
        0.0f,   y,      0.0f,   0.0f,
        0.0f,   0.0f,   z,      0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    };
}

static m4 M4Mul(m4 a, m4 b)
{
    return (m4) {
        a.x.x * b.x.x + a.y.x * b.x.y + a.z.x * b.x.z + a.w.x * b.x.w,
        a.x.y * b.x.x + a.y.y * b.x.y + a.z.y * b.x.z + a.w.y * b.x.w,
        a.x.z * b.x.x + a.y.z * b.x.y + a.z.z * b.x.z + a.w.z * b.x.w,
        a.x.w * b.x.x + a.y.w * b.x.y + a.z.w * b.x.z + a.w.w * b.x.w,

        a.x.x * b.y.x + a.y.x * b.y.y + a.z.x * b.y.z + a.w.x * b.y.w,
        a.x.y * b.y.x + a.y.y * b.y.y + a.z.y * b.y.z + a.w.y * b.y.w,
        a.x.z * b.y.x + a.y.z * b.y.y + a.z.z * b.y.z + a.w.z * b.y.w,
        a.x.w * b.y.x + a.y.w * b.y.y + a.z.w * b.y.z + a.w.w * b.y.w,

        a.x.x * b.z.x + a.y.x * b.z.y + a.z.x * b.z.z + a.w.x * b.z.w,
        a.x.y * b.z.x + a.y.y * b.z.y + a.z.y * b.z.z + a.w.y * b.z.w,
        a.x.z * b.z.x + a.y.z * b.z.y + a.z.z * b.z.z + a.w.z * b.z.w,
        a.x.w * b.z.x + a.y.w * b.z.y + a.z.w * b.z.z + a.w.w * b.z.w,

        a.x.x * b.w.x + a.y.x * b.w.y + a.z.x * b.w.z + a.w.x * b.w.w,
        a.x.y * b.w.x + a.y.y * b.w.y + a.z.y * b.w.z + a.w.y * b.w.w,
        a.x.z * b.w.x + a.y.z * b.w.y + a.z.z * b.w.z + a.w.z * b.w.w,
        a.x.w * b.w.x + a.y.w * b.w.y + a.z.w * b.w.z + a.w.w * b.w.w
    };
}

static v4 M4Mulv(m4 M, v4 v)
{
    return (v4) {
        M.x.x * v.x + M.y.x * v.y + M.z.x * v.z + M.w.x * v.w,
        M.x.y * v.x + M.y.y * v.y + M.z.y * v.z + M.w.y * v.w,
        M.x.z * v.x + M.y.z * v.y + M.z.z * v.z + M.w.z * v.w,
        M.x.w * v.x + M.y.w * v.y + M.z.w * v.z + M.w.w * v.w
    };
}

static m4 M4RotateX(float angle)
{
	float s = sinf(angle);
	float c = cosf(angle);

    return (m4) {
        1.0f,   0.0f,   0.0f,   0.0f,
        0.0f,   c,      s,      0.0f,
        0.0f,   -s,     c,      0.0f,
        0.0f,   0.0f,   0.0f,   1.0f,
    };
}

static m4 M4RotateY(float angle)
{
	float s = sinf(angle);
	float c = cosf(angle);

    return (m4) {
        c,      0.0f,   s,      0.0f,
        0.0f,   1.0f,   0.0f,   0.0f,
        -s,     0.0f,   c,      0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    };
}

static m4 M4RotateZ(float angle)
{
	float s = sinf(angle);
	float c = cosf(angle);

    return (m4) {
        c,      s,      0.0f,   0.0f,
        -s,     c,      0.0f,   0.0f,
        0.0f,   0.0f,   1.0f,   0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    };
}

// Rodrigues rotation formula
static m4 M4Rotate(v3 axis, float angle)
{
    float cosv      = cosf(angle);
    float sinv      = sinf(angle);
    float inv_cosv  = 1.0f - cosv;

    v3 sa   = V3Scale(axis, sinv);
    v3 omca = V3Scale(axis, inv_cosv);

    return (m4) {
        omca.x * axis.x + cosv,
        omca.x * axis.y - sa.x,
        omca.x * axis.z + sa.y,
        0.0f,

        omca.y * axis.x + sa.z,
        omca.y * axis.y + cosv,
        omca.y * axis.z - sa.z,
        0.0f,

        omca.z * axis.x - sa.y,
        omca.z * axis.y + sa.x,
        omca.z * axis.z + cosv,
        0.0f,

        0.0f,
        0.0f,
        0.0f,
        1.0f
    };
}

static m4 M4Frustum(float l, float r, float b, float t, float n, float f)
{
    return (m4) {
        2.0f * n / (r - l),     0.0f,                   0.0f,                       0.0f,
        0.0f,                   2.0f * n / (t - b),     0.0f,                       0.0f,
        (r + l) / (r - l),      (t + b) / (t - b),      -(f + n) / (f - n),         -1.0f,
        0.0f,                   0.0f,                   -2.0f * (f * n) / (f - n),  0.0f
    };
}

static m4 M4Ortho(float l, float r, float b, float t, float n, float f)
{
    return (m4) {
        2.0f / (r - l),     0.0f,               0.0f,               0.0f,
        0.0f,               2.0f / (t - b),     0.0f,               0.0f,
        0.0f,               0.0f,               -2.0f / (f - n),    0.0f,
        -(r + l) / (r - l), -(t + b) / (t - b), -(f + n) / (f - n), 1.0f
    };
}

static m4 M4Perspective(float y_fov, float aspect, float n, float f)
{
	const float a = 1.0f / tanf(y_fov / 2.0f);

    return (m4) {
        a / aspect,     0.0f,       0.0f,                           0.0f,
        0.0f,           a,          0.0f,                           0.0f,
        0.0f,           0.0f,       -((f + n) / (f - n)),           -1.0f,
        0.0f,           0.0f,       -((2.0f * f * n) / (f - n)),    0.0f
    };
}

static m4 M4LookAt(v3 eye, v3 center, v3 up)
{
    v3 f = V3Norm(V3Sub(center, eye));
	v3 s = V3Norm(V3Cross(f, up));
    v3 t = V3Cross(s, f);

    m4 M;

	M.x.x =  s.x;
	M.x.y =  t.x;
	M.x.z = -f.x;
	M.x.w =   0.0f;

	M.y.x =  s.y;
	M.y.y =  t.y;
	M.y.z = -f.y;
	M.y.w =  0.0f;

	M.z.x =  s.z;
	M.z.y =  t.z;
	M.z.z = -f.z;
	M.z.w =   0.0f;

    M.w.x = -(M.x.x * eye.x + M.y.x * eye.y + M.z.x * eye.z);
    M.w.y = -(M.x.y * eye.x + M.y.y * eye.y + M.z.y * eye.z);
    M.w.z = -(M.x.z * eye.x + M.y.z * eye.y + M.z.z * eye.z);
    M.w.w = -(M.x.w * eye.x + M.y.w * eye.y + M.z.w * eye.z - 1.0f);

    return M;
}

static m4 M4TranslateScale(float px, float py, float pz, float sx, float sy, float sz)
{
    return (m4) {
        sx,     0.0f,   0.0f,   0.0f,
        0.0f,   sy,     0.0f,   0.0f,
        0.0f,   0.0f,   sz,     0.0f,
        px,     py,     pz,     1.0f,
    };
}

static m4 M4TranslateScaleRotateZ(float px, float py, float pz, float sx, float sy, float sz, float angle)
{
    float s = sinf(angle);
    float c = cosf(angle);

    return (m4) {
        c * sx,     s * sx,     0.0f,   0.0f,
       -s * sy,     c * sy,     0.0f,   0.0f,
        0.0f,       0.0f,       sz,     0.0f,
        px,         py,         pz,     1.0f
    };
}

