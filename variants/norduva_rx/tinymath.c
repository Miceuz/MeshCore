// Small replacements for libc strtof/atof and libm pow/powf, linked in with -Wl,--wrap
// (see size_cuts.py). Saves ~15k on 128k parts. No exponent notation in strtof;
// powf max rel. error ~1e-3, enough for CLI parsing and retransmit delay scoring.
#ifdef NORDUVA_RX_SIZE_CUTS

#include <stdint.h>
#include <math.h>

__attribute__((used)) float __wrap_strtof(const char* s, char** end) {
  float v = 0.0f, scale = 1.0f;
  int neg = 0, frac = 0;
  while (*s == ' ' || *s == '\t') s++;
  if (*s == '-') { neg = 1; s++; } else if (*s == '+') s++;
  for (; *s; s++) {
    if (*s >= '0' && *s <= '9') {
      if (frac) { scale *= 0.1f; v += (*s - '0') * scale; }
      else v = v * 10.0f + (*s - '0');
    } else if (*s == '.' && !frac) {
      frac = 1;
    } else break;
  }
  if (end) *end = (char*)s;
  return neg ? -v : v;
}

__attribute__((used)) double __wrap_atof(const char* s) { return __wrap_strtof(s, 0); }

static float tiny_log2f(float x) {
  union { float f; uint32_t i; } u = { x };
  float e = (float)(int)(((u.i >> 23) & 0xFF) - 127);
  u.i = (u.i & 0x007FFFFF) | 0x3F800000;   // mantissa in [1,2)
  float m = u.f;
  float t = (m - 1.0f) / (m + 1.0f), t2 = t * t;   // ln(m) = 2*atanh(t)
  return e + 2.8853901f * t * (1.0f + t2 * (0.33333333f + t2 * (0.2f + t2 * 0.14285714f)));
}

static float tiny_exp2f(float x) {
  if (x < -126.0f) return 0.0f;
  if (x > 127.0f) return INFINITY;
  int i = (int)x; if (x < i) i--;
  float f = x - i;
  union { float f; uint32_t i; } u;
  u.i = (uint32_t)(i + 127) << 23;
  return u.f * (1.0f + f * (0.6931472f + f * (0.2402265f + f * (0.0555041f + f * 0.0096181f))));
}

__attribute__((used)) float __wrap_powf(float b, float e) {
  if (b <= 0.0f) return b == 0.0f ? 0.0f : NAN;
  return tiny_exp2f(e * tiny_log2f(b));
}

__attribute__((used)) double __wrap_pow(double b, double e) { return __wrap_powf((float)b, (float)e); }

#endif
