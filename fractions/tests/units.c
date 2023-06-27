#include <assert.h>
#include <string.h>
#include "../src/fractions.h"

typedef enum status_s {
  FAILED = 0,
  PASSED = 1
} status_t;

#define cnd0(f, nx, ny, ns) (f.x == nx && f.y == ny && f.s == ns)
#define cnd1(f1, f2) (f1.x == f2.x && f1.y == f2.y && f1.s == f2.s)

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

void print_footer(void) {
  for (size_t dot = 0; dot < 46; ++dot) {
    fprintf(stderr, ".");
  }
  
  fprintf(stderr, "\n\n");
}

void assert_frac(const char *const msg, status_t cond) {
  size_t msg_len = strlen(msg);
  size_t dots = 38 - msg_len;

  fprintf(stderr, "%s ", msg);
  for (size_t dot = 0; dot < dots; ++dot) {
    fprintf(stderr, ".");
  }
  fprintf(stderr,
          " %s\n",
          cond == PASSED ? "\033[0;32mpassed\033[0m" : "\033[0;31mfailed\033[0m"
  );
}

void test_is_ferror(void) {
  print_header("is_ferror");

  assert_frac("error = error", is_ferror(error_frac));
  assert_frac("1/2 != error", !is_ferror((frac_t){1, 2, plus}));
  assert_frac("-1/2 != error", !is_ferror((frac_t){1, 2, mins}));
  assert_frac("zero != error", !is_ferror(zero_frac));
  assert_frac("id != error", !is_ferror(id_frac));
  assert_frac("0/2 != error", !is_ferror((frac_t){0, 2, plus}));
  assert_frac("-0/2 != error", !is_ferror((frac_t){0, 2, mins}));

  print_footer();
}

void test_is_fzero(void) {
  print_header("is_fzero");

  assert_frac("error != zero", !is_fzero(error_frac));
  assert_frac("1/2 != zero", !is_fzero((frac_t){1, 2, plus}));
  assert_frac("-1/2 != zero", !is_fzero((frac_t){1, 2, mins}));
  assert_frac("zero = zero", is_fzero(zero_frac));
  assert_frac("id != zero", !is_fzero(id_frac));
  assert_frac("0/2 = zero", is_fzero((frac_t){0, 2, plus}));
  assert_frac("-0/2 = zero", is_fzero((frac_t){0, 2, mins}));

  print_footer();
}

void test_is_fid(void) {
  print_header("is_fid");

  assert_frac("error != id", !is_fid(error_frac));
  assert_frac("1/2 != id", !is_fid((frac_t){1, 2, plus}));
  assert_frac("-1/2 != id", !is_fid((frac_t){1, 2, mins}));
  assert_frac("zero != id", !is_fid(zero_frac));
  assert_frac("id = id", is_fid(id_frac));
  assert_frac("0/2 != id", !is_fid((frac_t){0, 2, plus}));
  assert_frac("-0/2 != id", !is_fid((frac_t){0, 2, mins}));
  assert_frac("-1/1 != id", !is_fid((frac_t){1, 1, mins}));
  assert_frac("1/1 = id", is_fid((frac_t){1, 1, plus}));

  print_footer();
}

void test_fxy(void) {
  print_header("fxy");

  assert_frac("1/1 = id", is_fid(fxy(1, 1, plus)));
  assert_frac("-1/1 != id", !cnd1(fxy(1, 1, mins), id_frac));
  assert_frac("0/1 = 0", is_fzero(fxy(0, 1, plus)));
  assert_frac("-0/1 = 0", is_fzero(fxy(0, 1, mins)));
  assert_frac("1/0 = error", is_ferror(fxy(1, 0, plus)));
  assert_frac("-1/0 = error", is_ferror(fxy(1, 0, mins)));
  assert_frac("24/6 = 4", cnd1(fxy(24, 6, plus), fxy(4, 1, plus)));
  assert_frac("6/24 = 1/4", cnd1(fxy(6, 24, plus), fxy(1, 4, plus)));
  assert_frac("1/7 = 1/7", cnd1(fxy(1, 7, plus), fxy(1, 7, plus)));

  print_footer();
}

