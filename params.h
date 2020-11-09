//
// Created by Anna Bakkebø on 30/10/2020.
//

#ifndef NTT_PARAMS_H
#define NTT_PARAMS_H

#define N 16 // the degree of the modulos polynomial, X^N+1
#define Q 17 // the modulos
#define ROOT_OF_UNITY 6 // the PRIMITIVE_N th root of unity
#define PRIMITIVE_N 16 //to what power the root of unity is 1
#define LEVEL 3 // how many levels that will be performed
#define INNVERSE_POWER_OF_TWO -2// the inverse of 2^LEVEL


int roots[PRIMITIVE_N];
void load_roots(int *roots);

#endif //NTT_PARAMS_H
