#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <float.h>

#define PI_CONSTANT   (3.141592654f)
#define TO_RAD        ((PI_CONSTANT / 180.0f))

// ============================================================================ //
//
//                                  FLOAT MATH
//
// =========================================================================== //
static const f32 f3z[3];
static const f32 m3z[9];
static const f32 m3id[9] = {1,0,0, 0,1,0, 0,0,1};

#define fop(r,e,a,p,b,i,s) (r) e ((a) p (b)) i (s)
#define f3op(r,e,a,p,b,i,s) \
{ fop((r)[0],e,(a)[0],p,(b)[0],i,s),\
  fop((r)[1],e,(a)[1],p,(b)[1],i,s),\
  fop((r)[2],e,(a)[2],p,(b)[2],i,s); }
#define f3set(v,x,y,z) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define f3zero(v) f3set(v,0,0,0)
#define f3unpack(v) (v)[0],(v)[1],(v)[2]
#define f3cpy(d,s) (d)[0]=(s)[0],(d)[1]=(s)[1],(d)[2]=(s)[2]
#define f3add(d,a,b) f3op(d,=,a,+,b,+,0)
#define f3sub(d,a,b) f3op(d,=,a,-,b,+,0)
#define f3mul(d,a,s) f3op(d,=,a,+,f3z,*,s)
#define f3dot(a,b) ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define f3lerp(r,a,t,b) f3op(r,=,a,+,f3z,*,(1.0f - (t))); f3op(r,+=,b,+,f3z,*,t)
#define m3cpy(d,s) memcpy(d,s,9 * sizeof(f32))

static void f3cross(f32 *r, const f32 *a, const f32 *b) {
  r[0] = (a[1]*b[2]) - (a[2]*b[1]);
  r[1] = (a[2]*b[0]) - (a[0]*b[2]);
  r[2] = (a[0]*b[1]) - (a[1]*b[0]);
}

static f32 f3box(const f32 *a, const f32 *b, const f32 *c) {
  f32 n[3];
  f3cross(n, a, b);
  return f3dot(n, c);
}

static void f3norm(f32 *v) {
  f32 il, l = f3dot(v, v);
  if (l == 0.0f) return;
  il = 1 / sqrtf(l);
  f3mul(v,v,il);
}

static void m3mul(f32 *r, const f32 *a, const f32 *b) {
  int i = 0;
#define A(col, row) (a)[(col*3)+row]
#define B(col, row) (b)[(col*3)+row]
#define P(col, row) (r)[(col*3)+row]
  for (i = 0; i < 3; ++i)
  {
    const f32 ai0 = A(i,0), ai1 = A(i,1), ai2 = A(i,2);
    P(i,0) = ai0 * B(0,0) + ai1 * B(1,0) + ai2 * B(2,0);
    P(i,1) = ai0 * B(0,1) + ai1 * B(1,1) + ai2 * B(2,1);
    P(i,2) = ai0 * B(0,2) + ai1 * B(1,2) + ai2 * B(2,2);
  }
#undef A
#undef B
#undef P
}

static void transform(f32 *r, const f32 *v, const f32 *r33, const f32 *t3) {
  for (int i = 0; i < 3; ++i) {
    r[i]  = v[i] * r33[i*3+0];
    r[i] += v[i] * r33[i*3+1];
    r[i] += v[i] * r33[i*3+2];
    r[i] += t3[i];
  }
}

static void transformS(f32 *v, const f32 *r33, const f32 *t3) {
  f32 tmp[3]; f3cpy(tmp, v);
  transform(v, tmp, r33, t3);
}

static void transformT(f32 *r, const f32 *v, const f32 *r33, const f32 *t3) {
  for (int i = 0; i < 3; ++i) {
    f32 p = v[i] - t3[i];
    r[i]  = p * r33[0*3+i];
    r[i] += p * r33[1*3+i];
    r[i] += p * r33[2*3+i];
  }
}

static void transformST(f32 *v, const f32 *r33, const f32 *t3) {
  f32 tmp[3]; f3cpy(tmp, v);
  transformT(v, tmp, r33, t3);
}

static void m3rot(f32 *r, f32 angle, f32 X, f32 Y, f32 Z) {
#define M(col, row) m[(col*3)+row]
  f32 s = (f32)sinf(angle);
  f32 c = (f32)cosf(angle);
  f32 oc = 1.0f - c;
  f32 m[16];

  M(0,0) = oc * X * X + c;
  M(0,1) = oc * X * Y - Z * s;
  M(0,2) = oc * Z * X + Y * s;

  M(1,0) = oc * X * Y + Z * s;
  M(1,1) = oc * Y * Y + c;
  M(1,2) = oc * Y * Z - X * s;

  M(2,0) = oc * Z * X - Y * s;
  M(2,1) = oc * Y * Z + X * s;
  M(2,2) = oc * Z * Z + c;
  m3mul(r, r, m);
}

static void m3mulv(f32 *r, const f32 *m, const f32 *v) {
#define X(a) (r)[0]
#define Y(a) (r)[1]
#define Z(a) (r)[2]
#define M(col, row) m[(col*3)+row]
  f32 o[3]; f3cpy(o,v);

  X(r) = M(0,0)*X(o) + M(0,1)*Y(o) + M(0,2)*Z(o);
  Y(r) = M(1,0)*X(o) + M(1,1)*Y(o) + M(1,2)*Z(o);
  Z(r) = M(2,0)*X(o) + M(2,1)*Y(o) + M(2,2)*Z(o);
#undef X
#undef Y
#undef Z
#undef M
}

// ============================================================================ //
//
//                                  GJK
//
// =========================================================================== //

#define GJK_MAX_ITERATIONS 20

typedef struct {
  int aid, bid;
  f32 a[3];
  f32 b[3];
} gjk_support;

typedef struct {
  f32 a[3];
  f32 b[3];
  f32 p[3];
  int aid, bid;
} gjk_vertex;

typedef struct {
  int hit;
  int iter, max_iter;
  int vcnt, scnt;
  int saveA[5], saveB[5];
  gjk_vertex v[5];
  f32 bc[4], D;
} gjk_simplex;

typedef struct {
  int hit;
  f32 p0[3];
  f32 p1[3];
  f32 distance_squared;
  int iterations;
} gjk_result;

