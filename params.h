//
// Created by Anna Bakkebø on 30/10/2020.
//

#ifndef NTT_PARAMS_H
#define NTT_PARAMS_H

#define N (4096*2*2*2*2) // the degree of the modulos polynomial, X^N+1, Maximum N
#define Q 12289 // the modulos
#define COUNTOPERATIONS 0// 0 for not counting and 1 for counting
#if Q==12289
#define ROOT_OF_UNITY 41 // the PRIMITIVE_N th root of unity
#define PRIMITIVE_N 4096 //to what power the root of unity is 1
#define LEVEL 11 // maximum amount of levels
#define INNVERSE_POWER_OF_TWO 6145// the inverse of 2

#define NUM_POLYNOMIALS (2*2*2*2*2*2*2*2*2*2*2)// number of polynomials in the LEVELth level ie 2^LEVEL
#endif
#if Q==4993
#define ROOT_OF_UNITY 66 // the PRIMITIVE_N th root of unity
#define PRIMITIVE_N 128 //to what power the root of unity is 1
#define LEVEL 6 // maximum amount of levels
#define INNVERSE_POWER_OF_TWO 2497// the inverse of 2

#define NUM_POLYNOMIALS 64// number of polynomials in the LEVELth level ie 2^LEVEL
#endif
#if Q==2753
#define ROOT_OF_UNITY 6 // the PRIMITIVE_N th root of unity
#define PRIMITIVE_N 64 //to what power the root of unity is 1
#define LEVEL 5 // maximum amount of levels
#define INNVERSE_POWER_OF_TWO 1377// the inverse of 2

#define NUM_POLYNOMIALS 32// number of polynomials in the LEVELth level ie 2^LEVEL
#endif
#if Q==769
#define ROOT_OF_UNITY 7 // the PRIMITIVE_N th root of unity
#define PRIMITIVE_N 256 //to what power the root of unity is 1
#define LEVEL 7 // maximum amount of levels
#define INNVERSE_POWER_OF_TWO 385// the inverse of 2

#define NUM_POLYNOMIALS 128// number of polynomials in the LEVELth level ie 2^LEVEL
#endif

long roots[PRIMITIVE_N];
/**
 * Stores the different roots of unity in the array roots
 * @param roots The array where the roots of unity will be stored
 */
void load_roots(long *roots);

/**
 * Set the level. This function checks that the level is below the maximum amount of levels and sets the level.
 * If it is over the maximum amount of levels, the level is set to max.
 * @param level number of how many levels you want
 */
void load_level(long level);
/**
 * Returns the value of the level that is set
 * @return level
 */
long get_Level();

/**
 * refreshes the number of polynomials at the lowest level to fit the current level and N
 */
void load_num_polynomials();
/**
 * Returns number of polynomials at the lowest level
 * @return number of polynomials at the lowest level
 */
long get_num_polynomials();

long inverses_power_of_two[LEVEL+1];
/**
 * loading the inverse of two to the power and storing it at the levelth place. Ie inverse of 2^LEVEL is stored at the LEVELETH-1 place
 * @param inverses list where the inverses_power_of_two will be stored
 */
void load_inverses(long *inverses);
/**
 * prints the list of inverses
 */
void print_inverses();
/**
 * Returns the value of N. I.e. the degree of the polynomial
 * @return N
 */
long get_N();
/**
 * Updates the size of N. Has restriction that N can not be bigger the maximum N defined.
 * @param power N=2^power
 */
void set_N(long power);
/**
 * Returns the degree the polynomials at the lowest level is
 * @return degree of polynomials at lowest level
 */
long get_sizeofpol();
/**
 * Returns how many steps should be moved when going through the list for inverse NTT
 * @return move
 */
long get_move();
/**
 * Loads the size of the polynomials at the lowest level, as well as the move value that is used for inverse NTT
 */
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
#endif

#endif //NTT_PARAMS_H
