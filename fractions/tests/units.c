/**
 * @file units.c
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

#include <assert.h>
#include <string.h>
#include "../src/fractions.h"

/**
 * @brief Checks a raw equality of a fraction to another.
 * 
 */
#define cnd(f1, f2) (f1.x == f2.x && f1.y == f2.y && f1.s == f2.s)

/**
 * @brief Prints the header of the test.
 * 
 * @param msg the header title.
 */
void print_header(const char *const msg) {
  size_t msg_len = strlen(msg);
  size_t dots = (44 - msg_len) / 2;

  for (size_t dot = 0; dot < dots; ++dot) {
    fprintf(stderr, ".");
  }

  fprintf(stderr, " \033[0;32m%s\033[0m ", msg);
  
  for (size_t dot = 0; dot < dots; ++dot) {
    fprintf(stderr, ".");
  }

  if (msg_len & 1) {
    fprintf(stderr, ".");
  }

  fprintf(stderr, "\n");
}

/**
 * @brief Print the footer of a test.
 * 
 */
void print_footer(void) {
  for (size_t dot = 0; dot < 46; ++dot) {
    fprintf(stderr, ".");
  }
  
  fprintf(stderr, "\n\n");
}

/**
 * @brief Checks if the condition is true and prints passed
 * or failed on the `stderr`.
 * 
 * @param msg 
 * @param cond 
 */
void assert_frac(const char *const msg, fbool_t cond) {
  size_t msg_len = strlen(msg);
  size_t dots = 38 - msg_len;

  fprintf(stderr, "%s ", msg);
  for (size_t dot = 0; dot < dots; ++dot) {
    fprintf(stderr, ".");
  }
  fprintf(stderr,
          " %s\n",
          cond == ftrue ? "\033[0;32mpassed\033[0m" : "\033[0;31mfailed\033[0m"
  );
}

/**
 * @brief Test `is_finf` method.
 * 
 */
void test_is_finf(void) {
  print_header("is_fpinf");

  frac_t a = (frac_t){1, 2, plus};

  assert_frac("inf is infinity", is_finf(pinf_frac));
  assert_frac("-inf is infinity", is_finf(minf_frac));
  assert_frac("-inf != inf", !cnd(minf_frac, pinf_frac));
  assert_frac("inf != -inf", !cnd(pinf_frac, minf_frac));
  assert_frac("a is not infinity", !is_finf(a));
  assert_frac("nan is not infinity", !is_finf(nan_frac));
  assert_frac("zero is not infinity", !is_finf(zero_frac));
  assert_frac("is is not infinity", !is_finf(id_frac));

  print_footer();
}

/**
 * @brief Test `is_fzero` method.
 * 
 */
void test_is_fzero(void) {
  print_header("is_fzero");

  frac_t a = (frac_t){1, 2, plus};
  frac_t b = (frac_t){0, 2, plus};
  frac_t _b = (frac_t){0, 2, mins};
  frac_t mzero = (frac_t){0, 1, mins};

  assert_frac("zero is zero", is_fzero(zero_frac));
  assert_frac("-zero is zero", is_fzero(mzero));
  assert_frac("a is not zero", !is_fzero(a));
  assert_frac("id is not zero", !is_fzero(id_frac));
  assert_frac("inf is not zero", !is_fzero(pinf_frac));
  assert_frac("-inf is not zero", !is_fzero(minf_frac));
  assert_frac("0/b is zero", is_fzero(b));
  assert_frac("-0/b is zero", is_fzero(_b));

  print_footer();
}

/**
 * @brief Test `is_fid` method.
 * 
 */
void test_is_fid(void) {
  print_header("is_fid");

  frac_t a = (frac_t){1, 2, plus};
  frac_t b = (frac_t){1, 1, plus};
  frac_t mid = (frac_t){1, 1, mins};

  assert_frac("id is identity", is_fid(id_frac));
  assert_frac("-id is not identity", !is_fid(mid));
  assert_frac("a is not identity", !is_fid(a));
  assert_frac("zero is not identity", !is_fid(zero_frac));
  assert_frac("inf is not identity", !is_fid(pinf_frac));
  assert_frac("-inf is not identity", !is_fid(minf_frac));
  assert_frac("1/1 is identity", is_fid(b));

  print_footer();
}

