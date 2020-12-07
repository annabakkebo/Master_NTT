//
// Created by Anna Bakkebø on 05/11/2020.
//
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <zconf.h>
#include <time.h>
#include <unistd.h>
#include "params.h"
#include "NTT.h"
#include "multiplication.h"
#include <time.h>
#include <stdlib.h>
#include "pbPlots/pbPlots.h"
#include "pbPlots/supportLib.h"
#include "MatrixMultiplication.h"


/**
 * Stores a random number from 0 to Q in each of the first n inputs of the array pol
 * @param pol array where the random values will be stored
 * @param n size of the array
 */
void random_numb(long *pol, long n) {

    for (long i = 0; i < n; i++) {
        pol[i] = rand() % Q;
    }
}

/**
 * Check that two arrays are equal mod Q. Prints "The polynomials are NOT equal mod Q" if they are not equal
 * @param pol1 the first array
 * @param pol2 the second array
 * @param n the size of the two arrays
 */
void checkEqual(long *pol1, long *pol2, long n) {
    long result = 0;
    for (long i = 0; i < n; i++) {
        if ((pol1[i] - pol2[i]) % Q != 0) {
            result = 1;
            //printf("Fail at the %ld th element ",i);
        }
    }
    if (result == 0) {
        //printf("The polynomials are equal mod %d ",Q);
    } else {
        printf("The polynomials are NOT equal mod %d ", Q);
    }
}

/**
 * Stores all the necessary NTT values needed.
 * @param power N=2^power
 * @param level number of levels. Should be less than power
 */
void initiate(long power, long level) {
    load_roots(roots); //creates list of the roots of unity
    if (power < level) {
        printf("The power can't be higher than the level. We therefore changed so the level is %ld and power is %ld\n",
               power, level);
        set_N(level);

        load_level(power);
    } else {
        set_N(power);

        load_level(level);
    }

    //printf("2^%ld=%ld\n",get_Level(),get_num_polynomials());
    initiate_NTT_forward(roots, NTT_forward, PRIMITIVE_N / 2, 1, false, 0,
                         get_Level()); //creates list for the rules for NTT forward
    initiate_NTT_roots(NTT_forward, get_Level(),
                       NTT_roots);//list of the roots that is used for the multiplication when the polynomial is in NTT version
    load_inverses(inverses_power_of_two);
    load_size_of_pol_move();
}


int main() {
    initiate(2,1);

    srand(time(NULL));
    struct A_1_marked A_1;
    for(int i=0;i<D;i++){
        for(int j=0;j<(K-D);j++) {
            struct pol pol1;
            random_numb(pol1.coeffs, get_N());
            A_1.pol[i][j] = pol1;
        }
    }

    struct A_2_marked A_2;
    for(int i=0;i<L;i++){
        for(int j=0;j<(K-D-L);j++) {
            struct pol pol1;
            random_numb(pol1.coeffs, get_N());
            A_2.pol[i][j] = pol1;
        }
    }

    printf("A_1");
    for(int i=0;i<D;i++){
        for(int j=0;j<(K-D);j++) {
            printpolynomial(A_1.pol[i][j]);
        }printf("\n");
    }
    printf("A_2");
    for(int i=0;i<L;i++){
        for(int j=0;j<(K-D-L);j++) {
            printpolynomial(A_2.pol[i][j]);
        }printf("\n");
    }

    struct randomness_vector_K r;
    for(int i=0;i<K;i++){
        struct pol pol1;
        random_numb(pol1.coeffs, get_N());
        r.pol[i] = pol1;
    }
    printf("r: ");
    for(int i=0;i<K;i++){
        printpolynomial(r.pol[i]);
    }printf("\n");
    struct comitment_vector_DL c1;
    struct comitment_vector_DL c2;
    printf("The commitment, c1, before committing \n");
    for(int i=0;i< D+L;i++){
        printpolynomial(c1.pol[i]);
    }
    printf("The commitment, c2, before committing \n");
    for(int i=0;i< D+L;i++){
        printpolynomial(c2.pol[i]);
    }
    struct message_vector_L m;
    for(int i=0;i<L;i++){
        struct pol pol1;
        random_numb(pol1.coeffs, get_N());
        m.pol[i] = pol1;
    }


    /*matrixTimesVectorNormalA_1(A_1, r, &c);
    matrixTimesVectorNormalA_2(A_2, r, &c);
    for(int i=0;i< D+L;i++){
        printpolynomial(c.pol[i]);
    }
    for(int i=0;i< L;i++){
        c.pol[i+L]=addPolynomials(c.pol[i],m.pol[i],get_N());
    }*/
    printf("The commitment is \n");
    for(int i=0;i< D+L;i++){
        printpolynomial(c1.pol[i]);
    }

    commitNormal(A_1,A_2,r,m,&c2);
    printf("The commitment using normal is \n");
    for(int i=0;i< D+L;i++){
        printpolynomial(c2.pol[i]);
    }
    commitNTT(A_1,A_2,r,m,&c1);
    printf("The commitment using NTT is \n");
    for(int i=0;i< D+L;i++){
        printpolynomial(c1.pol[i]);
    }

    struct pol resultNormal1; //where the result of pol1 * pol2 will be stored using normal multiplication
    struct pol resultNTT1; //where the result of pol1 * pol2 will be stored using NTT multiplication
    struct pol resultNormal3; //where the result of pol6 * pol7 will be stored using normal multiplication
    struct pol resultNTT3; //where the result of pol6 * pol7 will be stored using NTT multiplication

    printf("Checking multiplication works: ");
    struct pol pol1;
    random_numb(pol1.coeffs, get_N());
    struct pol pol2;
    random_numb(pol2.coeffs, get_N());

    multiplied_normal2(&pol1,&pol2,&resultNormal1,get_N());
    printpolynomial(resultNormal1);
    forward_NTT2(&pol1,NTT_forward,0,0,get_Level(),get_N());
    forward_NTT2(&pol2,NTT_forward,0,0,get_Level(),get_N());
    multiplied_NTT2(&pol1,&pol2,&resultNTT1,NTT_roots,get_sizeofpol(),get_num_polynomials());
    inverse_NTT2(&resultNTT1,NTT_forward+get_move()-1, get_move(), 0, get_Level(),get_sizeofpol());
    printpolynomial(resultNTT1);




    return 0;
}