void test_fconst(void) {
  print_header("fconst");

  frac_t a = fxy(1, 3, plus);
  frac_t _a = fxy(1, 3, mins);
  int32_t c = 7;

  assert_frac("c * error = error", is_ferror(fconst(error_frac, c)));
  assert_frac("(-c) * error = error", is_ferror(fconst(error_frac, -c)));
  assert_frac("c * 0 = 0", is_fzero(fconst(zero_frac, c)));
  assert_frac("(-c) * 0 = 0", is_fzero(fconst(zero_frac, -c)));
  assert_frac("c * a = (c*a)", cnd1(fconst(a, c), fxy(7, 3, plus)));
  assert_frac("(-c) * a = -(c*a)", cnd1(fconst(a, -c), fxy(7, 3, mins)));
  assert_frac("0 * a = 0", is_fzero(fconst(a, 0)));
  assert_frac("0 * (-a) = 0", is_fzero(fconst(_a, 0)));
  assert_frac("c * id = (c*id)", cnd1(fconst(id_frac, c), fxy(c, 1, plus)));
  assert_frac("(-c) * id = -(c*id)", cnd1(fconst(id_frac, -c), fxy(c, 1, mins)));

  print_footer();
}

void test_fadd(void) {
  print_header("fadd");

  frac_t a = fxy(1, 3, plus);
  frac_t _a = fxy(1, 3, mins);
  frac_t b = fxy(1, 2, plus);
  frac_t c = fxy(7, 4, plus);

  assert_frac("error + error = error", is_ferror(fadd(error_frac, error_frac)));
  assert_frac("error + a = error", is_ferror(fadd(error_frac, a)));
  assert_frac("a + error = error", is_ferror(fadd(a, error_frac)));
  assert_frac("0 + error = error", is_ferror(fadd(zero_frac, error_frac)));
  assert_frac("error + 0 = error", is_ferror(fadd(error_frac, zero_frac)));
  assert_frac("0 + a = a", cnd1(fadd(zero_frac, a), a));
  assert_frac("0 + (-a) = -a", cnd1(fadd(zero_frac, _a), _a));
  assert_frac("a + 0 = a", cnd1(fadd(a, zero_frac), a));
  assert_frac("(-a) + 0 = -a", cnd1(fadd(_a, zero_frac), _a));
  assert_frac("a + a = 2a", cnd1(fadd(a, a), fconst(a, 2)));
  assert_frac("a + (-a) = 0", cnd1(fadd(a, _a), zero_frac));
  assert_frac("(-a) + (-a) = -2a", cnd1(fadd(_a, _a), fconst(a, -2)));
  assert_frac("a + b = b + a", cnd1(fadd(a, b), fadd(b, a)));
  assert_frac("(a+b)+c = a+(b+c)", cnd1(fadd(fadd(a, b), c), fadd(a, fadd(b, c))));

  print_footer();
}

void test_fsub(void) {
  print_header("fsub");

  frac_t a = fxy(1, 3, plus);
  frac_t _a = fxy(1, 3, mins);
  frac_t b = fxy(1, 2, plus);
  frac_t c = fxy(7, 4, plus);

  assert_frac("error - error = error", is_ferror(fsub(error_frac, error_frac)));
  assert_frac("error - a = error", is_ferror(fsub(error_frac, a)));
  assert_frac("a - error = error", is_ferror(fsub(a, error_frac)));
  assert_frac("0 - error = error", is_ferror(fsub(zero_frac, error_frac)));
  assert_frac("error - 0 = error", is_ferror(fsub(error_frac, zero_frac)));
  assert_frac("0 - a = -a", cnd1(fsub(zero_frac, a), _a));
  assert_frac("0 - (-a) = a", cnd1(fsub(zero_frac, _a), a));
  assert_frac("a - 0 = a", cnd1(fsub(a, zero_frac), a));
  assert_frac("(-a) - 0 = -a", cnd1(fsub(_a, zero_frac), _a));
  assert_frac("a - a = 0", cnd1(fsub(a, a), zero_frac));
  assert_frac("(-a) - (-a) = 0", cnd1(fsub(_a, _a), zero_frac));
  assert_frac("(-a) - a = -2a", cnd1(fsub(_a, a), fconst(a, -2)));
  assert_frac("a - (-a) = 2a", cnd1(fsub(a, _a), fconst(a, 2)));
  assert_frac("a - b != b - a", !cnd1(fsub(a, b), fsub(b, a)));
  assert_frac("(a-b)-c = a-(b+c)", cnd1(fsub(fsub(a, b), c), fsub(a, fadd(b, c))));

  print_footer();
}

