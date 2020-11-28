//
// Created by Anna Bakkebø on 30/10/2020.
//

#ifndef NTT_PARAMS_H
#define NTT_PARAMS_H

#define N (4096*2*2*2*2) // the degree of the modulos polynomial, X^N+1, Maximum N
#define Q 12289 // the modulos
#define ROOT_OF_UNITY 41 // the PRIMITIVE_N th root of unity
#define PRIMITIVE_N 4096 //to what power the root of unity is 1
#define LEVEL 11 // maximum amount of levels
#define INNVERSE_POWER_OF_TWO 6145// the inverse of 2

#define NUM_POLYNOMIALS (2*2*2*2*2*2*2*2*2*2*2)// number of polynomials in the LEVELth level ie 2^LEVEL
#define COUNTOPERATIONS 1// 0 for not counting and 1 for counting


long roots[PRIMITIVE_N];
/**
 * Stores the different roots of unity in the array roots
 * @param roots The array where the roots of unity will be stored
 */
void load_roots(long *roots);


void load_level(long level);
long get_Level();

void load_num_polynomials();
long get_num_polynomials();

long inverses_power_of_two[LEVEL+1];
/**
 * loading the inverse of two to the power and storing it at the levelth place. Ie inverse of 2^LEVEL is stored at the LEVELETH-1 place
 * @param inverses list where the inverses_power_of_two will be stored
 */
void load_inverses(long *inverses);
void print_inverses();

long get_N();
void set_N(long power);

long get_sizeofpol();

long get_move();

void load_size_of_pol_move();

/**
 * printing the list of roots of unity
 */
void print_roots();
#if COUNTOPERATIONS==1
long long Mult_Norm;
long long AddSub_Norm;

long Mult_NTT;
long AddSub_NTT;

long mult_NTT_forward;
long mult_NTT_inverse;
long mult_NTT_multiplication;

long addsub_NTT_forward;
long addsub_NTT_inverse;
long addsub_NTT_multiplication;
#endif

#endif //NTT_PARAMS_H
