//
// Created by Anna Bakkebø on 30/10/2020.
//

#ifndef NTT_NTT_H
#define NTT_NTT_H

#include <stdbool.h>
#include "params.h"

/**
 * Stores the roots of unity that is used for the forward splitting
 * @param roots List of roots of unity
 * @param NTT_forward  List where the forward NTT will be stored
 * @param n  The previous root of unity that is used x^N- w^n
 * @param move How many positions forward the next step should be stored
 * @param direction Whether you´re splitting to the right or left. Left: true, right: false, start with false.
 * @param start The level where it is started
 * @param stop  How many levels the splitting should be
 */
void initiate_NTT_forward(long *roots, long *NTT_forward, long n, long move, bool direction , long start, long  stop);

/**
 * Creates list of roots that the multiplication will do modulo when doing multiplication after NTTforward
 * @param NTT_forward The NTT_forward list
 * @param level How many levels that will be computed
 * @param NTT_roots The list where the roots will be stored
 */
void initiate_NTT_roots(long *NTT_forward, long level, long *NTT_roots);

long NTT_forward[NUM_POLYNOMIALS*2]; // Array of the roots of unity that will be used for forward NTT
long NTT_roots[NUM_POLYNOMIALS]; // Array of roots of unity that is used at the lowest level


/**
 * Computes forward NTT
 * @param pol  array of the coeefficients of the polynomial that the forward NTT will be performed on
 * @param NTT_forward list of the different roots of unity that it will be splitted modulo
 * @param move initiated at 0, this showes how long to move forward on the list of the forward NTT
 * @param start how many levels performed, start at 0
 * @param levels  how many levels the NTT will be performed
 * @param n  The length of the polynomial that will be splitted
 */
void forward_NTT(long *pol, long *NTT_forward, long move, long start, long levels, long n);
/**
 * Computes forward NTT
 * @param pol  the polynomial that the forward NTT will be performed on
 * @param NTT_forward list of the different roots of unity that it will be splitted modulo
 * @param move initiated at 0, this showes how long to move forward on the list of the forward NTT
 * @param start how many levels performed, start at 0
 * @param levels  how many levels the NTT will be performed
 * @param n  The length of the polynomial that will be splitted
 */
void forward_NTT2(struct pol *polynomial, long *NTT_forward, long move, long start, long levels, long n);

/**
 * Computes inverse NTT
 * @param pol Array of the coefficients of the polynomial that the inverse NTT will be performed on
 * @param NTT_forward The list for the roots of unity for the forward NTT + the integer so that the start point is the
 *                      list for the last level
 * @param move The length of the roots for the first level
 * @param start initiated at 0
 * @param levels how many levels that will be performed
 * @param n Twice the length of each of the small polynomial
 */
void inverse_NTT(long *pol, long *NTT_forward, long move, long start, long levels, long n);
/**
 * Computes inverse NTT
 * @param pol The polynomial that the inverse NTT will be performed on
 * @param NTT_forward The list for the roots of unity for the forward NTT + the integer so that the start point is the
 *                      list for the last level
 * @param move The length of the roots for the first level
 * @param start initiated at 0
 * @param levels how many levels that will be performed
 * @param n Twice the length of each of the small polynomial
 */
void inverse_NTT2(struct pol *polynomial, long *NTT_forward, long move, long start, long levels, long n);


/**
 * Multiplying by the inverse of 2^LEVEL
 * @param pol Array of the coefficients of the polynomial right after the inverse_NTT to finnish the inverse algorithm
 * @param inverse The inverse of the power of two
 */
void inverse_finnish(long *pol, int inverse);
/**
 * Multiplying by the inverse of 2^LEVEL
 * @param pol The polynomial right after the inverse_NTT to finnish the inverse algorithm
 * @param inverse The inverse of the power of two
 */
void inverse_finnish2(struct pol *polynomial, int inverse);

/**
 * printing the NTTroots variable
 */
void printNTTRoots();

/**
 * printing the NTT_forward variable
 */
void printNTT_Forward();

void printNTT_Forward();
void printNTTRoots();
#endif //NTT_NTT_H