static int gjk(gjk_simplex *s, const gjk_support *sup, f32 *dv) {
  assert(s);
  assert(dv);
  assert(sup);
  if (!s || !sup || !dv) return 0;
  if (s->max_iter > 0 && s->iter >= s->max_iter)
    return 0;

  // I.) Initialize
  if (!s->vcnt) {
    s->scnt = 0;
    s->D = FLT_MAX;
    s->max_iter = !s->max_iter ? GJK_MAX_ITERATIONS: s->max_iter;
  }
  // II.) Check for duplications
  for (int i = 0; i < s->scnt; ++i) {
    if (sup->aid != s->saveA[i]) continue;
    if (sup->bid != s->saveB[i]) continue;
    return 0;
  }
  // III.) Add vertex into simplex
  gjk_vertex *vert = &s->v[s->vcnt];
  f3cpy(vert->a, sup->a);
  f3cpy(vert->b, sup->b);
  f3cpy(vert->p, dv);
  vert->aid = sup->aid;
  vert->bid = sup->bid;
  s->bc[s->vcnt++] = 1.0f;

  // IV.) Find closest simplex point
  switch (s->vcnt) {
    case 1: break;
    case 2: {
      // -------------------- Line ----------------------- //
      f32 a[3], b[3];
      f3cpy(a, s->v[0].p);
      f3cpy(b, s->v[1].p);

      // compute barycentric coordinates
      f32 ab[3], ba[3];
      f3sub(ab, a, b);
      f3sub(ba, b, a);

      f32 u = f3dot(b, ba);
      f32 v = f3dot(a, ab);
      if (v <= 0.0f) {
        // region A
        s->bc[0] = 1.0f;
        s->vcnt = 1;
        break;
      }
      if (u <= 0.0f) {
        // region B
        s->v[0] = s->v[1];
        s->bc[0] = 1.0f;
        s->vcnt = 1;
        break;
      }
      // region AB
      s->bc[0] = u;
      s->bc[1] = v;
      s->vcnt = 2;
    } break;
    case 3: {
      // -------------------- Triangle ----------------------- //
      f32 a[3], b[3], c[3];
      f3cpy(a, s->v[0].p);
      f3cpy(b, s->v[1].p);
      f3cpy(c, s->v[2].p);

      f32 ab[3], ba[3], bc[3], cb[3], ca[3], ac[3];
      f3sub(ab, a, b);
      f3sub(ba, b, a);
      f3sub(bc, b, c);
      f3sub(cb, c, b);
      f3sub(ca, c, a);
      f3sub(ac, a, c);

      // compute barycentric coordinates
      f32 u_ab = f3dot(b, ba);
      f32 v_ab = f3dot(a, ab);

      f32 u_bc = f3dot(c, cb);
      f32 v_bc = f3dot(b, bc);

      f32 u_ca = f3dot(a, ac);
      f32 v_ca = f3dot(c, ca);

      if (v_ab <= 0.0f && u_ca <= 0.0f) {
        // region A
        s->bc[0] = 1.0f;
        s->vcnt = 1;
        break;
      }
      if (u_ab <= 0.0f && v_bc <= 0.0f) {
        // region B
        s->v[0] = s->v[1];
        s->bc[0] = 1.0f;
        s->vcnt = 1;
        break;
      }
      if (u_bc <= 0.0f && v_ca <= 0.0f) {
        // region C
        s->v[0] = s->v[2];
        s->bc[0] = 1.0f;
        s->vcnt = 1;
        break;
      }
      // calculate fractional area
      f32 n[3], n1[3], n2[3], n3[3];
      f3cross(n, ba, ca);
      f3cross(n1, b, c);
      f3cross(n2, c, a);
      f3cross(n3, a, b);

      f32 u_abc = f3dot(n1, n);
      f32 v_abc = f3dot(n2, n);
      f32 w_abc = f3dot(n3, n);

      if (u_ab > 0.0f && v_ab > 0.0f && w_abc <= 0.0f) {
        // region AB
        s->bc[0] = u_ab;
        s->bc[1] = v_ab;
        s->vcnt = 2;
        break;
      }
      if (u_bc > 0.0f && v_bc > 0.0f && u_abc <= 0.0f) {
        // region BC
        s->v[0] = s->v[1];
        s->v[1] = s->v[2];
        s->bc[0] = u_bc;
        s->bc[1] = v_bc;
        s->vcnt = 2;
        break;
      }
      if (u_ca > 0.0f && v_ca > 0.0f && v_abc <= 0.0f) {
        // region CA
        s->v[1] = s->v[0];
        s->v[0] = s->v[2];
        s->bc[0] = u_ca;
        s->bc[1] = v_ca;
        s->vcnt = 2;
        break;
      }
      // region ABC
      assert(u_abc > 0.0f && v_abc > 0.0f && w_abc > 0.0f);
      s->bc[0] = u_abc;
      s->bc[1] = v_abc;
      s->bc[2] = w_abc;
      s->vcnt = 3;
    } break;
    case 4: {
      // -------------------- Tetrahedron ----------------------- //
      f32 a[3], b[3], c[3], d[3];
      f3cpy(a, s->v[0].p);
      f3cpy(b, s->v[1].p);
      f3cpy(c, s->v[2].p);
      f3cpy(d, s->v[3].p);

      f32 ab[3], ba[3], bc[3], cb[3], ca[3], ac[3];
      f32 db[3], bd[3], dc[3], cd[3], ad[3], da[3];

      f3sub(ab, a, b);
      f3sub(ba, b, a);
      f3sub(bc, b, c);
      f3sub(cb, c, b);
      f3sub(ca, c, a);
      f3sub(ac, a, c);

      f3sub(db, d, b);
      f3sub(bd, b, d);
      f3sub(dc, d, c);
      f3sub(cd, c, d);
      f3sub(da, d, a);
      f3sub(ad, a, d);

      // compute barycentric coordinates
      f32 u_ab = f3dot(b, ba);
      f32 v_ab = f3dot(a, ab);

      f32 u_bc = f3dot(c, cb);
      f32 v_bc = f3dot(b, bc);

      f32 u_ca = f3dot(a, ac);
      f32 v_ca = f3dot(c, ca);

      f32 u_bd = f3dot(d, db);
      f32 v_bd = f3dot(b, bd);

      f32 u_dc = f3dot(c, cd);
      f32 v_dc = f3dot(d, dc);

      f32 u_ad = f3dot(d, da);
      f32 v_ad = f3dot(a, ad);

      // check verticies for closest point
      if (v_ab <= 0.0f && u_ca <= 0.0f && v_ad <= 0.0f) {
        // region A
        s->bc[0] = 1.0f;
        s->vcnt = 1;
        break;
      }
      if (u_ab <= 0.0f && v_bc <= 0.0f && v_bd <= 0.0f) {
        // region B
        s->v[0] = s->v[1];
        s->bc[0] = 1.0f;
        s->vcnt = 1;
        break;
      }
      if (u_bc <= 0.0f && v_ca <= 0.0f && u_dc <= 0.0f) {
        // region C
        s->v[0] = s->v[2];
        s->bc[0] = 1.0f;
        s->vcnt = 1;
        break;
      }
      if (u_bd <= 0.0f && v_dc <= 0.0f && u_ad <= 0.0f) {
        // region D
        s->v[0] = s->v[3];
        s->bc[0] = 1.0f;
        s->vcnt = 1;
        break;
      }
      // calculate fractional area
      f32 n[3], n1[3], n2[3], n3[3];
      f3cross(n, da, ba);
      f3cross(n1, d, b);
      f3cross(n2, b, a);
      f3cross(n3, a, d);

      f32 u_adb = f3dot(n1, n);
      f32 v_adb = f3dot(n2, n);
      f32 w_adb = f3dot(n3, n);

      f3cross(n, ca, da);
      f3cross(n1, c, d);
      f3cross(n2, d, a);
      f3cross(n3, a, c);

      f32 u_acd = f3dot(n1, n);
      f32 v_acd = f3dot(n2, n);
      f32 w_acd = f3dot(n3, n);

      f3cross(n, bc, dc);
      f3cross(n1, b, d);
      f3cross(n2, d, c);
      f3cross(n3, c, b);

      f32 u_cbd = f3dot(n1, n);
      f32 v_cbd = f3dot(n2, n);
      f32 w_cbd = f3dot(n3, n);

      f3cross(n, ba, ca);
      f3cross(n1, b, c);
      f3cross(n2, c, a);
      f3cross(n3, a, b);

      f32 u_abc = f3dot(n1, n);
      f32 v_abc = f3dot(n2, n);
      f32 w_abc = f3dot(n3, n);

      // check edges for closest point
      if (w_abc <= 0.0f && v_adb <= 0.0f && u_ab > 0.0f && v_ab > 0.0f) {
        // region AB
        s->bc[0] = u_ab;
        s->bc[1] = v_ab;
        s->vcnt = 2;
        break;
      }
      if (u_abc <= 0.0f && w_cbd <= 0.0f && u_bc > 0.0f && v_bc > 0.0f) {
        // region BC
        s->v[0] = s->v[1];
        s->v[1] = s->v[2];
        s->bc[0] = u_bc;
        s->bc[1] = v_bc;
        s->vcnt = 2;
        break;
      }
      if (v_abc <= 0.0f && w_acd <= 0.0f && u_ca > 0.0f && v_ca > 0.0f) {
        // region CA
        s->v[1] = s->v[0];
        s->v[0] = s->v[2];
        s->bc[0] = u_ca;
        s->bc[1] = v_ca;
        s->vcnt = 2;
        break;
      }
      if (v_cbd <= 0.0f && u_acd <= 0.0f && u_dc > 0.0f && v_dc > 0.0f) {
        // region DC
        s->v[0] = s->v[3];
        s->v[1] = s->v[2];
        s->bc[0] = u_dc;
        s->bc[1] = v_dc;
        s->vcnt = 2;
        break;
      }
      if (v_acd <= 0.0f && w_adb <= 0.0f && u_ad > 0.0f && v_ad > 0.0f) {
        // region AD
        s->v[1] = s->v[3];
        s->bc[0] = u_ad;
        s->bc[1] = v_ad;
        s->vcnt = 2;
        break;
      }
      if (u_cbd <= 0.0f && u_adb <= 0.0f && u_bd > 0.0f && v_bd > 0.0f) {
        // region BD
        s->v[0] = s->v[1];
        s->v[1] = s->v[3];
        s->bc[0] = u_bd;
        s->bc[1] = v_bd;
        s->vcnt = 2;
        break;
      }
      // calculate fractional volume (volume can be negative!)
      f32 denom = f3box(cb, ab, db);
      f32 volume = (denom == 0) ? 1.0f: 1.0f/denom;
      f32 u_abcd = f3box(c, d, b) * volume;
      f32 v_abcd = f3box(c, a, d) * volume;
      f32 w_abcd = f3box(d, a, b) * volume;
      f32 x_abcd = f3box(b, a, c) * volume;

      // check faces for closest point
      if (x_abcd <= 0.0f && u_abc > 0.0f && v_abc > 0.0f && w_abc > 0.0f) {
        // region ABC
        s->bc[0] = u_abc;
        s->bc[1] = v_abc;
        s->bc[2] = w_abc;
        s->vcnt = 3;
        break;
      }
      if (u_abcd <= 0.0f && u_cbd > 0.0f && v_cbd > 0.0f && w_cbd > 0.0f) {
        // region CBD
        s->v[0] = s->v[2];
        s->v[2] = s->v[3];
        s->bc[0] = u_cbd;
        s->bc[1] = v_cbd;
        s->bc[2] = w_cbd;
        s->vcnt = 3;
        break;
      }
      if (v_abcd <= 0.0f && u_acd > 0.0f && v_acd > 0.0f && w_acd > 0.0f) {
        // region ACD
        s->v[1] = s->v[2];
        s->v[2] = s->v[3];
        s->bc[0] = u_acd;
        s->bc[1] = v_acd;
        s->bc[2] = w_acd;
        s->vcnt = 3;
        break;
      }
      if (w_abcd <= 0.0f && u_adb > 0.0f && v_adb > 0.0f && w_adb > 0.0f) {
        // region ADB
        s->v[2] = s->v[1];
        s->v[1] = s->v[3];
        s->bc[0] = u_adb;
        s->bc[1] = v_adb;
        s->bc[2] = w_adb;
        s->vcnt = 3;
        break;
      }
      // region ABCD
      assert(u_abcd > 0.0f && v_abcd > 0.0f && w_abcd > 0.0f && x_abcd > 0.0f);
      s->bc[0] = u_abcd;
      s->bc[1] = v_abcd;
      s->bc[2] = w_abcd;
      s->bc[3] = x_abcd;
      s->vcnt = 4;
    } break;}

  // V.) Check if origin is enclosed by tetrahedron
  if (s->vcnt == 4) {
    s->hit = 1;
    return 0;
  }
  // VI.) Ensure closing in on origin to prevent multi-step cycling
  {f32 pnt[3], denom = 0;
    for (int i = 0; i < s->vcnt; ++i)
      denom += s->bc[i];
    denom = 1.0f / denom;

    switch (s->vcnt) {
      case 1: f3cpy(pnt, s->v[0].p); break;
      case 2: {
        // --------- Line --------
        f32 a[3], b[3];
        f3mul(a, s->v[0].p, denom * s->bc[0]);
        f3mul(b, s->v[1].p, denom * s->bc[1]);
        f3add(pnt, a, b);
      } break;
      case 3: {
        // ------- Triangle ------
        f32 a[3], b[3], c[3];
        f3mul(a, s->v[0].p, denom * s->bc[0]);
        f3mul(b, s->v[1].p, denom * s->bc[1]);
        f3mul(c, s->v[2].p, denom * s->bc[2]);

        f3add(pnt, a, b);
        f3add(pnt, pnt, c);
      } break;
      case 4: {
        // ----- Tetrahedron -----
        f32 a[3], b[3], c[3], d[3];
        f3mul(a, s->v[0].p, denom * s->bc[0]);
        f3mul(b, s->v[1].p, denom * s->bc[1]);
        f3mul(c, s->v[2].p, denom * s->bc[2]);
        f3mul(d, s->v[3].p, denom * s->bc[3]);

        f3add(pnt, a, b);
        f3add(pnt, pnt, c);
        f3add(pnt, pnt, d);
      } break;}

    f32 d2 = f3dot(pnt, pnt);
    if (d2 >= s->D) return 0;
    s->D = d2;}

  // VII.) New search direction
  switch (s->vcnt) {
    default: assert(0); break;
    case 1: {
      // --------- Point --------
      f3mul(dv, s->v[0].p, -1);
    } break;
    case 2: {
      // ------ Line segment ----
      f32 b0[3], ba[3], t[3];
      f3sub(ba, s->v[1].p, s->v[0].p);
      f3mul(b0, s->v[1].p, -1);
      f3cross(t, ba, b0);
      f3cross(dv, t, ba);
    } break;
    case 3: {
      // ------- Triangle -------
      f32 n[3], ab[3], ac[3];
      f3sub(ab, s->v[1].p, s->v[0].p);
      f3sub(ac, s->v[2].p, s->v[0].p);
      f3cross(n, ab, ac);
      if (f3dot(n, s->v[0].p) <= 0.0f)
        f3cpy(dv, n);
      else f3mul(dv, n, -1);
    }}
  if (f3dot(dv,dv) < FLT_EPSILON * FLT_EPSILON)
    return 0;

  // VIII.) Save ids for next duplicate check
  s->scnt = s->vcnt; s->iter++;
  for (int i = 0; i < s->scnt; ++i) {
    s->saveA[i] = s->v[i].aid;
    s->saveB[i] = s->v[i].bid;
  } return 1;
}

