//
// Created by Anna Bakkebø on 30/10/2020.
//

#ifndef NTT_PARAMS_H
#define NTT_PARAMS_H

#define N 16384 // the degree of the modulos polynomial, X^N+1
#define Q 40961 // the modulos
#define ROOT_OF_UNITY 12 // the PRIMITIVE_N th root of unity
#define PRIMITIVE_N 8192 //to what power the root of unity is 1
#define LEVEL 12 // how many levels that will be performed
#define INNVERSE_POWER_OF_TWO 40951// the inverse of 2^LEVEL he

#define NUM_POLYNOMIALS 4096// number of polynomials in the LEVELth level ie 2^LEVEL
#define COUNTOPERATIONS 0// 0 for not counting and 1 for counting


long roots[PRIMITIVE_N];
void load_roots(long *roots);
void print_roots();
#if COUNTOPERATIONS==1
int Mult_Norm;
int AddSub_Norm;

int Mult_NTT;
int AddSub_NTT;
#endif

#endif //NTT_PARAMS_H
