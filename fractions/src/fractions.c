/**
 * @file fractions.c
 * @author Mihai Negru (determinant289@gmail.com)
 * @version 1.0.0
 * @date 2023-06-26
 *
 * @copyright Copyright (C) 2022-2023 Mihai Negru <determinant289@gmail.com>
 * This file is part of c-language-data-structures.
 *
 * c-language-data-structures is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * c-language-data-structures is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with c-language-data-structures.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

#include "./fractions.h"

#define eval_sign(s) ((s) == mins ? -1 : 1)

static uint32_t gcd(uint32_t u, uint32_t v) {
  if (u == 0) {
    return v;
  }

  if (v == 0) {
    return u;
  }

  int32_t shift = __builtin_ctz(u | v);
  u >>= __builtin_ctz(u);

  uint32_t m;
  do {
    v >>= __builtin_ctz(v);
    v -= u;
    m = (int32_t)v >> 31;
    u += v & m;
    v = (v + m) ^ m;
  } while (v != 0);

  return (u << shift);
}

uint8_t is_ferror(frac_t f) {
  return ((f.x == (uint32_t)-1) && (f.y == (uint32_t)-1)) || (f.s == erro);
}

uint8_t is_fzero(frac_t f) { return (f.x == (uint32_t)0); }

uint8_t is_fid(frac_t f) { return (f.x == (uint32_t)1) && (f.y == (uint32_t)1) && (f.s == plus);}

double feval(frac_t f) {
  if (is_ferror(f)) {
    return DBL_MAX;
  }

  if (is_fzero(f)) {
    return 0.0f;
  }

  if (is_fid(f)) {
    return 1.0f;
  }

  return 1.0f * eval_sign(f.s) * f.x / f.y;
}

frac_t fxy(uint32_t x, uint32_t y, sign_t s) {
  if (y == 0 || s == erro) {
    return error_frac;
  }

  if (x == 0) {
    return zero_frac;
  }

  uint32_t simplify = gcd(x, y);

  return (frac_t){(x / simplify), (y / simplify), s};
}

frac_t fadd(frac_t f1, frac_t f2) {
  if (is_ferror(f1) || is_ferror(f2)) {
    return error_frac;
  }

  if (is_fzero(f1)) {
    return f2;
  }

  if (is_fzero(f2)) {
    return f1;
  }

  uint32_t common = gcd(f1.y, f2.y); // common = ab/gcd(a,b) = a * c
  uint32_t amplify_f1 = f2.y / common;
  uint32_t amplify_f2 = f1.y / common;

  f1.x *= amplify_f1;
  f2.x *= amplify_f2;

  frac_t f;
  f.y = f1.y * amplify_f1;
  if (f1.s == mins && f2.s == mins) {
    f.x = f1.x + f2.x;
    f.s = mins;
  } else if (f1.s == mins) {
    if (f1.x > f2.x) {
      f.x = f1.x - f2.x;
      f.s = mins;
    } else {
      f.x = f2.x - f1.x;
      f.s = plus;
    }
  } else if (f2.s == mins) {
    if (f1.x >= f2.x) {
      f.x = f1.x - f2.x;
      f.s = plus;
    } else {
      f.x = f2.x - f1.x;
      f.s = mins;
    }
  } else {
    f.x = f1.x + f2.x;
    f.s = plus;
  }

  if (f.x == 0) {
    f = zero_frac;
  }

  return f;
}

frac_t fsub(frac_t f1, frac_t f2) {
  if (is_ferror(f1) || is_ferror(f2)) {
    return error_frac;
  }

  f2.s = (f2.s + 1) % 2;

  return fadd(f1, f2);
}

frac_t fmul(frac_t f1, frac_t f2) {
  if (is_ferror(f1) || is_ferror(f2)) {
    return error_frac;
  }

  if (is_fzero(f1) || is_fzero(f2)) {
    return zero_frac;
  }

  if (is_fid(f1)) {
    return f2;
  }

  if (is_fid(f2)) {
    return f1;
  }

  uint32_t g1 = gcd(f1.x, f2.y);
  uint32_t g2 = gcd(f2.x, f1.y);

  frac_t f;
  f.x = (f1.x / g1) * (f2.x / g2);
  f.y = (f1.y / g2) * (f2.y / g1);
  
  if ((f1.s == mins && f2.s == mins) || (f1.s == plus && f2.s == plus)) {
    f.s = plus;
  } else {
    f.s = mins;
  }

  return f;
}

frac_t fdiv(frac_t f1, frac_t f2) {
  if (is_fzero(f2)) {
    return error_frac;
  }

  uint32_t temp = f2.x;
  f2.x = f2.y;
  f2.y = temp;

  return fmul(f1, f2);
}

frac_t fconst(frac_t f1, int32_t c) {
  if (is_ferror(f1)) {
    return error_frac;
  }

  if (c == 0) {
    return zero_frac;
  }

  frac_t f;
  if (c < 0) {
    f.s = (f1.s + 1) % 2;
    c *= -1;
  } else {
    f.s = f1.s;
  }

  uint32_t simplify = gcd((uint32_t)c, f1.y);
  f.x = (c / simplify) * f1.x;
  f.y = f1.y / simplify;

  return f;
}

void fconstp(frac_t *f1, int32_t c) {
  *f1 = fconst(*f1, c);
}


void faddp(frac_t *f1, frac_t f2) {
  *f1 = fadd(*f1, f2);
}

void fsubp(frac_t *f1, frac_t f2) {
  *f1 = fsub(*f1, f2);
}

void fmulp(frac_t *f1, frac_t f2) {
  *f1 = fmul(*f1, f2);
}

void fdivp(frac_t *f1, frac_t f2) {
  *f1 = fdiv(*f1, f2);
}

uint8_t feq(frac_t f1, frac_t f2) {
  return (f1.s == f2.s) && (f1.x == f2.x) && (f1.y == f2.y);
}

uint8_t fneq(frac_t f1, frac_t f2) {
  return (f1.s != f2.s) || (f1.x != f2.x) || (f1.y != f2.y);
}

uint8_t fgt(frac_t f1, frac_t f2) {
  if (feq(f1, f2) || is_ferror(f2)) {
    return 0;
  }

  if (is_ferror(f1)) {
    return 1;
  }

  uint32_t common = gcd(f1.y, f2.y);

  return (eval_sign(f1.s) * f1.x * (f2.y / common)) > (eval_sign(f2.s) * f2.x * (f1.y / common)); 
}

uint8_t flt(frac_t f1, frac_t f2) {
  if (feq(f1, f2) || is_ferror(f1)) {
    return 0;
  }

  if (is_ferror(f2)) {
    return 1;
  }

  uint32_t common = gcd(f1.y, f2.y);

  return (eval_sign(f1.s) * f1.x * (f2.y / common)) < (eval_sign(f2.s) * f2.x * (f1.y / common));
}

uint8_t fgte(frac_t f1, frac_t f2) {
  if (is_ferror(f2)) {
    return 0;
  }

  if (is_ferror(f1)) {
    return 1;
  }

  uint32_t common = gcd(f1.y, f2.y);

  return (eval_sign(f1.s) * f1.x * (f2.y / common)) >= (eval_sign(f2.s) * f2.x * (f1.y / common)); 
}

uint8_t flte(frac_t f1, frac_t f2) {
  if (is_ferror(f1)) {
    return 0;
  }

  if (is_ferror(f2)) {
    return 1;
  }

  uint32_t common = gcd(f1.y, f2.y);

  return (eval_sign(f1.s) * f1.x * (f2.y / common)) <= (eval_sign(f2.s) * f2.x * (f1.y / common));
}
