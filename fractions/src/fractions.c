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

/**
 * @brief Evaluates the sign of the fraction as a number,
 * for convinience minus is `-1` and plus is `1`, should be used
 * in the following conditions `eval_sign(s) * number`.
 * 
 */
#define eval_sign(s) ((s) == mins ? -1 : 1)

/**
 * @brief Binary greatest common divisor of unsigned two numbers.
 * 
 * @param u first unsigned number
 * @param v second unsigned number
 * @return uint32_t the gcd of the numbers
 */
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


/**
 * @brief Checks wheter a fraction is not a number or not.
 * 
 * @param f fraction structure.
 * @return fbool_t ftrue if fraction is not a number, ffalse otherwise.
 */
fbool_t is_fnan(frac_t f) {return ((f.x == (uint32_t)-1) && (f.y == (uint32_t)-1)) || (f.s == nan);}

/**
 * @brief Checks wheter a fraction is infinity fraction or not.
 * 
 * @param f fraction structure.
 * @return fbool_t ftrue if the fraction is infinity fraction, ffalse otherwise.
 */
fbool_t is_finf(frac_t f) {return f.y == 0;}

/**
 * @brief Checks wheter a fraction is positive infinity fraction or not.
 * 
 * @param f fraction structure.
 * @return fbool_t ftrue if the fraction is positive infinity fraction, ffalse otherwise.
 */
fbool_t is_fpinf(frac_t f) {return ((f.y == 0) && (f.s == plus));}

/**
 * @brief Checks wheter a fraction is negative infinity fraction or not.
 * 
 * @param f fraction structure.
 * @return fbool_t ftrue if the fraction is negative infinity fraction, ffalse otherwise.
 */
fbool_t is_fminf(frac_t f) {return ((f.y == 0) && (f.s == mins));}

/**
 * @brief Checks if the fraction is positive or negative.
 * 
 * @param f fraction structure.
 * @return fbool_t ftrue if fraction is positive, ffalse otherwise.
 */
fbool_t is_fpositive(frac_t f) { return f.s == plus; }

/**
 * @brief Checks wheter a fraction is a zero fraction or not.
 * The fraction has to have the x field equal to zero, the y and sign
 * are ignored, by the function.
 *
 * @param f fraction structure.
 * @return fbool_t ftrue if the fraction is a zero fraction, ffalse otherwise.
 */
fbool_t is_fzero(frac_t f) { return (f.x == (uint32_t)0); }

/**
 * @brief Checks wheter a fraction is an identity fraction or not.
 * The fraction has to have the x and y fields equal to 1 and the sign
 * has to be `plus`.
 * 
 * @param f fraction structure.
 * @return fbool_t ftrue if the fraction is an identity fraction, ffalse otherwise.
 */
fbool_t is_fid(frac_t f) { return (f.x == (uint32_t)1) && (f.y == (uint32_t)1) && (f.s == plus);}


/**
 * @brief Evaluates the fraction structure to a double value.
 * 
 * @param f fraction structure to valuate.
 * @return double the real value of the fraction.
 */