/**
 * @brief Test `fxy` method.
 * 
 */
void test_fxy(void) {
  print_header("fxy");

  assert_frac("1/1 = id", is_fid(fxy(1, 1, plus)));
  assert_frac("-1/1 != id", !cnd(fxy(1, 1, mins), id_frac));
  assert_frac("0/1 = zero", is_fzero(fxy(0, 1, plus)));
  assert_frac("-0/1 = zero", is_fzero(fxy(0, 1, mins)));
  assert_frac("1/0 = inf", is_fpinf(fxy(1, 0, plus)));
  assert_frac("-1/0 = -inf", is_fminf(fxy(1, 0, mins)));
  assert_frac("nan = nan", is_fnan(fxy(1, 4, nans)));
  assert_frac("24/6 = 4", cnd(fxy(24, 6, plus), fxy(4, 1, plus)));
  assert_frac("6/24 = 1/4", cnd(fxy(6, 24, plus), fxy(1, 4, plus)));
  assert_frac("1/7 is ireductible", cnd(fxy(1, 7, plus), fxy(1, 7, plus)));

  print_footer();
}

/**
 * @brief Test `fmconst` method.
 * 
 */
void test_fmconst(void) {
  print_header("fmconst");

  frac_t a = fxy(1, 3, plus);
  frac_t _a = fxy(1, 3, mins);
  int32_t c = 7;

  assert_frac("c * nan = nan", is_fnan(fmconst(nan_frac, c)));
  assert_frac("-c * nan = nan", is_fnan(fmconst(nan_frac, -c)));
  assert_frac("c * inf = inf", is_fpinf(fmconst(pinf_frac, c)));
  assert_frac("(-c) * inf = -inf", is_fminf(fmconst(pinf_frac, -c)));
  assert_frac("c * (-inf) = -inf", is_fminf(fmconst(minf_frac, c)));
  assert_frac("(-c) * (-inf) = inf", is_fpinf(fmconst(minf_frac, -c)));
  assert_frac("c * zero = zero", is_fzero(fmconst(zero_frac, c)));
  assert_frac("(-c) * zero = zero", is_fzero(fmconst(zero_frac, -c)));
  assert_frac("c * a = (c*a)", cnd(fmconst(a, c), fxy(7, 3, plus)));
  assert_frac("(-c) * a = -(c*a)", cnd(fmconst(a, -c), fxy(7, 3, mins)));
  assert_frac("0 * a = zero", is_fzero(fmconst(a, 0)));
  assert_frac("0 * (-a) = zero", is_fzero(fmconst(_a, 0)));
  assert_frac("0 * inf = nan", is_fnan(fmconst(pinf_frac, 0)));
  assert_frac("0 * (-inf) = nan", is_fnan(fmconst(minf_frac, 0)));
  assert_frac("c * id = (c*id)", cnd(fmconst(id_frac, c), fxy(c, 1, plus)));
  assert_frac("(-c) * id = -(c*id)", cnd(fmconst(id_frac, -c), fxy(c, 1, mins)));

  print_footer();
}

/**
 * @brief Test `fdconst` method.
 * 
 */
void test_fdconst(void) {
  print_header("fdconst");

  frac_t a = fxy(1, 3, plus);
  frac_t _a = fxy(1, 3, mins);
  int32_t c = 7;

  assert_frac("nan / c = nan", is_fnan(fdconst(nan_frac, c)));
  assert_frac("nan / (-c) = nan", is_fnan(fdconst(nan_frac, -c)));
  assert_frac("inf / c = inf", is_fpinf(fdconst(pinf_frac, c)));
  assert_frac("inf / (-c) = -inf", is_fminf(fdconst(pinf_frac, -c)));
  assert_frac("(-inf) / c = -inf", is_fminf(fdconst(minf_frac, c)));
  assert_frac("(-inf) / (-c) = inf", is_fpinf(fdconst(minf_frac, -c)));
  assert_frac("zero / c = zero", is_fzero(fdconst(zero_frac, c)));
  assert_frac("zero / (-c) = zero", is_fzero(fdconst(zero_frac, -c)));
  assert_frac("a / c = (a/c)", cnd(fdconst(a, c), fxy(1, 21, plus)));
  assert_frac("a / (-c) = -(a/c)", cnd(fdconst(a, -c), fxy(1, 21, mins)));
  assert_frac("a / 0 = inf", is_fpinf(fdconst(a, 0)));
  assert_frac("(-a) / 0 = -inf", is_fminf(fdconst(_a, 0)));
  assert_frac("inf / 0 = inf", is_fpinf(fdconst(pinf_frac, 0)));
  assert_frac("(-inf) / 0 = -inf", is_fminf(fdconst(minf_frac, 0)));
  assert_frac("id / c = (id/c)", cnd(fdconst(id_frac, c), fxy(1, c, plus)));
  assert_frac("id / (-c) = -(id/c)", cnd(fdconst(id_frac, -c), fxy(1, c, mins)));

  print_footer();
}

