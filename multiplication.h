//
// Created by Anna Bakkebø on 04/11/2020.
//

#ifndef NTT_MULTIPLICATION_H
#define NTT_MULTIPLICATION_H



/**
 * Updates result to be the multiplication of pol1 and pol2
 *
 * @param pol1 The first polynomials
 * @param pol2  The second polynomials
 * @param result Polynomial/ array where the result is stored
 * @param roots  list of the roost, w, used for the different polynomials X^N+w
 * @param sizeofpol The degree of the modpols
 * @param amountofpol Number of polynomials that will be multiplied
 * @author Anna Bakkebø
 */
void multiplied_NTT(long *pol1, long *pol2, long *result, long* roots, long sizeofpol, long amountofpol);

/**
 * Normal multiplication modulo x^N+1, the result is stored in result
 * @param pol1 The first polynomial
 * @param pol2 The second polynomial
 * @param result The array where the result is stored
 * @param n  The degree of the modulo polynomial
 * @author Anna Bakkebø
 */
void multiplied_normal(long *pol1, long *pol2,long *result, long n);


/**
 * Returns the value base^power
 * @param base value of the base
 * @param power value of the power
 * @return base^power
 */
long lpow(long base, long power);

#endif //NTT_MULTIPLICATION_H
