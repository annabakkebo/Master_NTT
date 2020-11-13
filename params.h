//
// Created by Anna Bakkebø on 30/10/2020.
//

#ifndef NTT_PARAMS_H
#define NTT_PARAMS_H

#define N 256 // the degree of the modulos polynomial, X^N+1
#define Q 12289 // the modulos
#define ROOT_OF_UNITY 41 // the PRIMITIVE_N th root of unity
#define PRIMITIVE_N 4096 //to what power the root of unity is 1
#define LEVEL 7 // how many levels that will be performed
#define INNVERSE_POWER_OF_TWO 12193// the inverse of 2^LEVEL he

#define NUM_POLYNOMIALS 128// number of polynomials in the LEVELth level ie 2^LEVEL
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