/**
 * @brief Test `fadd` method.
 * 
 */
void test_fadd(void) {
  print_header("fadd");

  frac_t a = fxy(1, 3, plus);
  frac_t _a = fxy(1, 3, mins);
  frac_t b = fxy(1, 2, plus);
  frac_t c = fxy(7, 4, plus);

  assert_frac("nan + nan = nan", is_fnan(fadd(nan_frac, nan_frac)));
  assert_frac("nan + a = nan", is_fnan(fadd(nan_frac, a)));
  assert_frac("a + nan = nan", is_fnan(fadd(a, nan_frac)));
  assert_frac("inf + inf = inf", is_fpinf(fadd(pinf_frac, pinf_frac)));
  assert_frac("inf + (-inf) = nan", is_fnan(fadd(pinf_frac, minf_frac)));
  assert_frac("(-inf) + inf = nan", is_fnan(fadd(minf_frac, pinf_frac)));
  assert_frac("inf + a = inf", is_fpinf(fadd(pinf_frac, a)));
  assert_frac("a + inf = inf", is_fpinf(fadd(a, pinf_frac)));
  assert_frac("(-inf) + a = -inf", is_fminf(fadd(minf_frac, a)));
  assert_frac("a + (-inf) = -inf", is_fminf(fadd(a, minf_frac)));
  assert_frac("zero + a = a", cnd(fadd(zero_frac, a), a));
  assert_frac("zero + (-a) = -a", cnd(fadd(zero_frac, _a), _a));
  assert_frac("a + zero = a", cnd(fadd(a, zero_frac), a));
  assert_frac("(-a) + zero = -a", cnd(fadd(_a, zero_frac), _a));
  assert_frac("a + a = 2a", cnd(fadd(a, a), fmconst(a, 2)));
  assert_frac("a + (-a) = zero", cnd(fadd(a, _a), zero_frac));
  assert_frac("(-a) + (-a) = -2a", cnd(fadd(_a, _a), fmconst(a, -2)));
  assert_frac("a + b = b + a", cnd(fadd(a, b), fadd(b, a)));
  assert_frac("(a+b)+c = a+(b+c)", cnd(fadd(fadd(a, b), c), fadd(a, fadd(b, c))));
  assert_frac("1/3 + 7/4 = 25/12", cnd(fadd(a, c), fxy(25, 12, plus)));
  assert_frac("-1/3 + 1/2 = 1/6", cnd(fadd(_a, b), fxy(1, 6, plus)));

  print_footer();
}

/**
 * @brief Test `fsub` method.
 * 
 */