void test_fmul(void) {
  print_header("fmul");

  frac_t a = fxy(1, 3, plus);
  frac_t _a = fxy(1, 3, mins);
  frac_t b = fxy(1, 2, plus);
  frac_t c = fxy(7, 4, plus);

  assert_frac("error * error = error", is_ferror(fmul(error_frac, error_frac)));
  assert_frac("error * a = error", is_ferror(fmul(error_frac, a)));
  assert_frac("a * error = error", is_ferror(fmul(a, error_frac)));
  assert_frac("0 * error = error", is_ferror(fmul(zero_frac, error_frac)));
  assert_frac("error * 0 = error", is_ferror(fmul(error_frac, zero_frac)));
  assert_frac("1 * error = error", is_ferror(fmul(id_frac, error_frac)));
  assert_frac("error * 1 = error", is_ferror(fmul(error_frac, id_frac)));
  assert_frac("0 * a = 0", cnd1(fmul(zero_frac, a), zero_frac));
  assert_frac("a * 0 = 0", cnd1(fmul(a, zero_frac), zero_frac));
  assert_frac("0 * (-a) = 0", cnd1(fmul(zero_frac, _a), zero_frac));
  assert_frac("(-a) * 0 = 0", cnd1(fmul(_a, zero_frac), zero_frac));
  assert_frac("1 * a = a", cnd1(fmul(id_frac, a), a));
  assert_frac("a * 1 = a", cnd1(fmul(a, id_frac), a));
  assert_frac("1 * (-a) = -a", cnd1(fmul(id_frac, _a), _a));
  assert_frac("(-a) * 1 = -a", cnd1(fmul(_a, id_frac), _a));
  assert_frac("(-a) * a = -a^2", cnd1(fmul(_a, a), fxy(1, 9, mins)));
  assert_frac("a * (-a) = -a^2", cnd1(fmul(a, _a), fxy(1, 9, mins)));
  assert_frac("(-a) * (-a) = a^2", cnd1(fmul(_a, _a), fxy(1, 9, plus)));
  assert_frac("a * a = a^2", cnd1(fmul(a, a), fxy(1, 9, plus)));
  assert_frac("(a*b)*c = a*(b*c)", cnd1(fmul(fmul(a, b), c), fmul(a, fmul(b, c))));

  print_footer();
}

void test_fdiv(void) {
  print_header("fdiv");

  frac_t a = fxy(1, 3, plus);
  frac_t _a = fxy(1, 3, mins);
  frac_t b = fxy(1, 2, plus);
  frac_t c = fxy(7, 4, plus);

  assert_frac("error / error = error", is_ferror(fdiv(error_frac, error_frac)));
  assert_frac("error / a = error", is_ferror(fdiv(error_frac, a)));
  assert_frac("a / error = error", is_ferror(fdiv(a, error_frac)));
  assert_frac("0 / error = error", is_ferror(fdiv(zero_frac, error_frac)));
  assert_frac("error / 0 = error", is_ferror(fdiv(error_frac, zero_frac)));
  assert_frac("0 / a = 0", is_fzero(fdiv(zero_frac, a)));
  assert_frac("a / 0 = 0", is_ferror(fdiv(a, zero_frac)));
  assert_frac("0 / (-a) = 0", is_fzero(fdiv(zero_frac, _a)));
  assert_frac("(-a) / 0 = 0", is_ferror(fdiv(_a, zero_frac)));
  assert_frac("1/(x/y) = y/x", cnd1(fdiv(id_frac, a), fxy(a.y, a.x, a.s)));
  assert_frac("a / 1 = a", cnd1(fdiv(a, id_frac), a));
  assert_frac("(-a) / 1 = -a", cnd1(fdiv(_a, id_frac), _a));
  assert_frac("a / a = 1", is_fid(fdiv(a, a)));
  assert_frac("(-a) / a = -1", is_fid(fconst(fdiv(_a, a), -1)));
  assert_frac("a / (-a) = -1", is_fid(fconst(fdiv(a, _a), -1)));
  assert_frac("(-a) / (-a) = 1", is_fid(fdiv(_a, _a)));
  assert_frac("a/b != b/a", !cnd1(fdiv(a, b), fdiv(b, a)));
  assert_frac("(a/b)/c != a/(b/c)", !cnd1(fdiv(fdiv(a, b), c), fdiv(a, fdiv(b, c))));
  assert_frac("a/b/c = a/(b/c)", cnd1(fxy(7, 6, plus), fdiv(a, fdiv(b, c))));

  print_footer();
}

