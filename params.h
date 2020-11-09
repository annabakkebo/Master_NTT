//
// Created by Anna Bakkebø on 30/10/2020.
//

#ifndef NTT_PARAMS_H
#define NTT_PARAMS_H

#define N 768 // the degree of the modulos polynomial, X^N+1
#define Q 257 // the modulos
#define ROOT_OF_UNITY 3 // the PRIMITIVE_N th root of unity
#define PRIMITIVE_N 256 //to what power the root of unity is 1
#define LEVEL 7 // how many levels that will be performed
#define INNVERSE_POWER_OF_TWO 255// the inverse of 2^LEVEL he

#define NUM_POLYNOMIALS 128// number of polynomials in the 7th level ie 2^LEVEL


int roots[PRIMITIVE_N];
void load_roots(int *roots);
void print_roots();

int Mult_Norm;
int AddSub_Norm;

int Mult_NTT;
int AddSub_NTT;

#endif //NTT_PARAMS_H