void test_fsub(void) {
  print_header("fsub");

  frac_t a = fxy(1, 3, plus);
  frac_t _a = fxy(1, 3, mins);
  frac_t b = fxy(1, 2, plus);
  frac_t c = fxy(7, 4, plus);

  assert_frac("nan - nan = nan", is_fnan(fsub(nan_frac, nan_frac)));
  assert_frac("nan - a = nan", is_fnan(fsub(nan_frac, a)));
  assert_frac("a - nan = nan", is_fnan(fsub(a, nan_frac)));
  assert_frac("inf - inf = nan", is_fnan(fsub(pinf_frac, pinf_frac)));
  assert_frac("inf - (-inf) = inf", is_fpinf(fsub(pinf_frac, minf_frac)));
  assert_frac("(-inf) - inf = -inf", is_fminf(fsub(minf_frac, pinf_frac)));
  assert_frac("inf - a = inf", is_fpinf(fsub(pinf_frac, a)));
  assert_frac("a - inf = -inf", is_fminf(fsub(a, pinf_frac)));
  assert_frac("(-inf) - a = -inf", is_fminf(fsub(minf_frac, a)));
  assert_frac("a - (-inf) = inf", is_fpinf(fsub(a, minf_frac)));
  assert_frac("zero - inf = -inf", is_fminf(fsub(zero_frac, pinf_frac)));
  assert_frac("inf - zero = inf", is_fpinf(fsub(pinf_frac, zero_frac)));
  assert_frac("zero - (-inf) = inf", is_fpinf(fsub(zero_frac, minf_frac)));
  assert_frac("(-inf) - zero = -inf", is_fminf(fsub(minf_frac, zero_frac)));
  assert_frac("zero - a = -a", cnd(fsub(zero_frac, a), _a));
  assert_frac("zero - (-a) = a", cnd(fsub(zero_frac, _a), a));
  assert_frac("a - zero = a", cnd(fsub(a, zero_frac), a));
  assert_frac("(-a) - zero = -a", cnd(fsub(_a, zero_frac), _a));
  assert_frac("a - a = zero", cnd(fsub(a, a), zero_frac));
  assert_frac("(-a) - (-a) = zero", cnd(fsub(_a, _a), zero_frac));
  assert_frac("(-a) - a = -2a", cnd(fsub(_a, a), fmconst(a, -2)));
  assert_frac("a - (-a) = 2a", cnd(fsub(a, _a), fmconst(a, 2)));
  assert_frac("a - b != b - a", !cnd(fsub(a, b), fsub(b, a)));
  assert_frac("(a-b)-c = a-(b+c)", cnd(fsub(fsub(a, b), c), fsub(a, fadd(b, c))));
  assert_frac("1/3 - 7/4 = -17/12", cnd(fsub(a, c), fxy(17, 12, mins)));
  assert_frac("_1/3 - 7/4 = -25/12", cnd(fsub(_a, c), fxy(25, 12, mins)));

  print_footer();
}

/**
 * @brief Test `fmul` method.
 * 
 */
