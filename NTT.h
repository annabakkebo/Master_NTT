//
// Created by Anna Bakkebø on 30/10/2020.
//

#ifndef NTT_NTT_H
#define NTT_NTT_H


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
void initiate_NTT_forward(long *roots, long *NTT_forward, long n, long move, bool direction , long start, long stop);

/**
 * Creates list of roots that the multiplication will do modulo when doing multiplication after NTTforward
 * @param NTT_forward The NTT_forward list
 * @param level How many levels that will be computed
 * @param NTT_roots The list where the roots will be stored
 */
void initiate_NTT_roots(long *NTT_forward, long level, long *NTT_roots);

long NTT_forward[NUM_POLYNOMIALS*2];
long NTT_roots[NUM_POLYNOMIALS];

/**
 * Computes the forward NTT. This doesn´t work as of 8.11.20
 *
 * @param pol polynomial that will be splitted
 * @param i half the degree that the primitive root of unity is raised to the powr of
 * @param n half the degree of the mod polynomial
 * @param m don't remember what this is used to
 * @param roots list of the primitive roots of unity
 * @param start the current level. Will be initiated at 0
 * @param stop how many levels the forward NTT should be
 * @author Anna Bakkebø
 * @date 8.11.20
 */
void forward_NTT(long *pol, long i,long  n,long m, long * roots, long start, long stop);

/**
 * Computes forward NTT
 * @param pol  the polynomial that the forward NTT will be performed on
 * @param NTT_forward list of the different roots of unity that it will be splitted modulo
 * @param move initiated at 0, this showes how long to move forward on the list of the forward NTT
 * @param start how many levels performed, start at 0
 * @param levels  how many levels the NTT will be performed
 * @param n  The length of the polynomial that will be splitted
 */
void forward_NTT2(long *pol, long *NTT_forward,long move, long start, long levels, long n);

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
void innverse_NTT2(long *pol, long *NTT_forward,long move, long start, long levels, long n);
void inverse_NTT();

/**
 * Multiplying by the innverse of 2^LEVEL
 * @param pol The polynomial right after the innverse_NTT2 to finnish the inverse algorithm
 */
void innverse_finnish(long *pol);

/**
 * printing the NTTroots variable
 */
void printNTTRoots();

/**
 * printing the NTT_forward variable
 */
void printNTT_Forward();

#endif //NTT_NTT_H
