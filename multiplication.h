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
void multiplied_NTT(int *pol1, int *pol2, int *result, int* roots, int sizeofpol, int amountofpol);

/**
 * Normal multiplication modulo x^N+1, the result is stored in result
 * @param pol1 The first polynomial
 * @param pol2 The second polynomial
 * @param result The array where the result is stored
 * @param n  The degree of the modulo polynomial
 * @author Anna Bakkebø
 */
void multiplied_normal(int *pol1, int *pol2,int *result, int n);

/**
 * Updates pol1 to be the multiplication of pol1 and pol2 when the modulos polynomial is just a random polynomial
 * @param pol1 The first polynomial that is to be multiplied
 * @param pol2  The second polynomial that is to be multiplied
 * @param modpol The polynomial that the multiplication is reduced modulo
 * @param n The degree of the modulos polynomial
 * @author Anna Bakkebø
 */
void multiplied_rand(int *pol1, int *pol2, int *modpol, int n);


#endif //NTT_MULTIPLICATION_H