void test_fmul(void) {
  print_header("fmul");

  frac_t a = fxy(4, 3, plus);
  frac_t _a = fxy(4, 3, mins);
  frac_t a_over_1 = fxy(3, 4, plus);
  frac_t _a_over_1 = fxy(3, 4, mins);
  frac_t b = fxy(1, 2, plus);
  frac_t c = fxy(7, 4, plus);

  assert_frac("nan * nan = nan", is_fnan(fmul(nan_frac, nan_frac)));
  assert_frac("nan * a = nan", is_fnan(fmul(nan_frac, a)));
  assert_frac("a * nan = nan", is_fnan(fmul(a, nan_frac)));
  assert_frac("inf * inf = inf", is_fpinf(fmul(pinf_frac, pinf_frac)));
  assert_frac("(-inf) * inf = -inf", is_fminf(fmul(minf_frac, pinf_frac)));
  assert_frac("inf * (-inf) = -inf", is_fminf(fmul(pinf_frac, minf_frac)));
  assert_frac("(-inf) * (-inf) = inf", is_fpinf(fmul(minf_frac, minf_frac)));
  assert_frac("zero * inf = nan", is_fnan(fmul(zero_frac, pinf_frac)));
  assert_frac("zero * -inf = nan", is_fnan(fmul(zero_frac, minf_frac)));
  assert_frac("inf * zero = nan", is_fnan(fmul(pinf_frac, zero_frac)));
  assert_frac("(-inf) * zero = nan", is_fnan(fmul(minf_frac, zero_frac)));
  assert_frac("a * inf = inf", is_fpinf(fmul(a, pinf_frac)));
  assert_frac("a * (-inf) = -inf", is_fminf(fmul(a, minf_frac)));
  assert_frac("(-a) * inf = -inf", is_fminf(fmul(_a, pinf_frac)));
  assert_frac("(-a) * (-inf) = inf", is_fpinf(fmul(_a, minf_frac)));
  assert_frac("zero * a = zero", cnd(fmul(zero_frac, a), zero_frac));
  assert_frac("a * zero = zero", cnd(fmul(a, zero_frac), zero_frac));
  assert_frac("zero * (-a) = zero", cnd(fmul(zero_frac, _a), zero_frac));
  assert_frac("(-a) * zero = zero", cnd(fmul(_a, zero_frac), zero_frac));
  assert_frac("id * a = a", cnd(fmul(id_frac, a), a));
  assert_frac("a * id = a", cnd(fmul(a, id_frac), a));
  assert_frac("id * (-a) = -a", cnd(fmul(id_frac, _a), _a));
  assert_frac("(-a) * id = -a", cnd(fmul(_a, id_frac), _a));
  assert_frac("a * (1/a) = id", cnd(fmul(a, a_over_1),id_frac));
  assert_frac("(-a) * (-1/a) = id", cnd(fmul(_a, _a_over_1),id_frac));
  assert_frac("(1/a) * a = id", cnd(fmul(a_over_1, a),id_frac));
  assert_frac("(-1/a) * (-a) = id", cnd(fmul(_a_over_1, _a),id_frac));
  assert_frac("(-a) * a = -a^2", cnd(fmul(_a, a), fxy(16, 9, mins)));
  assert_frac("a * (-a) = -a^2", cnd(fmul(a, _a), fxy(16, 9, mins)));
  assert_frac("(-a) * (-a) = a^2", cnd(fmul(_a, _a), fxy(16, 9, plus)));
  assert_frac("a * a = a^2", cnd(fmul(a, a), fxy(16, 9, plus)));
  assert_frac("(a*b)*c = a*(b*c)", cnd(fmul(fmul(a, b), c), fmul(a, fmul(b, c))));
  assert_frac("(4/3)*(7/4)*(1/2) = 7/6", cnd(fmul(fmul(a, c), b), fxy(7, 6, plus)));

  print_footer();
}

/**
 * @brief Test `fdiv` method.
 * 
 */
void test_fdiv(void) {
  print_header("fdiv");

  frac_t a = fxy(1, 3, plus);
  frac_t _a = fxy(1, 3, mins);
  frac_t b = fxy(1, 2, plus);
  frac_t c = fxy(7, 4, plus);

  assert_frac("nan / nan = nan", is_fnan(fdiv(nan_frac, nan_frac)));
  assert_frac("nan / a = nan", is_fnan(fdiv(nan_frac, a)));
  assert_frac("a / nan = nan", is_fnan(fdiv(a, nan_frac)));
  assert_frac("inf / inf = nan", is_fnan(fdiv(pinf_frac, pinf_frac)));
  assert_frac("inf / (-inf) = nan", is_fnan(fdiv(pinf_frac, minf_frac)));
  assert_frac("(-inf) / inf = nan", is_fnan(fdiv(minf_frac, pinf_frac)));
  assert_frac("(-inf) / (-inf) = nan", is_fnan(fdiv(minf_frac, minf_frac)));
  assert_frac("inf / a = inf", is_fpinf(fdiv(pinf_frac, a)));
  assert_frac("inf / (-a) = -inf", is_fminf(fdiv(pinf_frac, _a)));
  assert_frac("(-inf) / a = -inf", is_fminf(fdiv(minf_frac, a)));
  assert_frac("(-inf) / (-a) = inf", is_fpinf(fdiv(minf_frac, _a)));
  assert_frac("a / inf = zero", is_fzero(fdiv(a, pinf_frac)));
  assert_frac("a / (-inf) = zero", is_fzero(fdiv(a, minf_frac)));
  assert_frac("(-a) / inf = zero", is_fzero(fdiv(_a, pinf_frac)));
  assert_frac("(-a) / (-inf) = zero", is_fzero(fdiv(_a, minf_frac)));
  assert_frac("zero / a = zero", is_fzero(fdiv(zero_frac, a)));
  assert_frac("a / zero = inf", is_fpinf(fdiv(a, zero_frac)));
  assert_frac("(-a) / zero = -inf", is_fminf(fdiv(_a, zero_frac)));
  assert_frac("zero / (-a) = zero", is_fzero(fdiv(zero_frac, _a)));
  assert_frac("1/(x/y) = y/x", cnd(fdiv(id_frac, a), fxy(a.y, a.x, a.s)));
  assert_frac("a / 1 = a", cnd(fdiv(a, id_frac), a));
  assert_frac("(-a) / 1 = -a", cnd(fdiv(_a, id_frac), _a));
  assert_frac("a / a = 1", is_fid(fdiv(a, a)));
  assert_frac("(-a) / a = -1", is_fid(fmconst(fdiv(_a, a), -1)));
  assert_frac("a / (-a) = -1", is_fid(fmconst(fdiv(a, _a), -1)));
  assert_frac("(-a) / (-a) = 1", is_fid(fdiv(_a, _a)));
  assert_frac("a/b != b/a", !cnd(fdiv(a, b), fdiv(b, a)));
  assert_frac("(a/b)/c != a/(b/c)", !cnd(fdiv(fdiv(a, b), c), fdiv(a, fdiv(b, c))));
  assert_frac("a/b/c = a/(b/c)", cnd(fxy(7, 6, plus), fdiv(a, fdiv(b, c))));
  assert_frac("(1/3)/(1/2)/(7/4) = 7/6", cnd(fdiv(a, fdiv(b, c)), fxy(7, 6, plus)));

  print_footer();
}