void test_fconstp(void) {
  print_header("fconstp");

  frac_t a = fxy(1, 3, plus);
  frac_t _a = fxy(1, 3, mins);
  int32_t c = 7;

  frac_t f = error_frac; fconstp(&f, c);
  assert_frac("c * error = error", is_ferror(f));

  f = error_frac; fconstp(&f, -c);
  assert_frac("(-c) * error = error", is_ferror(f));

  f = zero_frac; fconstp(&f, c);
  assert_frac("c * 0 = 0", is_fzero(f));

  f = zero_frac; fconstp(&f, -c);
  assert_frac("(-c) * 0 = 0", is_fzero(f));

  f = a; fconstp(&f, c);
  assert_frac("c * a = (c*a)", cnd1(f, fxy(7, 3, plus)));

  f = a; fconstp(&f, -c);
  assert_frac("(-c) * a = -(c*a)", cnd1(f, fxy(7, 3, mins)));

  f = a; fconstp(&f, 0);
  assert_frac("0 * a = 0", is_fzero(f));

  f = _a; fconstp(&f, 0);
  assert_frac("0 * (-a) = 0", is_fzero(f));

  f = id_frac; fconstp(&f, c);
  assert_frac("c * id = (c*id)", cnd1(f, fxy(c, 1, plus)));

  f = id_frac; fconstp(&f, -c);
  assert_frac("(-c) * id = -(c*id)", cnd1(f, fxy(c, 1, mins)));

  print_footer();
}

void test_faddp(void) {
  print_header("faddp");

  frac_t a = fxy(1, 3, plus);
  frac_t _a = fxy(1, 3, mins);
  frac_t b = fxy(1, 2, plus);
  frac_t c = fxy(7, 4, plus);

  frac_t f = error_frac; faddp(&f, error_frac);
  assert_frac("error + error = error", is_ferror(f));

  f = error_frac; faddp(&f, a);
  assert_frac("error + a = error", is_ferror(f));

  f = a; faddp(&f, error_frac);
  assert_frac("a + error = error", is_ferror(f));

  f = zero_frac; faddp(&f, error_frac);
  assert_frac("0 + error = error", is_ferror(f));

  f = error_frac; faddp(&f, zero_frac);
  assert_frac("error + 0 = error", is_ferror(f));

  f = zero_frac; faddp(&f, a);
  assert_frac("0 + a = a", cnd1(f, a));

  f = zero_frac; faddp(&f, _a);
  assert_frac("0 + (-a) = -a", cnd1(f, _a));

  f = a; faddp(&f, zero_frac);
  assert_frac("a + 0 = a", cnd1(f, a));

  f = _a; faddp(&f, zero_frac);
  assert_frac("(-a) + 0 = -a", cnd1(f, _a));

  f = a; faddp(&f, a);
  assert_frac("a + a = 2a", cnd1(f, fconst(a, 2)));

  f = a; faddp(&f, _a);
  assert_frac("a + (-a) = 0", cnd1(f, zero_frac));

  f = _a; faddp(&f, _a);
  assert_frac("(-a) + (-a) = -2a", cnd1(f, fconst(a, -2)));

  f = a; faddp(&f, b);
  frac_t f1 = b; faddp(&f1, a);
  assert_frac("a + b = b + a", cnd1(f, f1));

  f = a; faddp(&f, b); faddp(&f, c);
  f1 = b; faddp(&f1, c); faddp(&f1, a);
  assert_frac("(a+b)+c = a+(b+c)", cnd1(f, f1));

  print_footer();
}