double feval(frac_t f) {
  if (is_fnan(f) || is_fminf(f)) {
    return DBL_MIN;
  }
  
  if (is_fpinf(f)) {
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

/**
 * @brief Fraction constructor. Creates a fraction structure,
 * from a numerator, denominator and a sign. If the denominator
 * is zero, the function returns the error fraction. If the sign
 * is not `plus` or `mins` the constructor returns the error fraction.
 * If the numerator is 0, and denumerator is not 0, the function returns
 * the zero fruction, otherwise the function returns the irreductible fraction f = a/b,
 * where a and b are coprime.
 * 
 * @param x numerator.
 * @param y denominator.
 * @param s sign.
 * @return frac_t the fraction structure. 
 */
frac_t fxy(uint32_t x, uint32_t y, sign_t s) {
  if (s == nan) {
    return nan_frac;
  }

  if (y == 0) {
    if (s == plus) {
      return pinf_frac;
    }
    
    return minf_frac;
  }

  if (x == 0) {
    return zero_frac;
  }

  uint32_t simplify = gcd(x, y);

  return (frac_t){(x / simplify), (y / simplify), s};
}


/**
 * @brief Multiplies the fraction with a constant and returns the irreductible fraction f = a/b,
 * where a and b are coprime. The function does the exact operation as the '*' operator.
 * f = c * f1.
 * 
 * @param f1 first fraction.
 * @param c constant value.
 * @return frac_t the product of the constant and fraction, or a infinity fraction
 * if the fraction is error.
 */
frac_t fconst(frac_t f1, int32_t c) {
  if (is_fnan(f1)) {
    return nan_frac;
  }

  if (is_fzero(f1)) {
    return zero_frac;
  }

  frac_t f;
  if (c < 0) {
    f.s = (f1.s + 1) % 2;
    c *= -1;
  } else {
    f.s = f1.s;
  }

  fbool_t is_f1_inf = is_finf(f1);

  if (c == 0) {
    if (is_f1_inf) {
      return nan_frac;
    }

    return zero_frac;
  } else {
    if (is_f1_inf) {
      f.x = f1.x;
      f.y = f1.y;
    } else {
      uint32_t simplify = gcd((uint32_t)c, f1.y);
      f.x = (c / simplify) * f1.x;
      f.y = f1.y / simplify;
    }

    return f;
  }
}

/**
 * @brief Function that handles the rules of adding between a number
 * and infinity, the number may also be infinity. MUST not be used outside
 * this file.
 * 
 * @param f1 first fraction.
 * @param f2 second fraction.
 * @return frac_t nan fraction or one of the infinity.
 */
static frac_t finf_add(frac_t f1, frac_t f2) {
  if ((is_fpinf(f1) && is_fminf(f2)) || (is_fminf(f1) && is_fpinf(f2))) {
    return nan_frac;
  } else if (is_fpinf(f1) || is_fpinf(f2)) {
    return pinf_frac;
  } else {
    return minf_frac;
  }
}

/**
 * @brief Adds two fractions and returns the irreductible fraction f = a/b,
 * where a and b are coprime. The function does the exact operation as the '+' operator.
 * f = f1 + f2.
 * 
 * @param f1 first fraction.
 * @param f2 second fraction.
 * @return frac_t the sum of the two fractions, or a infinity fraction if one of
 * the member is a infinity fraction.
 */
frac_t fadd(frac_t f1, frac_t f2) {
  if (is_fnan(f1) || is_fnan(f2)) {
    return nan_frac;
  }

  if (is_finf(f1) || is_finf(f2)) {
    return finf_add(f1, f2);
  }

  if (is_fzero(f1)) {
    return f2;
  }

  if (is_fzero(f2)) {
    return f1;
  }

  uint32_t common = gcd(f1.y, f2.y);
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

/**
 * @brief Subtracts two fractions and returns the irreductible fraction f = a/b,
 * where a and b are coprime. The function does the exact operation as the '-' operator.
 * f = f1 - f2.
 * 
 * @param f1 first fraction.
 * @param f2 second fraction.
 * @return frac_t the difference of the two fractions, or a infinity fraction if one of
 * the member is a infinity fraction.
 */
frac_t fsub(frac_t f1, frac_t f2) {
  if (is_fnan(f1) || is_fnan(f2)) {
    return nan_frac;
  }

  f2.s = (f2.s + 1) % 2;

  return fadd(f1, f2);
}

/**
 * @brief Function that handles the rules of multiplying between a number
 * and infinity, the number may also be infinity. MUST not be used outside
 * this file.
 * 
 * @param f1 first fraction.
 * @param f2 second fraction.
 * @return frac_t nan fraction or one of the infinity.
 */
static frac_t finf_mul(frac_t f1, frac_t f2) {
  if (is_fzero(f2) || is_fzero(f1)) {
    return nan_frac;
  } else {
    fbool_t is_f1_pos = is_fpositive(f1);
    fbool_t is_f2_pos = is_fpositive(f2);

    if ((is_f1_pos && is_f2_pos) || (!is_f1_pos && !is_f2_pos)) {
      return pinf_frac;
    } else {
      return minf_frac;
    }
  }
}

/**
 * @brief Multiplies two fractions and returns the irreductible fraction f = a/b,
 * where a and b are coprime. The function does the exact operation as the '*' operator.
 * f = f1 * f2.
 * 
 * @param f1 first fraction.
 * @param f2 second fraction.
 * @return frac_t the product of the two fractions, or a infinity fraction if one of
 * the member is a infinity fraction.
 */
frac_t fmul(frac_t f1, frac_t f2) {
  if (is_fnan(f1) || is_fnan(f2)) {
    return nan_frac;
  }

  if (is_finf(f1) || is_finf(f2)) {
    return finf_mul(f1, f2);
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

/**
 * @brief Function that handles the rules of multiplying between a number
 * and infinity, the number may also be infinity. MUST not be used outside
 * this file.
 * 
 * @param f1 first fraction.
 * @param f2 second fraction.
 * @return frac_t nan fraction, zero fraction or one of the infinity.
 */
static frac_t finf_div(frac_t f1, frac_t f2) {
  if (is_finf(f1) && is_finf(f2)) {
    return nan_frac;
  } else if (is_finf(f1)) {
    if (is_fpositive(f2)) {
      return f1;
    }

    f1.s = (f1.s + 1) % 2;
    return f1;
  } else {
    return zero_frac;
  }
}

/**
 * @brief Divides two fractions and returns the irreductible fraction f = a/b,
 * where a and b are coprime. The function does the exact operation as the '/' operator.
 * f = f1 / f2 = f1 * (1/f2).
 * 
 * @param f1 first fraction.
 * @param f2 second fraction.
 * @return frac_t the division of the two fractions, or a infinity fraction if one of
 * the member is a infinity fraction.
 */
frac_t fdiv(frac_t f1, frac_t f2) {
  if (is_fnan(f1) || is_fnan(f2)) {
    return nan_frac;
  }

  if (is_finf(f1) || is_finf(f2)) {
    return finf_div(f1, f2);
  }

  if(is_fzero(f1) && is_fzero(f2)) {
    return nan_frac;
  } else if (is_fzero(f2)) {
    if (is_fpositive(f1)) {
      return pinf_frac;
    }

    return minf_frac;
  } else if (is_fzero(f1)) {
    return zero_frac;
  }

  uint32_t temp = f2.x;
  f2.x = f2.y;
  f2.y = temp;

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


/**
 * @brief Multiplies the fraction with a constant and stores the irreductible
 * fraction f = a/b in the first fraction, where a and b are coprime.
 * The function does the exact operation as the '*=' operator.
 * f1 *= c.
 * 
 * @param f1 pointer to first fraction.
 * @param c constant value.
 *
 */
void fconstp(frac_t *f1, int32_t c) {
  *f1 = fconst(*f1, c);
}

/**
 * @brief Adds two fractions and stores the irreductible fraction f = a/b in the first fraction,
 * where a and b are coprime. The function does the exact operation as the '+=' operator.
 * f1 += f2.
 * 
 * @param f1 pointer to first fraction.
 * @param f2 second fraction.
 *
 */
void faddp(frac_t *f1, frac_t f2) {
  *f1 = fadd(*f1, f2);
}

/**
 * @brief Subtracts two fractions and stores the irreductible fraction f = a/b in the first fraction,
 * where a and b are coprime. The function does the exact operation as the '-=' operator.
 * f1 -= f2.
 * 
 * @param f1 pointer to first fraction.
 * @param f2 second fraction.
 *
 */
void fsubp(frac_t *f1, frac_t f2) {
  *f1 = fsub(*f1, f2);
}

/**
 * @brief Multiplies two fractions and stores the irreductible fraction f = a/b in the first fraction,
 * where a and b are coprime. The function does the exact operation as the '*=' operator.
 * f1 *= f2.
 * 
 * @param f1 pointer to first fraction.
 * @param f2 second fraction.
 *
 */
void fmulp(frac_t *f1, frac_t f2) {
  *f1 = fmul(*f1, f2);
}

/**
 * @brief Divides two fractions and stores the irreductible fraction f = a/b in the first fraction,
 * where a and b are coprime. The function does the exact operation as the '/=' operator.
 * f1 /= f2.
 * 
 * @param f1 pointer to first fraction.
 * @param f2 second fraction.
 *
 */
void fdivp(frac_t *f1, frac_t f2) {
  *f1 = fdiv(*f1, f2);
}


/**
 * @brief Checks if two fractions are equal. Because all the fractions
 * are represented as ireductible fractions, the function checks if the
 * numerators, denominators and signs are equal. The `nan` and `infinities`
 * fraction will always generate the `funknown` answer for indeterminate states.
 * f1 == f2
 * 
 * @param f1 first fraction.
 * @param f2 second fraction.
 * @return fbool_t ftrue if the fractions are equal, ffalse otherwise.
 */
fbool_t feq(frac_t f1, frac_t f2) {
  if (is_fnan(f1) || is_fnan(f2)) {
    return funknown;
  }

  if (is_finf(f1) || is_finf(f2)) {
    if ((is_fpinf(f1) && is_fpinf(f2)) || (is_fminf(f1) && is_fminf(f2))) {
      return funknown;
    } else {
      return ffalse;
    }
  }

  return (f1.s == f2.s) && (f1.x == f2.x) && (f1.y == f2.y);
}


/**
 * @brief Checks if two fractions are not equal. Because all the fractions
 * are represented as ireductible fractions, the function checks if the
 * numerators, denominators or signs are not equal. The `nan` and `infinities`
 * fraction will always generate the `funknown` answer for indeterminate states.
 * f1 != f2
 * 
 * @param f1 first fraction.
 * @param f2 second fraction.
 * @return fbool_t ftrue if the fractions are not equal, ffalse otherwise.
 */
fbool_t fneq(frac_t f1, frac_t f2) {
  if (is_fnan(f1) || is_fnan(f2)) {
    return funknown;
  }

  if (is_finf(f1) || is_finf(f2)) {
    if ((is_fpinf(f1) && is_fpinf(f2)) || (is_fminf(f1) && is_fminf(f2))) {
      return funknown;
    } else {
      return ftrue;
    }
  }

  return (f1.s != f2.s) || (f1.x != f2.x) || (f1.y != f2.y);
}

/**
 * @brief Compares two fractions with the "greater than" operation.
 * The `nan` and `infinities` fraction will always generate the
 * `funknown` answer for indeterminate states.
 * f1 > f2
 * 
 * @param f1 first fraction
 * @param f2 second fraction
 * @return fbool_t ftrue if the first fraction is greater than second, ffalse otherwise.
 */
fbool_t fgt(frac_t f1, frac_t f2) {
  if (is_fnan(f1) || is_fnan(f2)) {
    return funknown;
  }

  if (is_finf(f1) || is_finf(f2)) {
    if ((is_fpinf(f1) && is_fpinf(f2)) || (is_fminf(f1) && is_fminf(f2))) {
      return funknown;
    } else if (is_fpinf(f1) || is_fminf(f2)) {
      return ftrue;
    } else {
      return ffalse;
    }
  }

  if (f1.s == plus && f2.s == mins) {
    return ftrue;
  } else if (f1.s == mins && f2.s == plus) {
    return ffalse;
  } else {
    uint32_t common = gcd(f1.y, f2.y);
    uint32_t x1 = f1.x * (f2.y / common);
    uint32_t x2 = f2.x * (f1.y / common);

    if (f1.s == plus && f2.s == plus) {
      return x1 > x2;
    } else {
      return x1 < x2;
    }
  } 
}

/**
 * @brief Compares two fractions with the "less than" operation.
 * The `nan` and `infinities` fraction will always generate the
 * `funknown` answer for indeterminate states.
 * f1 < f2
 * 
 * @param f1 first fraction
 * @param f2 second fraction
 * @return fbool_t ftrue if the first fraction is less than second, ffalse otherwise.
 */
fbool_t flt(frac_t f1, frac_t f2) {
  if (is_fnan(f1) || is_fnan(f2)) {
    return funknown;
  }

  if (is_finf(f1) || is_finf(f2)) {
    if ((is_fpinf(f1) && is_fpinf(f2)) || (is_fminf(f1) && is_fminf(f2))) {
      return funknown;
    } else if (is_fpinf(f1) || is_fminf(f2)) {
      return ffalse;
    } else {
      return ftrue;
    }
  }

  if (f1.s == plus && f2.s == mins) {
    return ffalse;
  } else if (f1.s == mins && f2.s == plus) {
    return ftrue;
  } else {
    uint32_t common = gcd(f1.y, f2.y);
    uint32_t x1 = f1.x * (f2.y / common);
    uint32_t x2 = f2.x * (f1.y / common);

    if (f1.s == plus && f2.s == plus) {
      return x1 < x2;
    } else {
      return x1 > x2;
    }
  } 
}

/**
 * @brief Compares two fractions with the "greater than or equal to" operation.
 * The `nan` and `infinities` fraction will always generate the
 * `funknown` answer for indeterminate states.
 * f1 >= f2
 * 
 * @param f1 first fraction
 * @param f2 second fraction
 * @return fbool_t ftrue if the first fraction is greater than or equal to second,
 * ffalse otherwise.
 */
fbool_t fgte(frac_t f1, frac_t f2) {
  if (is_fnan(f1) || is_fnan(f2)) {
    return funknown;
  }

  if (is_finf(f1) || is_finf(f2)) {
    if ((is_fpinf(f1) && is_fpinf(f2)) || (is_fminf(f1) && is_fminf(f2))) {
      return funknown;
    } else if (is_fpinf(f1) || is_fminf(f2)) {
      return ftrue;
    } else {
      return ffalse;
    }
  }

  if (f1.s == plus && f2.s == mins) {
    return ftrue;
  } else if (f1.s == mins && f2.s == plus) {
    return ffalse;
  } else {
    uint32_t common = gcd(f1.y, f2.y);
    uint32_t x1 = f1.x * (f2.y / common);
    uint32_t x2 = f2.x * (f1.y / common);

    if (f1.s == plus && f2.s == plus) {
      return x1 >= x2;
    } else {
      return x1 <= x2;
    }
  }  
}

/**
 * @brief Compares two fractions with the "less than or equal to" operation.
 * The `nan` and `infinities` fraction will always generate the
 * `funknown` answer for indeterminate states.
 * f1 <= f2
 * 
 * @param f1 first fraction
 * @param f2 second fraction
 * @return fbool_t ftrue if the first fraction is less than or equal to second,
 * ffalse otherwise.
 */
fbool_t flte(frac_t f1, frac_t f2) {
  if (is_fnan(f1) || is_fnan(f2)) {
    return funknown;
  }

  if (is_finf(f1) || is_finf(f2)) {
    if ((is_fpinf(f1) && is_fpinf(f2)) || (is_fminf(f1) && is_fminf(f2))) {
      return funknown;
    } else if (is_fpinf(f1) || is_fminf(f2)) {
      return ffalse;
    } else {
      return ftrue;
    }
  }

  if (f1.s == plus && f2.s == mins) {
    return ffalse;
  } else if (f1.s == mins && f2.s == plus) {
    return ftrue;
  } else {
    uint32_t common = gcd(f1.y, f2.y);
    uint32_t x1 = f1.x * (f2.y / common);
    uint32_t x2 = f2.x * (f1.y / common);

    if (f1.s == plus && f2.s == plus) {
      return x1 <= x2;
    } else {
      return x1 >= x2;
    }
  } 
}