static gjk_result gjk_analyze(const gjk_simplex *s) {
  gjk_result r = {0};
  r.iterations = s->iter;
  r.hit = s->hit;

  // calculate normalization denominator
  f32 denom = 0;
  for (int i = 0; i < s->vcnt; ++i)
    denom += s->bc[i];
  denom = 1.0f / denom;

  // compute closest points
  switch (s->vcnt) {
    default: assert(0); break;
    case 1: {
      // Point
      f3cpy(r.p0, s->v[0].a);
      f3cpy(r.p1, s->v[0].b);
    } break;
    case 2: {
      // Line
      f32 as = denom * s->bc[0];
      f32 bs = denom * s->bc[1];

      f32 a[3], b[3], c[3], d[3];
      f3mul(a, s->v[0].a, as);
      f3mul(b, s->v[1].a, bs);
      f3mul(c, s->v[0].b, as);
      f3mul(d, s->v[1].b, bs);

      f3add(r.p0, a, b);
      f3add(r.p1, c, d);
    } break;
    case 3: {
      // Triangle
      f32 as = denom * s->bc[0];
      f32 bs = denom * s->bc[1];
      f32 cs = denom * s->bc[2];

      f32 a[3], b[3], c[3];
      f3mul(a, s->v[0].a, as);
      f3mul(b, s->v[1].a, bs);
      f3mul(c, s->v[2].a, cs);

      f32 d[3], e[3], f[3];
      f3mul(d, s->v[0].b, as);
      f3mul(e, s->v[1].b, bs);
      f3mul(f, s->v[2].b, cs);

      f3add(r.p0, a, b);
      f3add(r.p0, r.p0, c);

      f3add(r.p1, d, e);
      f3add(r.p1, r.p1, f);
    } break;
    case 4: {
      // Tetrahedron
      f32 as = denom * s->bc[0];
      f32 bs = denom * s->bc[1];
      f32 cs = denom * s->bc[2];
      f32 ds = denom * s->bc[3];

      f32 a[3], b[3], c[3], d[3];
      f3mul(a, s->v[0].a, as);
      f3mul(b, s->v[1].a, bs);
      f3mul(c, s->v[2].a, cs);
      f3mul(d, s->v[3].a, ds);

      f3add(r.p0, a, b);
      f3add(r.p0, r.p0, c);
      f3add(r.p0, r.p0, d);
      f3cpy(r.p1, r.p0);
    } break;}

  if (!r.hit) {
    // compute distance
    f32 d[3]; f3sub(d, r.p1, r.p0);
    r.distance_squared = f3dot(d, d);
  } else r.distance_squared = 0;
  return r;
}

// ============================================================================ //
//
//                                  COLLISION
//
// =========================================================================== //
typedef struct {
  f32 depth;
  f32 contact_point[3];
  f32 normal[3];
} manifold;

