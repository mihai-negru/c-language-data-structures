/**
 * @file fractions.h
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

#ifndef C_LANGUAGE_DATA_STRUCTURE_PROJECT_FRACTIONS_H_
#define C_LANGUAGE_DATA_STRUCTURE_PROJECT_FRACTIONS_H_

#include <float.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum sign_s {
  plus = 0,
  mins = 1,
  erro = 2
} sign_t;

typedef struct frac_s {
  uint32_t  x;
  uint32_t  y;
  sign_t    s;
} frac_t;

#define     error_frac    (frac_t) { (uint32_t) - 1, (uint32_t)-1, erro }
#define     zero_frac     (frac_t) { (uint32_t)0, (uint32_t)0, plus }
#define     id_frac       (frac_t) { (uint32_t)1, (uint32_t)1, plus }

uint8_t     is_ferror     (frac_t f);
uint8_t     is_fzero      (frac_t f);
uint8_t     is_fid        (frac_t f);

double      feval         (frac_t f);

frac_t      fxy           (uint32_t x, uint32_t y, sign_t s);

frac_t      fadd          (frac_t f1, frac_t f2);
frac_t      fsub          (frac_t f1, frac_t f2);
frac_t      fmul          (frac_t f1, frac_t f2);
frac_t      fdiv          (frac_t f1, frac_t f2);
frac_t      fconst        (frac_t f1, int32_t c);

void        faddp         (frac_t *f1, frac_t f2);
void        fsubp         (frac_t *f1, frac_t f2);
void        fmulp         (frac_t *f1, frac_t f2);
void        fdivp         (frac_t *f1, frac_t f2);
void        fconstp       (frac_t *f1, int32_t c);

uint8_t     feq           (frac_t f1, frac_t f2);
uint8_t     fneq          (frac_t f1, frac_t f2);
uint8_t     fgt           (frac_t f1, frac_t f2);
uint8_t     flt           (frac_t f1, frac_t f2);
uint8_t     fgte          (frac_t f1, frac_t f2);
uint8_t     flte          (frac_t f1, frac_t f2);

#endif /* C_LANGUAGE_DATA_STRUCTURE_PROJECT_FRACTIONS_H_ */