/**
 * @brief Test `feq` method.
 * 
 */
void test_feq(void) {
  print_header("feq");

  frac_t a = fxy(1, 3, plus);
  frac_t b = fxy(4, 7, plus);

  assert_frac("nan == nan", feq(nan_frac, nan_frac) == funknown);
  assert_frac("a == nan", feq(a, nan_frac) == funknown);
  assert_frac("nan == a", feq(nan_frac, a) == funknown);
  assert_frac("inf == inf", feq(pinf_frac, pinf_frac) == funknown);
  assert_frac("-inf == -inf", feq(minf_frac, minf_frac) == funknown);
  assert_frac("-inf == inf", feq(minf_frac, pinf_frac) == ffalse);
  assert_frac("inf == -inf", feq(pinf_frac, minf_frac) == ffalse);
  assert_frac("inf == a", feq(pinf_frac, a) == ffalse);
  assert_frac("-inf == a", feq(minf_frac, a) == ffalse);
  assert_frac("a == inf", feq(a, pinf_frac) == ffalse);
  assert_frac("a == -inf", feq(a, minf_frac) == ffalse);
  assert_frac("a == a", feq(a, a) == ftrue);
  assert_frac("a == b", feq(a, b) == ffalse);
  assert_frac("b == a", feq(b, a) == ffalse);
  
  print_footer();
}

/**
 * @brief Test `fneq` method.
 * 
 */
void test_fneq(void) {
  print_header("fneq");

  frac_t a = fxy(1, 3, plus);
  frac_t b = fxy(4, 7, plus);

  assert_frac("nan != nan", fneq(nan_frac, nan_frac) == funknown);
  assert_frac("a != nan", fneq(a, nan_frac) == funknown);
  assert_frac("nan != a", fneq(nan_frac, a) == funknown);
  assert_frac("inf != inf", fneq(pinf_frac, pinf_frac) == funknown);
  assert_frac("-inf != -inf", fneq(minf_frac, minf_frac) == funknown);
  assert_frac("-inf != inf", fneq(minf_frac, pinf_frac) == ftrue);
  assert_frac("inf != -inf", fneq(pinf_frac, minf_frac) == ftrue);
  assert_frac("inf != a", fneq(pinf_frac, a) == ftrue);
  assert_frac("-inf != a", fneq(minf_frac, a) == ftrue);
  assert_frac("a != inf", fneq(a, pinf_frac) == ftrue);
  assert_frac("a != -inf", fneq(a, minf_frac) == ftrue);
  assert_frac("a != a", fneq(a, a) == ffalse);
  assert_frac("a != b", fneq(a, b) == ftrue);
  assert_frac("b != a", fneq(b, a) == ftrue);

  print_footer();
}

/**
 * @brief Test `fgt` method.
 * 
 */