// segment
static f32 segment_closest_point_to_point_sqdist(const f32 *a, const f32 *b, const f32 *p);
static void segment_closest_point_to_point(f32 *res, const f32 *a, const f32 *b, const f32 *p);
static f32 segment_closest_point_to_segment(f32 *t1, f32 *t2, f32 *c1, f32 *c2, const f32 *p1, const f32 *q1, const f32 *p2, const f32 *q2);
// plane
static void planeq(f32 *plane4, const f32 *n3, const f32 *pnt3);
static void planeqf(f32 *plane4, f32 nx, f32 ny, f32 nz, f32 px, f32 py, f32 pz);
// ray
static f32 ray_intersects_plane(const f32 *ro, const f32 *rd, const f32 *p4);
static f32 ray_intersects_triangle(const f32 *ro, const f32 *rd, const f32 *p1, const f32 *p2, const f32 *p3);
static int ray_intersects_sphere(f32 *t0, f32 *t1, const f32 *ro, const f32 *rd, const f32 *c, f32 r);
static int ray_intersects_aabb(f32 *t0, f32 *t1, const f32 *ro, const f32 *rd, const f32 *min, const f32 *max);
// sphere
static void sphere_closest_point_to_point(f32 *res, const f32 *c, const f32 r, const f32 *p);
static int sphere_intersects_sphere(const f32 *ca, f32 ra, const f32 *cb, f32 rb);
static int sphere_intersects_sphere_manifold(manifold *m, const f32 *ca, f32 ra, const f32 *cb, f32 rb);
static int sphere_intersect_aabb(const f32 *c, f32 r, const f32 *min, const f32 *max);
static int sphere_intersect_aabb_manifold(manifold *m, const f32 *c, f32 r, const f32 *min, const f32 *max);
static int sphere_intersect_capsule(const f32 *sc, f32 sr, const f32 *ca, const f32 *cb, f32 cr);
static int sphere_intersect_capsule_manifold(manifold *m, const f32 *sc, f32 sr, const f32 *ca, const f32 *cb, f32 cr);
static int sphere_intersecting_polyhedron(const f32 *c, f32 r, const f32 *verts, int cnt);
// aabb
static void aabb_rebalance_transform(f32 *bmin, f32 *bmax, const f32 *amin, const f32 *amax, const f32 *m, const f32 t[3]);
static void aabb_closest_point_to_point(f32 *res, const f32 *min, const f32 *max, const f32 *p);
static f32 aabb_sqdist_to_point(const f32 *min, const f32 *max, const f32 *p);
static int aabb_contains_point(const f32 *amin, const f32 *amax, const f32 *p);
static int aabb_intersect_aabb(const f32 *amin, const f32 *amax, const f32 *bmin, const f32 *bmax);
static int aabb_intersect_aabb_manifold(manifold *m, const f32 *amin, const f32 *amax, const f32 *bmin, const f32 *bmax);
static int aabb_intersect_sphere(const f32 *min, const f32 *max, const f32 *c, f32 r);
static int aabb_intersect_sphere_manifold(manifold *m, const f32 *min, const f32 *max, const f32 *c, f32 r);
static int aabb_intersect_capsule(const f32 *min, const f32 *max, const f32 *ca, const f32 *cb, f32 cr);
static int aabb_intersect_capsule_manifold(manifold *m, const f32 *min, const f32 *max, const f32 *ca, const f32 *cb, f32 cr);
static int aabb_intersect_polyhedron(const f32 *min, const f32 *max, const f32 *verts, int cnt);
// capsule
static f32 capsule_point_sqdist(const f32 *ca, const f32 *cb, f32 cr, const f32 *p);
static void capsule_closest_point_to_point(f32 *res, const f32 *ca, const f32 *cb, f32 cr, const f32 *p);
static int capsule_intersect_capsule(const f32 *aa, const f32 *ab, f32 ar, const f32 *ba, const f32 *bb, f32 br);
static int capsule_intersect_capsule_manifold(manifold *m, const f32 *aa, const f32 *ab, f32 ar, const f32 *ba, const f32 *bb, f32 br);
static int capsule_intersect_sphere(const f32 *ca, const f32 *cb, f32 cr, const f32 *sc, f32 sr);
static int capsule_intersect_sphere_manifold(manifold *m, const f32 *ca, const f32 *cb, f32 cr, const f32 *sc, f32 sr);
static int capsule_intersect_aabb(const f32 *ca, const f32 *cb, f32 cr, const f32 *min, const f32 *max);
static int capsule_intersect_aabb_manifold(manifold *m, const f32 *ca, const f32 *cb, f32 cr, const f32 *min, const f32 *max);
static int capsule_intersect_polyhedron(const f32 *ca, const f32 *cb, f32 cr, const f32 *verts, int cnt);
// polyhedron: query
static int polyhedron_is_intersecting_sphere(const f32 *verts, int cnt, const f32 *c, f32 r);
static int polyhedron_is_intersecting_aabb(const f32 *verts, int cnt, const f32 *min, const f32 *max);
static int polyhedron_is_intersecting_capsule(const f32 *verts, int cnt, const f32 *ca, const f32 *cb, f32 cr);
static int polyhedron_is_intersecting_polyhedron(const f32 *averts, int acnt, const f32 *bverts, int bcnt);
// polyhedron: query transformed
static int polyhedron_is_intersecting_sphere_transform(const f32 *verts, int cnt, const f32 *pos3, const f32 *rot33, const f32 *sc, f32 sr);
static int polyhedron_is_intersecting_aabb_transform(const f32 *verts, int cnt, const f32 *apos3, const f32 *arot33, const f32 *min, const f32 *max);
static int polyhedron_is_intersecting_capsule_transform(const f32 *verts, int cnt, const f32 *pos3, const f32 *rot33, const f32 *ca, const f32 *cb, f32 cr);
static int polyhedron_is_intersecting_polyhedron_transform(const f32 *averts, int acnt, const f32 *apos3, const f32 *arot33, const f32 *bverts, int bcnt, const f32 *bpos3, const f32 *brot33);
// polyhedron: gjk result
static int polyhedron_intersect_sphere(gjk_result *res, const f32 *verts, int cnt, const f32 *sc, f32 sr);
static int polyhedron_intersect_aabb(gjk_result *res, const f32 *verts, int cnt, const f32 *min, const f32 *max);
static int polyhedron_intersect_capsule(gjk_result *res, const f32 *verts, int cnt, const f32 *ca, const f32 *cb, f32 cr);
static int polyhedron_intersect_polyhedron(gjk_result *res, const f32 *averts, int acnt, const f32 *bverts, int bcnt);
// polyhedron: gjk result transformed
static int polyhedron_intersect_sphere_transform(gjk_result *res, const f32 *verts, int cnt, const f32 *pos3, const f32 *rot33, const f32 *sc, f32 sr);
static int polyhedron_intersect_aabb_transform(gjk_result *res, const f32 *verts, int cnt, const f32 *pos3, const f32 *rot33, const f32 *min, const f32 *max);
static int polyhedron_intersect_capsule_transform(gjk_result *res, const f32 *verts, int cnt, const f32 *pos3, const f32 *rot33, const f32 *ca, const f32 *cb, f32 cr);
static int polyhedron_intersect_polyhedron_transform(gjk_result *res, const f32 *averts, int acnt, const f32 *at3, const f32 *ar33, const f32 *bverts, int bcnt, const f32 *bt3, const f32 *br33);

static void planeq(f32 *r, const f32 *n, const f32 *p) {
  f3cpy(r,n);
  r[3] = -f3dot(n,p);
}

static void planeqf(f32 *r, f32 nx, f32 ny, f32 nz, f32 px, f32 py, f32 pz) {
  // Plane: ax + by + cz + d
  // Equation:
  //      n * (p - p0) = 0
  //      n * p - n * p0 = 0
  //      |a b c| * p - |a b c| * p0
  //
  //      |a b c| * p + d = 0
  //          d = -1 * |a b c| * p0
  //
  //  Plane: |a b c d| d = -|a b c| * p0
  //
  f32 n[3], p[3];
  f3set(n,nx,ny,nz);
  f3set(p,px,py,pz);
  planeq(r, n, p);
}

static void segment_closest_point_to_point(f32 *res, const f32 *a, const f32 *b, const f32 *p) {
  f32 ab[3], pa[3];
  f3sub(ab, b,a);
  f3sub(pa, p,a);
  f32 t = f3dot(pa,ab) / f3dot(ab,ab);
  if (t < 0.0f) t = 0.0f;
  if (t > 1.0f) t = 1.0f;
  f3mul(res, ab, t);
  f3add(res, a, res);
}

static f32 segment_closest_point_to_point_sqdist(const f32 *a, const f32 *b, const f32 *p) {
  f32 ab[3], ap[3], bp[3];
  f3sub(ab,a,b);
  f3sub(ap,a,p);
  f3sub(bp,a,p);
  f32 e = f3dot(ap,ab);

  // handle cases p proj outside ab
  if (e <= 0.0f) return f3dot(ap,ap);
  f32 f = f3dot(ab,ab);
  if (e >= f) return f3dot(bp,bp);
  return f3dot(ap,ap) - (e*e)/f;
}

static f32 segment_closest_point_to_segment(
  f32 *t1, f32 *t2, f32 *c1, f32 *c2, const f32 *p1, const f32 *q1, const f32 *p2, const f32 *q2) {
#define EPSILON (1e-6)
  f32 r[3], d1[3], d2[3];
  f3sub(d1, q1, p1); // direction vector segment s1
  f3sub(d2, q2, p2); //direction vector segment s2
  f3sub(r, p1, p2);

  f32 a = f3dot(d1, d1);
  f32 e = f3dot(d2, d2);
  f32 f = f3dot(d2, r);

  if (a <= EPSILON && e <= EPSILON) {
    // both segments degenerate into points
    f32 d12[3];
    *t1 = *t2 = 0.0f;
    f3cpy(c1, p1);
    f3cpy(c2, p2);
    f3sub(d12, c1, c2);
    return f3dot(d12,d12);
  }
  if (a > EPSILON) {
    f32 c = f3dot(d1,r);
    if (e > EPSILON) {
      // non-degenerate case
      f32 b = f3dot(d1,d2);
      f32 denom = a*e - b*b;

      // compute closest point on L1/L2 if not parallel else pick any t2
      if (denom != 0.0f)
        *t1 = clamp(0.0f, (b*f - c*e) / denom, 1.0f);
      else *t1 = 0.0f;

      // cmpute point on L2 closest to S1(s)
      *t2 = (b*(*t1) + f) / e;
      if (*t2 < 0.0f) {
        *t2 = 0.0f;
        *t1 = clamp(0.0f, -c/a, 1.0f);
      } else if (*t2 > 1.0f) {
        *t2 = 1.0f;
        *t1 = clamp(0.0f, (b-c)/a, 1.0f);
      }
    } else {
      // second segment degenerates into a point
      *t1 = clamp(0.0f, -c/a, 1.0f);
      *t2 = 0.0f;
    }
  } else {
    // first segment degenerates into a point
    *t2 = clamp(0.0f, f / e, 1.0f);
    *t1 = 0.0f;
  }
  // calculate closest points
  f32 n[3], d12[3];
  f3mul(n, d1, *t1);
  f3add(c1, p1, n);
  f3mul(n, d2, *t2);
  f3add(c2, p2, n);

  // calculate squared distance
  f3sub(d12, c1, c2);
  return f3dot(d12,d12);
}

static f32 ray_intersects_plane(const f32 *ro, const f32 *rd, const f32 *plane) {
  // Ray: P = origin + rd * t
  // Plane: plane_normal * P + d = 0
  //
  // Substitute:
  //      normal * (origin + rd*t) + d = 0
  //
  // Solve for t:
  //      plane_normal * origin + plane_normal * rd*t + d = 0
  //      -(plane_normal*rd*t) = plane_normal * origin + d
  //
  //                  plane_normal * origin + d
  //      t = -1 * -------------------------
  //                  plane_normal * rd
  //
  // Result:
  //      Behind: t < 0
  //      Infront: t >= 0
  //      Parallel: t = 0
  //      Intersection point: ro + rd * t
  //
  f32 n = -(f3dot(plane,ro) + plane[3]);
  if (fabs(n) < 0.0001f) return 0.0f;
  return n/(f3dot(plane,rd));
}

static f32 ray_intersects_triangle(const f32 *ro, const f32 *rd, const f32 *p0, const f32 *p1, const f32 *p2) {
  f32 p[4];
  f32 t = 0;
  f32 di0[3], di1[3], di2[3];
  f32 d21[3], d02[3], in[3];
  f32 n[3], d10[3], d20[3];
  f32 in0[3], in1[3], in2[3];

  // calculate triangle normal
  f3sub(d10, p1,p0);
  f3sub(d20, p2,p0);
  f3sub(d21, p2,p1);
  f3sub(d02, p0,p2);
  f3cross(n, d10,d20);

  // check for plane intersection
  planeq(p, n, p0);
  t = ray_intersects_plane(ro, rd, p);
  if (t <= 0.0f) return t;

  // intersection point
  f3mul(in,rd,t);
  f3add(in,in,ro);

  // check if point inside triangle in plane
  f3sub(di0, in, p0);
  f3sub(di1, in, p1);
  f3sub(di2, in, p2);

  f3cross(in0, d10, di0);
  f3cross(in1, d21, di1);
  f3cross(in2, d02, di2);

  if (f3dot(in0,n) < 0.0f)
    return -1;
  if (f3dot(in1,n) < 0.0f)
    return -1;
  if (f3dot(in2,n) < 0.0f)
    return -1;
  return t;
}

