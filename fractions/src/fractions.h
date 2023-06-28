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

/**
 * @brief Enum that encodes the truth values `false` and `true`,
 * however we add another truth value as `unknown`, which is used when
 * comparing nans or infinities, because mathematicaly this represents
 * an indeterminate state.
 * 
 */
typedef enum fbool_s {
  ffalse    = 0,
  ftrue     = 1,
  funknown  = 2
} fbool_t;

/**
 * @brief Enum to represent the sign of a fraction.
 * 
 */
typedef enum sign_s {
  plus  = 0,           /* Plus sign + */
  mins  = 1,           /* Minus sign - */
  nan   = 2            /* Error sign */
} sign_t;

/**
 * @brief Structure to represent a fraction.
 * The fraction is represented as a numerator and a denominator.
 * The fraction is reduced to the tuple (x, y, s), where x is the numenrator,
 * y is the denominator and s is the sign of the fraction. Both x and y
 * are positive integers.
 * 
 */
typedef struct frac_s {
  uint32_t  x;
  uint32_t  y;
  sign_t    s;
} frac_t;


/**
 * @brief Basic fractions, the plus and minus infinity fraction which represent
 * the standard infinity for real numbers (encoded as 1/0 fraction), nan fraction
 * which represents an illegal operation like infinity over infinity or inifnity
 * minus infinity and so on, the zero fraction and the identity fraction.
 * 
 */
#define     nan_frac      (frac_t) { (uint32_t)-1, (uint32_t)-1, nan }
#define     pinf_frac     (frac_t) { 1, 0, plus }
#define     minf_frac     (frac_t) { 1, 0, mins }
#define     zero_frac     (frac_t) { (uint32_t)0, (uint32_t)-1, plus }
#define     id_frac       (frac_t) { (uint32_t)1, (uint32_t)1, plus }


/**
 * @brief Checks wheter a fraction is not a number or not.
 * 
 * @param f fraction structure.
 * @return fbool_t ftrue if fraction is not a number, ffalse otherwise.
 */
fbool_t     is_fnan      (frac_t f);

/**
 * @brief Checks wheter a fraction is infinity fraction or not.
 * 
 * @param f fraction structure.
 * @return fbool_t ftrue if the fraction is infinity fraction, ffalse otherwise.
 */
fbool_t     is_finf      (frac_t f);

/**
 * @brief Checks wheter a fraction is positive infinity fraction or not.
 * 
 * @param f fraction structure.
 * @return fbool_t ftrue if the fraction is positive infinity fraction, ffalse otherwise.
 */
fbool_t     is_fpinf     (frac_t f);

/**
 * @brief Checks wheter a fraction is negative infinity fraction or not.
 * 
 * @param f fraction structure.
 * @return fbool_t ftrue if the fraction is negative infinity fraction, ffalse otherwise.
 */
fbool_t     is_fminf     (frac_t f);

/**
 * @brief Checks if the fraction is positive or negative.
 * 
 * @param f fraction structure.
 * @return fbool_t ftrue if fraction is positive, ffalse otherwise.
 */
fbool_t     is_fpositive  (frac_t f);

/**
 * @brief Checks wheter a fraction is a zero fraction or not.
 * The fraction has to have the x field equal to zero, the y and sign
 * are ignored, by the function.
 *
 * @param f fraction structure.
 * @return fbool_t ftrue if the fraction is a zero fraction, ffalse otherwise.
 */
fbool_t     is_fzero      (frac_t f);

/**
 * @brief Checks wheter a fraction is an identity fraction or not.
 * The fraction has to have the x and y fields equal to 1 and the sign
 * has to be `plus`.
 * 
 * @param f fraction structure.
 * @return fbool_t ftrue if the fraction is an identity fraction, ffalse otherwise.
 */
fbool_t     is_fid        (frac_t f);


/**
 * @brief Evaluates the fraction structure to a double value.
 * 
 * @param f fraction structure to valuate.
 * @return double the real value of the fraction.
 */
double      feval         (frac_t f);

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
frac_t      fxy           (uint32_t x, uint32_t y, sign_t s);


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
frac_t      fconst        (frac_t f1, int32_t c);

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
frac_t      fadd          (frac_t f1, frac_t f2);

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
frac_t      fsub          (frac_t f1, frac_t f2);

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
frac_t      fmul          (frac_t f1, frac_t f2);

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
frac_t      fdiv          (frac_t f1, frac_t f2);


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
void        fconstp       (frac_t *f1, int32_t c);

/**
 * @brief Adds two fractions and stores the irreductible fraction f = a/b in the first fraction,
 * where a and b are coprime. The function does the exact operation as the '+=' operator.
 * f1 += f2.
 * 
 * @param f1 pointer to first fraction.
 * @param f2 second fraction.
 *
 */
void        faddp         (frac_t *f1, frac_t f2);

/**
 * @brief Subtracts two fractions and stores the irreductible fraction f = a/b in the first fraction,
 * where a and b are coprime. The function does the exact operation as the '-=' operator.
 * f1 -= f2.
 * 
 * @param f1 pointer to first fraction.
 * @param f2 second fraction.
 *
 */
void        fsubp         (frac_t *f1, frac_t f2);

/**
 * @brief Multiplies two fractions and stores the irreductible fraction f = a/b in the first fraction,
 * where a and b are coprime. The function does the exact operation as the '*=' operator.
 * f1 *= f2.
 * 
 * @param f1 pointer to first fraction.
 * @param f2 second fraction.
 *
 */
void        fmulp         (frac_t *f1, frac_t f2);

/**
 * @brief Divides two fractions and stores the irreductible fraction f = a/b in the first fraction,
 * where a and b are coprime. The function does the exact operation as the '/=' operator.
 * f1 /= f2.
 * 
 * @param f1 pointer to first fraction.
 * @param f2 second fraction.
 *
 */
void        fdivp         (frac_t *f1, frac_t f2);


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
fbool_t     feq           (frac_t f1, frac_t f2);

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
fbool_t     fneq          (frac_t f1, frac_t f2);

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
fbool_t     fgt           (frac_t f1, frac_t f2);

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
fbool_t     flt           (frac_t f1, frac_t f2);

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
fbool_t     fgte          (frac_t f1, frac_t f2);

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
fbool_t     flte          (frac_t f1, frac_t f2);

#endif /* C_LANGUAGE_DATA_STRUCTURE_PROJECT_FRACTIONS_H_ */