void test_fgt(void) {
  print_header("fgt");

  frac_t a = fxy(1, 3, plus);
  frac_t _a = fxy(1, 3, mins);
  frac_t b = fxy(4, 7, plus);

  assert_frac("nan > nan", fgt(nan_frac, nan_frac) == funknown);
  assert_frac("nan > a", fgt(nan_frac, a) == funknown);
  assert_frac("a > nan", fgt(a, nan_frac) == funknown);
  assert_frac("inf > inf", fgt(pinf_frac, pinf_frac) == funknown);
  assert_frac("inf > -inf", fgt(pinf_frac, minf_frac) == ftrue);
  assert_frac("-inf > inf", fgt(minf_frac, pinf_frac) == ffalse);
  assert_frac("-inf > -inf", fgt(minf_frac, minf_frac) == funknown);
  assert_frac("inf > a", fgt(pinf_frac, a) == ftrue);
  assert_frac("inf > -a", fgt(pinf_frac, _a) == ftrue);
  assert_frac("-inf > a", fgt(minf_frac, a) == ffalse);
  assert_frac("-inf > -a", fgt(minf_frac, _a) == ffalse);
  assert_frac("a > inf", fgt(a, pinf_frac) == ffalse);
  assert_frac("-a > inf", fgt(_a, pinf_frac) == ffalse);
  assert_frac("a > -inf", fgt(a, minf_frac) == ftrue);
  assert_frac("-a > -inf", fgt(_a, minf_frac) == ftrue);
  assert_frac("a > b", fgt(a, b) == ffalse);
  assert_frac("b > a", fgt(b, a) == ftrue);
  assert_frac("a > a", fgt(a, a) == ffalse);
  assert_frac("-a > -a", fgt(_a, _a) == ffalse);
  assert_frac("-a > b", fgt(_a, b) == ffalse);
  assert_frac("b > -a", fgt(b, _a) == ftrue);

  print_footer();
}

/**
 * @brief Test `flt` method.
 * 
 */
void test_flt(void) {
  print_header("flt");

  frac_t a = fxy(1, 3, plus);
  frac_t _a = fxy(1, 3, mins);
  frac_t b = fxy(4, 7, plus);

  assert_frac("nan < nan", flt(nan_frac, nan_frac) == funknown);
  assert_frac("nan < a", flt(nan_frac, a) == funknown);
  assert_frac("a < nan", flt(a, nan_frac) == funknown);
  assert_frac("inf < inf", flt(pinf_frac, pinf_frac) == funknown);
  assert_frac("inf < -inf", flt(pinf_frac, minf_frac) == ffalse);
  assert_frac("-inf < inf", flt(minf_frac, pinf_frac) == ftrue);
  assert_frac("-inf < -inf", flt(minf_frac, minf_frac) == funknown);
  assert_frac("inf < a", flt(pinf_frac, a) == ffalse);
  assert_frac("inf < -a", flt(pinf_frac, _a) == ffalse);
  assert_frac("-inf < a", flt(minf_frac, a) == ftrue);
  assert_frac("-inf < -a", flt(minf_frac, _a) == ftrue);
  assert_frac("a < inf", flt(a, pinf_frac) == ftrue);
  assert_frac("-a < inf", flt(_a, pinf_frac) == ftrue);
  assert_frac("a < -inf", flt(a, minf_frac) == ffalse);
  assert_frac("-a < -inf", flt(_a, minf_frac) == ffalse);
  assert_frac("a < b", flt(a, b) == ftrue);
  assert_frac("b < a", flt(b, a) == ffalse);
  assert_frac("a < a", flt(a, a) == ffalse);
  assert_frac("-a < -a", flt(_a, _a) == ffalse);
  assert_frac("-a < b", flt(_a, b) == ftrue);
  assert_frac("b < -a", flt(b, _a) == ffalse);

  print_footer();
}

/**
 * @brief Test `fgte` method.
 * 
 */