static int ray_intersects_sphere(f32 *t0, f32 *t1, const f32 *ro, const f32 *rd, const f32 *c, f32 r) {
  f32 a[3];
  f32 tc,td,d2,r2;
  f3sub(a,c,ro);
  tc = f3dot(rd,a);
  if (tc < 0) return 0;

  r2 = r*r;
  d2 = f3dot(a,a) - tc*tc;
  if (d2 > r2) return 0;
  td = sqrtf(r2 - d2);

  *t0 = tc - td;
  *t1 = tc + td;
  return 1;
}

static int ray_intersects_aabb( f32 *t0, f32 *t1, const f32 *ro, const f32 *rd, const f32 *min, const f32 *max) {
  f32 t0x = (min[0] - ro[0]) / rd[0];
  f32 t0y = (min[1] - ro[1]) / rd[1];
  f32 t0z = (min[2] - ro[2]) / rd[2];
  f32 t1x = (max[0] - ro[0]) / rd[0];
  f32 t1y = (max[1] - ro[1]) / rd[1];
  f32 t1z = (max[2] - ro[2]) / rd[2];

  f32 tminx = min(t0x, t1x);
  f32 tminy = min(t0y, t1y);
  f32 tminz = min(t0z, t1z);
  f32 tmaxx = max(t0x, t1x);
  f32 tmaxy = max(t0y, t1y);
  f32 tmaxz = max(t0z, t1z);
  if (tminx > tmaxy || tminy > tmaxx)
    return 0;

  *t0 = max(tminx, tminy);
  *t1 = min(tmaxy, tmaxx);
  if (*t0 > tmaxz || tminz> *t1)
    return 0;

  *t0 = max(*t0, tminz);
  *t1 = min(*t1, tmaxz);
  return 1;
}

static void sphere_closest_point_to_point(f32 *res, const f32 *c, const f32 r, const f32 *p) {
  f32 d[3], n[3];
  f3sub(d, p, c);
  f3norm(d);
  f3mul(res,n,r);
  f3add(res,c,res);
}

static int sphere_intersects_sphere(const f32 *ca, f32 ra, const f32 *cb, f32 rb) {
  f32 d[3];
  f3sub(d, cb, ca);
  f32 r = ra + rb;
  if (f3dot(d,d) > r*r)
    return 0;
  return 1;
}

static int sphere_intersects_sphere_manifold(manifold *m, const f32 *ca, f32 ra, const f32 *cb, f32 rb) {
  f32 d[3];
  f3sub(d, cb, ca);
  f32 r = ra + rb;
  f32 d2 = f3dot(d,d);
  if (d2 <= r*r) {
    f32 l = sqrtf(d2);
    f32 linv = 1.0f / ((l != 0) ? l: 1.0f);
    f3mul(m->normal, d, linv);
    m->depth = r - l;
    f3mul(d, m->normal, rb);
    f3sub(m->contact_point, cb, d);
    return 1;
  } return 0;
}

static int sphere_intersect_aabb(const f32 *c, f32 r, const f32 *min, const f32 *max) {
  return aabb_intersect_sphere(min, max, c, r);
}

static int sphere_intersect_aabb_manifold(manifold *m, const f32 *c, f32 r, const f32 *min, const f32 *max) {
  // find closest aabb point to sphere center point
  f32 ap[3], d[3];
  aabb_closest_point_to_point(ap, min, max, c);
  f3sub(d, c, ap);
  f32 d2 = f3dot(d, d);
  if (d2 > r*r) return 0;

  // calculate distance vector between sphere and aabb center points
  f32 ac[3];
  f3sub(ac, max, min);
  f3mul(ac, ac, 0.5f);
  f3add(ac, min, ac);
  f3sub(d, ac, c);

  // normalize distance vector
  f32 l2 = f3dot(d,d);
  f32 l = l2 != 0.0f ? sqrtf(l2): 1.0f;
  f32 linv = 1.0f/l;
  f3mul(d, d, linv);

  f3cpy(m->normal, d);
  f3mul(m->contact_point, m->normal, r);
  f3add(m->contact_point, c, m->contact_point);

  // calculate penetration depth
  f32 sp[3];
  sphere_closest_point_to_point(sp, c, r, ap);
  f3sub(d, sp, ap);
  m->depth = sqrtf(f3dot(d,d)) - l;
  return 1;
}

static int sphere_intersect_capsule(const f32 *sc, f32 sr, const f32 *ca, const f32 *cb, f32 cr) {
  return capsule_intersect_sphere(ca, cb, cr, sc, sr);
}

static int sphere_intersect_capsule_manifold(manifold *m, const f32 *sc, f32 sr, const f32 *ca, const f32 *cb, f32 cr) {
  // find closest capsule point to sphere center point
  f32 cp[3];
  capsule_closest_point_to_point(cp, ca, cb, cr, sc);
  f3sub(m->normal, cp, sc);
  f32 d2 = f3dot(m->normal, m->normal);
  if (d2 > sr*sr) return 0;

  // normalize manifold normal vector
  f32 l = d2 != 0.0f ? sqrtf(d2): 1;
  f32 linv = 1.0f/l;
  f3mul(m->normal, m->normal, linv);

  // calculate penetration depth
  f3mul(m->contact_point, m->normal, sr);
  f3add(m->contact_point, sc, m->contact_point);
  m->depth = d2 - sr*sr;
  m->depth = m->depth != 0.0f ? sqrtf(m->depth): 0.0f;
  return 1;
}

static int sphere_intersecting_polyhedron(const f32 *c, f32 r, const f32 *verts, int cnt) {
  return polyhedron_is_intersecting_sphere(verts, cnt, c, r);
}

static void aabb_rebalance_transform(f32 *bmin, f32 *bmax, const f32 *amin, const f32 *amax, const f32 *m, const f32 *t) {
  for (int i = 0; i < 3; ++i) {
    bmin[i] = bmax[i] = t[i];
    for (int j = 0; j < 3; ++j) {
      f32 e = m[i*3+j] * amin[j];
      f32 f = m[i*3+j] * amax[j];
      if (e < f) {
        bmin[i] += e;
        bmax[i] += f;
      } else {
        bmin[i] += f;
        bmax[i] += e;
      }
    }
  }
}

static void aabb_closest_point_to_point(f32 *res, const f32 *min, const f32 *max, const f32 *p) {
  for (int i = 0; i < 3; ++i) {
    f32 v = p[i];
    if (v < min[i]) v = min[i];
    if (v > max[i]) v = max[i];
    res[i] = v;
  }
}

static f32 aabb_sqdist_to_point(const f32 *min, const f32 *max, const f32 *p) {
  f32 r = 0;
  for (int i = 0; i < 3; ++i) {
    f32 v = p[i];
    if (v < min[i]) r += (min[i]-v) * (min[i]-v);
    if (v > max[i]) r += (v-max[i]) * (v-max[i]);
  } return r;
}

static int aabb_contains_point(const f32 *amin, const f32 *amax, const f32 *p) {
  if (p[0] < amin[0] || p[0] > amax[0]) return 0;
  if (p[1] < amin[1] || p[1] > amax[1]) return 0;
  if (p[2] < amin[2] || p[2] > amax[2]) return 0;
  return 1;
}

static int aabb_intersect_aabb(const f32 *amin, const f32 *amax, const f32 *bmin, const f32 *bmax) {
  if (amax[0] < bmin[0] || amin[0] > bmax[0]) return 0;
  if (amax[1] < bmin[1] || amin[1] > bmax[1]) return 0;
  if (amax[2] < bmin[2] || amin[2] > bmax[2]) return 0;
  return 1;
}

static int aabb_intersect_aabb_manifold(
  manifold *m,
  const f32 *amin, const f32 *amax,
  const f32 *bmin, const f32 *bmax
) {
  if (!aabb_intersect_aabb(amin, amax, bmin, bmax))
    return 0;

  // calculate distance vector between both aabb center points
  f32 ac[3], bc[3], d[3];
  f3sub(ac, amax, amin);
  f3sub(bc, bmax, bmin);

  f3mul(ac, ac, 0.5f);
  f3mul(bc, bc, 0.5f);

  f3add(ac, amin, ac);
  f3add(bc, bmin, bc);
  f3sub(d, bc, ac);

  // normalize distance vector
  f32 l2 = f3dot(d,d);
  f32 l = l2 != 0.0f ? sqrtf(l2): 1.0f;
  f32 linv = 1.0f/l;
  f3mul(d, d, linv);

  // calculate contact point
  f3cpy(m->normal, d);
  aabb_closest_point_to_point(m->contact_point, amin, amax, bc);
  f3sub(d, m->contact_point, ac);

  // calculate penetration depth
  f32 r2 = f3dot(d,d);
  f32 r = sqrtf(r2);
  m->depth = r - l;
  return 1;
}