void test_fsubp(void) {
  print_header("fsubp");

  frac_t a = fxy(1, 3, plus);
  frac_t _a = fxy(1, 3, mins);
  frac_t b = fxy(1, 2, plus);
  frac_t c = fxy(7, 4, plus);

  frac_t f = error_frac; fsubp(&f, error_frac);
  assert_frac("error - error = error", is_ferror(f));

  f = error_frac; fsubp(&f, a);
  assert_frac("error - a = error", is_ferror(f));

  f = a; fsubp(&f, error_frac);
  assert_frac("a - error = error", is_ferror(f));

  f = zero_frac; fsubp(&f, error_frac);
  assert_frac("0 - error = error", is_ferror(f));

  f = error_frac; fsubp(&f, zero_frac);
  assert_frac("error - 0 = error", is_ferror(f));

  f = zero_frac; fsubp(&f, a);
  assert_frac("0 - a = -a", cnd1(f, _a));

  f = zero_frac; fsubp(&f, _a);
  assert_frac("0 - (-a) = a", cnd1(f, a));

  f = a; fsubp(&f, zero_frac);
  assert_frac("a - 0 = a", cnd1(f, a));

  f = _a; fsubp(&f, zero_frac);
  assert_frac("(-a) - 0 = -a", cnd1(f, _a));

  f = a; fsubp(&f, a);
  assert_frac("a - a = 0", cnd1(f, zero_frac));

  f = _a; fsubp(&f, _a);
  assert_frac("(-a) - (-a) = 0", cnd1(f, zero_frac));

  f = _a; fsubp(&f, a);
  assert_frac("(-a) - a = -2a", cnd1(f, fconst(a, -2)));

  f = a; fsubp(&f, _a);
  assert_frac("a - (-a) = 2a", cnd1(f, fconst(a, 2)));

  f = a; fsubp(&f, b);
  frac_t f1 = b; fsubp(&f1, a);
  assert_frac("a - b != b - a", !cnd1(f, f1));

  f = a; fsubp(&f, b); fsubp(&f, c);
  f1 = b; faddp(&f1, c); frac_t f2 = a; fsubp(&f2, f1);
  assert_frac("(a-b)-c = a-(b+c)", cnd1(f, f2));

  print_footer();
}

void test_fmulp(void) {
  print_header("fmulp");

  frac_t a = fxy(1, 3, plus);
  frac_t _a = fxy(1, 3, mins);
  frac_t b = fxy(1, 2, plus);
  frac_t c = fxy(7, 4, plus);

  frac_t f = error_frac; fmulp(&f, error_frac);
  assert_frac("error * error = error", is_ferror(f));

  f = error_frac; fmulp(&f, a);
  assert_frac("error * a = error", is_ferror(f));

  f = a; fmulp(&f, error_frac);
  assert_frac("a * error = error", is_ferror(f));

  f = zero_frac; fmulp(&f, error_frac);
  assert_frac("0 * error = error", is_ferror(f));

  f = error_frac; fmulp(&f, zero_frac);
  assert_frac("error * 0 = error", is_ferror(f));

  f = id_frac; fmulp(&f, error_frac);
  assert_frac("1 * error = error", is_ferror(f));

  f = error_frac; fmulp(&f, id_frac);
  assert_frac("error * 1 = error", is_ferror(f));

  f = zero_frac; fmulp(&f, a);
  assert_frac("0 * a = 0", cnd1(f, zero_frac));

  f = a; fmulp(&f, zero_frac);
  assert_frac("a * 0 = 0", cnd1(f, zero_frac));

  f = zero_frac; fmulp(&f, _a);
  assert_frac("0 * (-a) = 0", cnd1(f, zero_frac));

  f = _a; fmulp(&f, zero_frac);
  assert_frac("(-a) * 0 = 0", cnd1(f, zero_frac));

  f = id_frac; fmulp(&f, a);
  assert_frac("1 * a = a", cnd1(f, a));

  f = a; fmulp(&f, id_frac);
  assert_frac("a * 1 = a", cnd1(f, a));

  f = id_frac; fmulp(&f, _a);
  assert_frac("1 * (-a) = -a", cnd1(f, _a));

  f = _a; fmulp(&f, id_frac);
  assert_frac("(-a) * 1 = -a", cnd1(f, _a));

  f = _a; fmulp(&f, a);
  assert_frac("(-a) * a = -a^2", cnd1(f, fxy(1, 9, mins)));

  f = a; fmulp(&f, _a);
  assert_frac("a * (-a) = -a^2", cnd1(f, fxy(1, 9, mins)));

  f = _a; fmulp(&f, _a);
  assert_frac("(-a) * (-a) = a^2", cnd1(f, fxy(1, 9, plus)));

  f = a; fmulp(&f, a);
  assert_frac("a * a = a^2", cnd1(f, fxy(1, 9, plus)));

  f = a; fmulp(&f, b); fmulp(&f, c);
  frac_t f1 = b; fmulp(&f1, c); frac_t f2 = a; fmulp(&f2, f1);
  assert_frac("(a*b)*c = a*(b*c)", cnd1(f, f2));

  print_footer();
}