void test_fgte(void) {
  print_header("fgte");

  frac_t a = fxy(1, 3, plus);
  frac_t _a = fxy(1, 3, mins);
  frac_t b = fxy(4, 7, plus);

  assert_frac("nan >= nan", fgte(nan_frac, nan_frac) == funknown);
  assert_frac("nan >= a", fgte(nan_frac, a) == funknown);
  assert_frac("a >= nan", fgte(a, nan_frac) == funknown);
  assert_frac("inf >= inf", fgte(pinf_frac, pinf_frac) == funknown);
  assert_frac("inf >= -inf", fgte(pinf_frac, minf_frac) == ftrue);
  assert_frac("-inf >= inf", fgte(minf_frac, pinf_frac) == ffalse);
  assert_frac("-inf >= -inf", fgte(minf_frac, minf_frac) == funknown);
  assert_frac("inf >= a", fgte(pinf_frac, a) == ftrue);
  assert_frac("inf >= -a", fgte(pinf_frac, _a) == ftrue);
  assert_frac("-inf >= a", fgte(minf_frac, a) == ffalse);
  assert_frac("-inf >= -a", fgte(minf_frac, _a) == ffalse);
  assert_frac("a >= inf", fgte(a, pinf_frac) == ffalse);
  assert_frac("-a >= inf", fgte(_a, pinf_frac) == ffalse);
  assert_frac("a >= -inf", fgte(a, minf_frac) == ftrue);
  assert_frac("-a >= -inf", fgte(_a, minf_frac) == ftrue);
  assert_frac("a >= b", fgte(a, b) == ffalse);
  assert_frac("b >= a", fgte(b, a) == ftrue);
  assert_frac("a >= a", fgte(a, a) == ftrue);
  assert_frac("-a >= -a", fgte(_a, _a) == ftrue);
  assert_frac("-a >= b", fgte(_a, b) == ffalse);
  assert_frac("b >= -a", fgte(b, _a) == ftrue);

  print_footer();
}

/**
 * @brief Test `flte` method.
 * 
 */
void test_flte(void) {
  print_header("flte");

  frac_t a = fxy(1, 3, plus);
  frac_t _a = fxy(1, 3, mins);
  frac_t b = fxy(4, 7, plus);

  assert_frac("nan <= nan", flte(nan_frac, nan_frac) == funknown);
  assert_frac("nan <= a", flte(nan_frac, a) == funknown);
  assert_frac("a <= nan", flte(a, nan_frac) == funknown);
  assert_frac("inf <= inf", flte(pinf_frac, pinf_frac) == funknown);
  assert_frac("inf <= -inf", flte(pinf_frac, minf_frac) == ffalse);
  assert_frac("-inf <= inf", flte(minf_frac, pinf_frac) == ftrue);
  assert_frac("-inf <= -inf", flte(minf_frac, minf_frac) == funknown);
  assert_frac("inf <= a", flte(pinf_frac, a) == ffalse);
  assert_frac("inf <= -a", flte(pinf_frac, _a) == ffalse);
  assert_frac("-inf <= a", flte(minf_frac, a) == ftrue);
  assert_frac("-inf <= -a", flte(minf_frac, _a) == ftrue);
  assert_frac("a <= inf", flte(a, pinf_frac) == ftrue);
  assert_frac("-a <= inf", flte(_a, pinf_frac) == ftrue);
  assert_frac("a <= -inf", flte(a, minf_frac) == ffalse);
  assert_frac("-a <= -inf", flte(_a, minf_frac) == ffalse);
  assert_frac("a <= b", flte(a, b) == ftrue);
  assert_frac("b <= a", flte(b, a) == ffalse);
  assert_frac("a <= a", flte(a, a) == ftrue);
  assert_frac("-a <= -a", flte(_a, _a) == ftrue);
  assert_frac("-a <= b", flte(_a, b) == ftrue);
  assert_frac("b <= -a", flte(b, _a) == ffalse);

  print_footer();
}

/**
 * @brief Main function that calls every test function appart.
 *
 */
int main(void) {
  print_header("FRAC UNIT TESTS");

  test_is_finf();
  test_is_fzero();
  test_is_fid();

  test_fxy();

  test_fmconst();
  test_fdconst();
  test_fadd();
  test_fsub();
  test_fmul();
  test_fdiv();

  test_feq();
  test_fneq();
  test_fgt();
  test_flt();
  test_fgte();
  test_flte();

  return 0;
}