static int aabb_intersect_sphere(const f32 *min, const f32 *max, const f32 *c, f32 r) {
  // compute squared distance between sphere center and aabb
  f32 d2 = aabb_sqdist_to_point(min, max, c);
  // intersection if distance is smaller/equal sphere radius
  return d2 <= r*r;
}

static int aabb_intersect_sphere_manifold(manifold *m, const f32 *min, const f32 *max, const f32 *c, f32 r) {
  // find closest aabb point to sphere center point
  f32 d[3];
  aabb_closest_point_to_point(m->contact_point, min, max, c);
  f3sub(d, c, m->contact_point);
  f32 d2 = f3dot(d, d);
  if (d2 > r*r) return 0;

  // calculate distance vector between aabb and sphere center points
  f32 ac[3];
  f3sub(ac, max, min);
  f3mul(ac, ac, 0.5f);
  f3add(ac, min, ac);
  f3sub(d, c, ac);

  // normalize distance vector
  f32 l2 = f3dot(d,d);
  f32 l = l2 != 0.0f ? sqrtf(l2): 1.0f;
  f32 linv = 1.0f/l;
  f3mul(d, d, linv);

  // calculate penetration depth
  f3cpy(m->normal, d);
  f3sub(d, m->contact_point, ac);
  m->depth = sqrtf(f3dot(d,d));
  return 1;
}

static int aabb_intersect_capsule(const f32 *min, const f32 *max, const f32 *ca, const f32 *cb, f32 cr) {
  return capsule_intersect_aabb(ca, cb, cr, min, max);
}

static int aabb_intersect_capsule_manifold(
  manifold *m,
  const f32 *min, const f32 *max,
  const f32 *ca, const f32 *cb, f32 cr
) {
  // calculate aabb center point
  f32 ac[3];
  f3sub(ac, max, min);
  f3mul(ac, ac, 0.5f);
  f3add(ac, min, ac);

  // calculate closest point from aabb to point on capsule and check if inside aabb
  f32 cp[3];
  capsule_closest_point_to_point(cp, ca, cb, cr, ac);
  if (!aabb_contains_point(min, max, cp))
    return 0;

  // vector and distance between both capsule closests point and aabb center
  f32 d[3], d2;
  f3sub(d, cp, ac);
  d2 = f3dot(d,d);

  // calculate penetration depth from closest aabb point to capsule
  f32 ap[3], dt[3];
  aabb_closest_point_to_point(ap, min, max, cp);
  f3sub(dt, ap, cp);
  m->depth = sqrtf(f3dot(dt,dt));

  // calculate normal
  f32 l = sqrtf(d2);
  f32 linv = 1.0f / ((l != 0.0f) ? l: 1.0f);
  f3mul(m->normal, d, linv);
  f3cpy(m->contact_point, ap);
  return 1;
}

static int aabb_intersect_polyhedron(const f32 *min, const f32 *max, const f32 *verts, int cnt) {
  return polyhedron_is_intersecting_aabb(verts, cnt, min, max);
}

static f32 capsule_point_sqdist(const f32 *ca, const f32 *cb, f32 cr, const f32 *p) {
  f32 d2 = segment_closest_point_to_point_sqdist(ca, cb, p);
  return d2 - (cr*cr);
}

static void capsule_closest_point_to_point(f32 *res, const f32 *ca, const f32 *cb, f32 cr, const f32 *p) {
  // calculate closest point to internal capsule segment
  f32 pp[3], d[3];
  segment_closest_point_to_point(pp, ca, cb, p);

  // extend point out by radius in normal direction
  f3sub(d,p,pp);
  f3norm(d);
  f3mul(res, d, cr);
  f3add(res, pp, res);
}

static int capsule_intersect_capsule(
  const f32 *aa, const f32 *ab, f32 ar,
  const f32 *ba, const f32 *bb, f32 br
) {
  f32 t1, t2;
  f32 c1[3], c2[3];
  f32 d2 = segment_closest_point_to_segment(&t1, &t2, c1, c2, aa, ab, ba, bb);
  f32 r = ar + br;
  return d2 <= r*r;
}

static int capsule_intersect_capsule_manifold(
  manifold *m,
  const f32 *aa, const f32 *ab, f32 ar,
  const f32 *ba, const f32 *bb, f32 br
) {
  f32 t1, t2;
  f32 c1[3], c2[3];
  f32 d2 = segment_closest_point_to_segment(&t1, &t2, c1, c2, aa, ab, ba, bb);
  f32 r = ar + br;
  if (d2 > r*r) return 0;

  // calculate normal from both closest points for each segement
  f32 cp[3], d[3];
  f3sub(m->normal, c2, c1);
  f3norm(m->normal);

  // calculate contact point from closest point and depth
  capsule_closest_point_to_point(m->contact_point, aa, ab, ar, c2);
  capsule_closest_point_to_point(cp, ba, bb, br, c1);
  f3sub(d, c1, cp);
  m->depth = sqrtf(f3dot(d,d));
  return 1;
}

static int capsule_intersect_sphere(
  const f32 *ca, const f32 *cb, f32 cr,
  const f32 *sc, f32 sr
) {
  // squared distance bwetween sphere center and capsule line segment
  f32 d2 = segment_closest_point_to_point_sqdist(ca,cb,sc);
  f32 r = sr + cr;
  return d2 <= r * r;
}

static int capsule_intersect_sphere_manifold(
  manifold *m,
  const f32 *ca, const f32 *cb, f32 cr,
  const f32 *sc, f32 sr
) {
  // find closest capsule point to sphere center point
  capsule_closest_point_to_point(m->contact_point, ca, cb, cr, sc);
  f3sub(m->normal, sc, m->contact_point);
  f32 d2 = f3dot(m->normal, m->normal);
  if (d2 > sr*sr) return 0;

  // normalize manifold normal vector
  f32 l = d2 != 0.0f ? sqrtf(d2): 1;
  f32 linv = 1.0f/l;
  f3mul(m->normal, m->normal, linv);

  // calculate penetration depth
  m->depth = d2 - sr*sr;
  m->depth = m->depth != 0.0f ? sqrtf(m->depth): 0.0f;
  return 1;
}

static int capsule_intersect_aabb(const f32 *ca, const f32 *cb, f32 cr, const f32 *min, const f32 *max) {
  // calculate aabb center point
  f32 ac[3];
  f3sub(ac, max, min);
  f3mul(ac, ac, 0.5f);

  // calculate closest point from aabb to point on capsule and check if inside aabb
  f32 p[3];
  capsule_closest_point_to_point(p, ca, cb, cr, ac);
  return aabb_contains_point(min, max, p);
}

static int capsule_intersect_aabb_manifold(
  manifold *m,
  const f32 *ca, const f32 *cb, f32 cr,
  const f32 *min, const f32 *max
) {
  // calculate aabb center point
  f32 ac[3];
  f3sub(ac, max, min);
  f3mul(ac, ac, 0.5f);
  f3add(ac, min, ac);

  // calculate closest point from aabb to point on capsule and check if inside aabb
  f32 cp[3];
  capsule_closest_point_to_point(cp, ca, cb, cr, ac);
  if (!aabb_contains_point(min, max, cp))
    return 0;

  // vector and distance between both capsule closests point and aabb center
  f32 d[3], d2;
  f3sub(d, ac, cp);
  d2 = f3dot(d,d);

  // calculate penetration depth from closest aabb point to capsule
  f32 ap[3], dt[3];
  aabb_closest_point_to_point(ap, min, max, cp);
  f3sub(dt, ap, cp);
  m->depth = sqrtf(f3dot(dt,dt));

  // calculate normal
  f32 l = sqrtf(d2);
  f32 linv = 1.0f / ((l != 0.0f) ? l: 1.0f);
  f3mul(m->normal, d, linv);
  f3cpy(m->contact_point, cp);
  return 1;
}

static int capsule_intersect_polyhedron(const f32 *ca, const f32 *cb, f32 cr, const f32 *verts, int cnt) {
  return polyhedron_is_intersecting_capsule(verts, cnt, ca, cb, cr);
}

static int line_support(f32 *support, const f32 *d, const f32 *a, const f32 *b) {
  int i = 0;
  f32 adot = f3dot(a, d);
  f32 bdot = f3dot(b, d);
  if (adot < bdot) {
    f3cpy(support, b);
    i = 1;
  } else f3cpy(support, a);
  return i;
}

static int polyhedron_support(f32 *support, const f32 *d, const f32 *verts, int cnt) {
  int imax = 0;
  f32 dmax = f3dot(verts, d);
  for (int i = 1; i < cnt; ++i) {
    // find vertex with max dot product in direction d 
    f32 dot = f3dot(&verts[i*3], d);
    if (dot < dmax) continue;
    imax = i, dmax = dot;
  } f3cpy(support, &verts[imax*3]);
  return imax;
}

static int polyhedron_intersect_sphere(
  gjk_result *res,
  const f32 *verts, int cnt,
  const f32 *sc, f32 sr
) {
  // initial guess
  f32 d[3] = {0};
  gjk_support s = {0};
  f3cpy(s.a, verts);
  f3cpy(s.b, sc);
  f3sub(d, s.b, s.a);

  // run gjk algorithm
  gjk_simplex gsx = {0};
  while (gjk(&gsx, &s, d)) {
    f32 n[3]; f3mul(n, d, -1);
    s.aid = polyhedron_support(s.a, n, verts, cnt);
    f3sub(d, s.b, s.a);
  }
  // check distance between closest points
  *res = gjk_analyze(&gsx);
  return res->distance_squared <= sr*sr;
}