void test_fdivp(void) {
  print_header("fdivp");

  frac_t a = fxy(1, 3, plus);
  frac_t _a = fxy(1, 3, mins);
  frac_t b = fxy(1, 2, plus);
  frac_t c = fxy(7, 4, plus);

  frac_t f = error_frac; fdivp(&f, error_frac);
  assert_frac("error / error = error", is_ferror(f));

  f = error_frac; fdivp(&f, a);
  assert_frac("error / a = error", is_ferror(f));

  f = a; fdivp(&f, error_frac);
  assert_frac("a / error = error", is_ferror(f));

  f = zero_frac; fdivp(&f, error_frac);
  assert_frac("0 / error = error", is_ferror(f));

  f = error_frac; fdivp(&f, zero_frac);
  assert_frac("error / 0 = error", is_ferror(f));

  f = zero_frac; fdivp(&f, a);
  assert_frac("0 / a = 0", is_fzero(f));

  f = a; fdivp(&f, zero_frac);
  assert_frac("a / 0 = 0", is_ferror(f));

  f = zero_frac; fdivp(&f, _a);
  assert_frac("0 / (-a) = 0", is_fzero(f));

  f = _a; fdivp(&f, zero_frac);
  assert_frac("(-a) / 0 = 0", is_ferror(f));

  f = id_frac; fdivp(&f, a);
  assert_frac("1/(x/y) = y/x", cnd1(f, fxy(a.y, a.x, a.s)));

  f = a; fdivp(&f, id_frac);
  assert_frac("a / 1 = a", cnd1(f, a));

  f = _a; fdivp(&f, id_frac);
  assert_frac("(-a) / 1 = -a", cnd1(f, _a));

  f = a; fdivp(&f, a);
  assert_frac("a / a = 1", is_fid(f));

  f = _a; fdivp(&f, a);
  assert_frac("(-a) / a = -1", is_fid(fconst(f, -1)));

  f = a; fdivp(&f, _a);
  assert_frac("a / (-a) = -1", is_fid(fconst(f, -1)));

  f = _a; fdivp(&f, _a);
  assert_frac("(-a) / (-a) = 1", is_fid(f));

  f = a; fdivp(&f, b);
  frac_t f1 = b; fdivp(&f1, a);
  assert_frac("a/b != b/a", !cnd1(f, f1));

  f = a; fdivp(&f, b); fdivp(&f, c);
  f1 = b; fdivp(&f1, c); frac_t f2 = a; fdivp(&f2, f1);
  assert_frac("(a/b)/c != a/(b/c)", !cnd1(f, f2));

  f = b; fdivp(&f, c); f1 = a; fdivp(&f1, f);
  assert_frac("a/b/c = a/(b/c)", cnd1(fxy(7, 6, plus), f1));

  print_footer();
}

void test_feq(void) {
  print_header("feq");
  
  print_footer();
}

void test_fneq(void) {
  print_header("fneq");
  print_footer();
}

void test_fgt(void) {
  print_header("fgt");
  print_footer();
}

void test_flt(void) {
  print_header("flt");
  print_footer();
}

void test_fgte(void) {
  print_header("fgte");
  print_footer();
}

void test_flte(void) {
  print_header("flte");
  print_footer();
}

int main(void) {
  print_header("FRAC UNIT TESTS");

  test_is_ferror();
  test_is_fzero();
  test_is_fid();

  test_fxy();

  test_fconst();
  test_fadd();
  test_fsub();
  test_fmul();
  test_fdiv();

  test_fconstp();
  test_faddp();
  test_fsubp();
  test_fmulp();
  test_fdivp();

  test_feq();
  test_fneq();
  test_fgt();
  test_flt();
  test_fgte();
  test_flte();

  return 0;
}