static int polyhedron_intersect_sphere_transform(
  gjk_result *res,
  const f32 *verts, int cnt, const f32 *pos3, const f32 *rot33,
  const f32 *sc, f32 sr
) {
  // initial guess
  f32 d[3] = {0};
  gjk_support s = {0};
  f3cpy(s.a, verts);
  f3cpy(s.b, sc);
  transformS(s.a, rot33, pos3);
  f3sub(d, s.b, s.a);

  // run gjk algorithm
  gjk_simplex gsx = {0};
  while (gjk(&gsx, &s, d)) {
    f32 n[3]; f3mul(n, d, -1);
    f32 da[3]; transformT(da, n, rot33, pos3);

    s.aid = polyhedron_support(s.a, da, verts, cnt);
    transformS(s.a, rot33, pos3);
    f3sub(d, s.b, s.a);
  }
  // check distance between closest points
  *res = gjk_analyze(&gsx);
  return res->distance_squared <= sr*sr;
}

static int polyhedron_is_intersecting_sphere(
  const f32 *verts, int cnt,
  const f32 *sc, f32 sr
) {
  gjk_result res;
  return polyhedron_intersect_sphere(&res, verts, cnt, sc, sr);
}

static int polyhedron_is_intersecting_sphere_transform(
  const f32 *verts, int cnt, const f32 *pos3,
  const f32 *rot33, const f32 *sc, f32 sr
) {
  gjk_result res;
  return polyhedron_intersect_sphere_transform(&res, verts, cnt, pos3, rot33, sc, sr);
}

static int polyhedron_intersect_capsule(
  gjk_result *res,
  const f32 *verts, int cnt,
  const f32 *ca, const f32 *cb, f32 cr
) {
  // initial guess
  f32 d[3] = {0};
  gjk_support s = {0};
  f3cpy(s.a, verts);
  f3cpy(s.b, ca);
  f3sub(d, s.b, s.a);

  // run gjk algorithm
  gjk_simplex gsx = {0};
  while (gjk(&gsx, &s, d)) {
    f32 n[3]; f3mul(n, d, -1);
    s.aid = polyhedron_support(s.a, n, verts, cnt);
    s.bid = line_support(s.b, d, ca, cb);
    f3sub(d, s.b, s.a);
  }
  // check distance between closest points
  assert(gsx.iter < gsx.max_iter);
  *res = gjk_analyze(&gsx);
  return res->distance_squared <= cr*cr;
}

static int polyhedron_is_intersecting_capsule(
  const f32 *verts, int cnt,
  const f32 *ca, const f32 *cb, f32 cr
) {
  gjk_result res;
  return polyhedron_intersect_capsule(&res, verts, cnt, ca, cb, cr);
}

static int polyhedron_intersect_capsule_transform(
  gjk_result *res,
  const f32 *verts, int cnt, const f32 *pos3, const f32 *rot33,
  const f32 *ca, const f32 *cb, f32 cr
) {
  // initial guess
  f32 d[3] = {0};
  gjk_support s = {0};
  f3cpy(s.a, verts);
  f3cpy(s.b, ca);
  transformS(s.a, rot33, pos3);
  f3sub(d, s.b, s.a);

  // run gjk algorithm
  gjk_simplex gsx = {0};
  while (gjk(&gsx, &s, d)) {
    f32 n[3]; f3mul(n, d, -1);
    f32 da[3]; transformT(da, n, rot33, pos3);

    s.aid = polyhedron_support(s.a, da, verts, cnt);
    s.bid = line_support(s.b, d, ca, cb);
    transformS(s.a, rot33, pos3);
    f3sub(d, s.b, s.a);
  }
  // check distance between closest points
  *res = gjk_analyze(&gsx);
  return res->distance_squared <= cr*cr;
}

static int polyhedron_is_intersecting_capsule_transform(
  const f32 *verts, int cnt,
  const f32 *pos3, const f32 *rot33,
  const f32 *ca, const f32 *cb, f32 cr
) {
  gjk_result res;
  return polyhedron_intersect_capsule_transform(&res, verts, cnt, pos3, rot33, ca, cb, cr);
}

static int polyhedron_intersect_polyhedron_transform(
  gjk_result *res,
  const f32 *averts, int acnt, const f32 *at3, const f32 *ar33,
  const f32 *bverts, int bcnt, const f32 *bt3, const f32 *br33
) {
  // initial guess
  f32 d[3] = {0};
  gjk_support s = {0};
  f3cpy(s.a, averts);
  f3cpy(s.b, bverts);
  transformS(s.a, ar33, at3);
  transformS(s.b, br33, bt3);
  f3sub(d, s.b, s.a);

  // run gjk algorithm
  gjk_simplex gsx = {0};
  while (gjk(&gsx, &s, d)) {
    // transform direction
    f32 n[3]; f3mul(n, d, -1);
    f32 da[3]; transformT(da, n, ar33, at3);
    f32 db[3]; transformT(db, d, br33, bt3);
    // run support function on tranformed directions
    s.aid = polyhedron_support(s.a, da, averts, acnt);
    s.bid = polyhedron_support(s.b, db, bverts, bcnt);
    // calculate distance vector on transformed points
    transformS(s.a, ar33, at3);
    transformS(s.b, br33, bt3);
    f3sub(d, s.b, s.a);
  }
  *res = gjk_analyze(&gsx);
  return gsx.hit;
}

static int polyhedron_intersect_polyhedron(
  gjk_result *res,
  const f32 *averts, int acnt,
  const f32 *bverts, int bcnt
) {
  // initial guess
  f32 d[3] = {0};
  gjk_support s = {0};
  f3cpy(s.a, averts);
  f3cpy(s.b, bverts);
  f3sub(d, s.b, s.a);

  // run gjk algorithm
  gjk_simplex gsx = {0};
  while (gjk(&gsx, &s, d)) {
    f32 n[3]; f3mul(n, d, -1);
    s.aid = polyhedron_support(s.a, n, averts, acnt);
    s.bid = polyhedron_support(s.b, d, bverts, bcnt);
    f3sub(d, s.b, s.a);
  }
  *res = gjk_analyze(&gsx);
  return gsx.hit;
}

static int polyhedron_is_intersecting_polyhedron(
  const f32 *averts, int acnt,
  const f32 *bverts, int bcnt
) {
  gjk_result res;
  return polyhedron_intersect_polyhedron(&res, averts, acnt, bverts, bcnt);
}

static int polyhedron_is_intersecting_polyhedron_transform(
  const f32 *averts, int acnt,
  const f32 *apos3, const f32 *arot33,
  const f32 *bverts, int bcnt,
  const f32 *bpos3, const f32 *brot33
) {
  gjk_result res;
  return polyhedron_intersect_polyhedron_transform(
    &res, averts, acnt,
    apos3, arot33, bverts, bcnt, bpos3, brot33);
}

static int polyhedron_intersect_aabb(
  gjk_result *res,
  const f32 *verts, int cnt,
  const f32 *min, const f32 *max
) {
  f32 box[24];
  f3set(box+0, min[0], min[1], min[2]),
  f3set(box+3, min[0], min[1], max[2]);
  f3set(box+6, min[0], max[1], min[2]);
  f3set(box+9, min[0], max[1], max[2]);
  f3set(box+12,max[0], min[1], min[2]);
  f3set(box+15,max[0], min[1], max[2]);
  f3set(box+18,max[0], max[1], min[2]);
  f3set(box+21,max[0], max[1], max[2]);
  return polyhedron_intersect_polyhedron(res, verts, cnt, box, 8);
}

static int polyhedron_intersect_aabb_transform(
  gjk_result *res,
  const f32 *verts, int cnt, const f32 *pos3, const f32 *rot33,
  const f32 *min, const f32 *max
) {
  f32 box[24];
  static const f32 zero[3];
  static const f32 id[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
  f3set(box+0, min[0], min[1], min[2]),
  f3set(box+3, min[0], min[1], max[2]);
  f3set(box+6, min[0], max[1], min[2]);
  f3set(box+9, min[0], max[1], max[2]);
  f3set(box+12,max[0], min[1], min[2]);
  f3set(box+15,max[0], min[1], max[2]);
  f3set(box+18,max[0], max[1], min[2]);
  f3set(box+21,max[0], max[1], max[2]);
  return polyhedron_intersect_polyhedron_transform(res, verts, cnt, pos3, rot33,
                                                   box, 8, zero, &id[0][0]);
}

static int polyhedron_is_intersecting_aabb(
  const f32 *verts, int cnt,
  const f32 *min, const f32 *max
) {
  gjk_result res;
  return polyhedron_intersect_aabb(&res, verts, cnt, min, max);
}

static int polyhedron_is_intersecting_aabb_transform(
  const f32 *verts, int cnt,
  const f32 *apos3, const f32 *arot33, const f32 *min, const f32 *max
) {
  gjk_result res;
  return polyhedron_intersect_aabb_transform(&res, verts, cnt, apos3, arot33, min, max);
}

// =========================================================================== //
//
//                                  MATH OBJECTS
//
// =========================================================================== //
#define vf(v) (&((v).x))
#define v3unpack(v) (v).x,(v).y,(v).z

static v3  v3mk(f32 x, f32 y, f32 z) { v3 r; f3set(&r.x, x,y,z); return r; }
static v3  v3mkv(const f32 *v)       { v3 r; f3set(&r.x, v[0],v[1],v[2]); return r; }
static v3  v3add(v3 a, v3 b)         { f3add(vf(a),vf(a),vf(b)); return a; }
static v3  v3sub(v3 a, v3 b)         { f3sub(vf(a),vf(a),vf(b)); return a; }
static v3  v3scale(v3 v, f32 s)      { f3mul(vf(v),vf(v),s); return v; }
static f32 v3dot(v3 a, v3 b)         { return f3dot(vf(a),vf(b)); }
static v3  v3norm(v3 v)              { f3norm(vf(v)); return v; }
static v3  v3cross(v3 a, v3 b)       { v3 r; f3cross(vf(r),vf(a),vf(b)); return r; }
static v3  v3lerp(v3 a, f32 t, v3 b) { v3 r; f3lerp(vf(r),vf(a),t,vf(b)); return r; }

// =========================================================================== //
//
//                              COLLISION VOLUME
//
// =========================================================================== //
typedef struct { v3 min, max; } aabb;
typedef struct { v3 p, n; } plane;
typedef struct { v3 a, b; f32 r; } capsule;
typedef struct { v3 p, d; } ray;
typedef struct { v3 p, n; f32 t0, t1; } raycast;

static int ray_test_plane(raycast *o, ray r,  plane p);
static int ray_test_triangle(raycast *o, ray r, const v3 *tri);
static int ray_test_sphere(raycast *o, ray r, sphere s);
static int ray_test_aabb(raycast *o, ray r, aabb a);
// sphere
static int sphere_test_sphere(sphere a, sphere b);
static int sphere_test_sphere_manifold(manifold *m, sphere a, sphere b);
static int sphere_test_aabb(sphere s, aabb a);
static int sphere_test_aabb_manifold(manifold *m, sphere s, aabb a);
static int sphere_test_capsule(sphere s, capsule c);
static int sphere_test_capsule_manifold(manifold *m, sphere s, capsule c);
// aabb
static aabb aabb_transform(aabb a, const f32 *rot, const v3 t);
static v3 aabb_closest_point(aabb a, v3 p);
static int aabb_test_aabb(aabb a, aabb b);
static int aabb_test_aabb_manifold(manifold *m,  aabb a, aabb b);
static int aabb_test_sphere( aabb a, sphere s);
static int aabb_test_sphere_manifold(manifold *m, aabb a, sphere s);
static int aabb_test_capsule(aabb a, capsule c);
static int aabb_test_capsule_manifold(manifold *m, aabb a, capsule c);
// capsule
static f32 capsule_sqdist_point(capsule c, v3 p);
static v3 capsule_closest_point(capsule c, v3 pnt);
static int capsule_test_sphere(capsule c, sphere s);
static int capsule_test_sphere_manifold(manifold *m, capsule c, sphere s);
static int capsule_test_aabb(capsule c, aabb a);
static int capsule_test_aabb_manifold(manifold *m, capsule c, aabb a);
static int capsule_test_capsule(capsule a, capsule b);
static int capsule_test_capsule_manifold(manifold *m, capsule a, capsule b);

static int ray_test_plane(raycast *o, ray r,  plane p) {
  f32 pf[4];
  planeq(pf, &p.n.x, &p.p.x);
  f32 t = ray_intersects_plane(&r.p.x, &r.d.x, pf);
  if (t <= 0.0f) return 0;
  o->p = v3add(r.p, v3scale(r.d, t));
  o->t0 = o->t1 = t;
  o->n = v3scale(p.n, -1.0f);
  return 1;
}

static int ray_test_triangle(raycast *o, ray r, const v3 *tri) {
  f32 t = ray_intersects_triangle(&r.p.x, &r.d.x, &tri[0].x, &tri[1].x, &tri[2].x);
  if (t <= 0) return 0;
  o->t0 = o->t1 = t;
  o->p = v3add(r.p, v3scale(r.d, t));
  o->n = v3norm(v3cross(v3sub(tri[1],tri[0]),v3sub(tri[2],tri[0])));
  return 1;
}

static int ray_test_sphere(raycast *o, ray r, sphere s) {
  if (!ray_intersects_sphere(&o->t0, &o->t1, &r.p.x, &r.d.x, &s.p.x, s.r))
    return 0;
  o->p = v3add(r.p, v3scale(r.d, min(o->t0,o->t1)));
  o->n = v3norm(v3sub(o->p, s.p));
  return 1;
}

static int ray_test_aabb(raycast *o, ray r, aabb a) {
  v3 pnt, ext, c;
  f32 d, min;

  if (!ray_intersects_aabb(&o->t0, &o->t1, &r.p.x, &r.d.x, &a.min.x, &a.max.x))
    return 0;

  o->p = v3add(r.p, v3scale(r.d, min(o->t0,o->t1)));
  ext = v3sub(a.max, a.min);
  c = v3add(a.min, v3scale(ext,0.5f));
  pnt = v3sub(o->p, c);

  min = fabs(ext.x - fabs(pnt.x));
  o->n = v3scale(v3mk(1,0,0), sign(pnt.x));
  d = fabs(ext.y - fabs(pnt.y));

  if (d < min) {
    min = d;
    o->n = v3scale(v3mk(0,1,0), sign(pnt.y));
  }

  d = fabs(ext.z - fabs(pnt.z));

  if (d < min)
    o->n = v3scale(v3mk(0,0,1), sign(pnt.z));

  return 1;
}

static int sphere_test_sphere(sphere a, sphere b) {
  return sphere_intersects_sphere(&a.p.x, a.r, &b.p.x, b.r);
}

static int sphere_test_sphere_manifold(manifold *m, sphere a, sphere b) {
  return sphere_intersects_sphere_manifold(m, &a.p.x, a.r, &b.p.x, b.r);
}

static int sphere_test_aabb(sphere s, aabb a) {
  return sphere_intersect_aabb(&s.p.x, s.r, &a.min.x, &a.max.x);
}

static int sphere_test_aabb_manifold(manifold *m, sphere s, aabb a) {
  return sphere_intersect_aabb_manifold(m, &s.p.x, s.r, &a.min.x, &a.max.x);
}

static int sphere_test_capsule(sphere s, capsule c) {
  return sphere_intersect_capsule(&s.p.x, s.r, &c.a.x, &c.b.x, c.r);
}

static int sphere_test_capsule_manifold(manifold *m, sphere s, capsule c) {
  return sphere_intersect_capsule_manifold(m, &s.p.x, s.r, &c.a.x, &c.b.x, c.r);
}

static aabb aabb_transform(aabb a, const f32 *rot, const v3 t) {
  aabb res;
  aabb_rebalance_transform(&res.min.x, &res.max.x, &a.min.x, &a.max.x, rot, &t.x);
  return res;
}

static v3 aabb_closest_point(aabb a, v3 p) {
  v3 res = {0};
  aabb_closest_point_to_point(&res.x, &a.min.x, &a.max.x, &p.x);
  return res;
}

static int aabb_test_aabb(aabb a, aabb b) {
  return aabb_intersect_aabb(&a.min.x, &a.max.x, &b.min.x, &b.max.x);
}

static int aabb_test_aabb_manifold(manifold *m, aabb a, aabb b) {
  return aabb_intersect_aabb_manifold(m, &a.min.x, &a.max.x, &b.min.x, &b.max.x);
}

static int aabb_test_sphere(aabb a, sphere s) {
  return aabb_intersect_sphere(&a.min.x, &a.max.x, &s.p.x, s.r);
}

static int aabb_test_sphere_manifold(manifold *m, aabb a, sphere s) {
  return aabb_intersect_sphere_manifold(m, &a.min.x, &a.max.x, &s.p.x, s.r);
}

static int aabb_test_capsule(aabb a, capsule c) {
  return aabb_intersect_capsule(&a.min.x, &a.max.x, &c.a.x, &c.b.x, c.r);
}

static int aabb_test_capsule_manifold(manifold *m, aabb a, capsule c) {
  return aabb_intersect_capsule_manifold(m, &a.min.x, &a.max.x, &c.a.x, &c.b.x, c.r);
}

static f32 capsule_sqdist_point(capsule c, v3 p) {
  return capsule_point_sqdist(&c.a.x, &c.b.x, c.r, &p.x);
}

static v3 capsule_closest_point(capsule c, v3 pnt) {
  f32 p[3];
  capsule_closest_point_to_point(p, &c.a.x, &c.b.x, c.r, &pnt.x);
  return v3mkv(p);
}

static int capsule_test_sphere(capsule c, sphere s) {
  return capsule_intersect_sphere(&c.a.x, &c.b.x, c.r, &s.p.x, s.r);
}

static int capsule_test_sphere_manifold(manifold *m, capsule c, sphere s) {
  return capsule_intersect_sphere_manifold(m, &c.a.x, &c.b.x, c.r, &s.p.x, s.r);
}

static int capsule_test_aabb(capsule c, aabb a) {
  return capsule_intersect_aabb(&c.a.x, &c.b.x, c.r, &a.min.x, &a.max.x);
}

static int capsule_test_aabb_manifold(manifold *m, capsule c, aabb a) {
  return capsule_intersect_aabb_manifold(m, &c.a.x, &c.b.x, c.r, &a.min.x, &a.max.x);
}

static int capsule_test_capsule(capsule a, capsule b) {
  return capsule_intersect_capsule(&a.a.x, &a.b.x, a.r, &b.a.x, &b.b.x, b.r);
}

static int capsule_test_capsule_manifold(manifold *m, capsule a, capsule b) {
  return capsule_intersect_capsule_manifold(m, &a.a.x, &a.b.x, a.r, &b.a.x, &b.b.x, b.r);
}

