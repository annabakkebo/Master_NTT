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



void random_numb(long *pol, long n){

    for(long i=0;i<n;i++){
        pol[i]=rand()%Q;
    }
}

void checkEqual(long*pol1, long *pol2, long n){
    long result=0;
    for(long i =0;i<n;i++){
        if((pol1[i]-pol2[i])%Q!=0){
            result=1;
            //printf("Fail at the %ld th element ",i);
        }
    }
    if(result==0){
        printf("The polynomials are equal mod %d\n",Q);
    } else{
        printf("The polynomials ar NOT equal mod %d\n",Q);
    }
}

int main() {
    load_roots( roots); //creates list of the roots of unity
    initiate_NTT_forward(roots, NTT_forward, PRIMITIVE_N / 2, 1, false, 0, LEVEL); //creates list for the rules for NTT forward
    initiate_NTT_roots(NTT_forward,LEVEL,NTT_roots);//list of the roots that is used for the multiplication when the polynomial is in NTT version
    load_inverses(inverses_power_of_two);
    print_inverses();
    long n = N;
    long sizeofpol = n/NUM_POLYNOMIALS; //size of the polynomials in the NTT version
    long move = NUM_POLYNOMIALS/2;
    load_level(11);
    load_num_polynomials();
    printf("2^%ld=%ld\n",get_Level(),get_num_polynomials());

    /*long pol1[N]={0};
    long pol2[N]={0};
    long pol3[N]={0};
    long pol4[N]={0};*/
    long pol6[N]={0};
    long pol7[N]={0};
    /*long resultNormal1[N]={0};
    long resultNTT1[N]={0};
    long resultNormal2[N]={0};
    long resultNTT2[N]={0};*/
    long resultNormal3[N]={0};
    long resultNTT3[N]={0};
    srand(time(NULL));
    /*random_numb(pol1,N);
    random_numb(pol2,N);
    random_numb(pol3,N);
    random_numb(pol4,N);*/
    random_numb(pol6,N);
    random_numb(pol7,N);

    printf("Multiplying the ''Normal way''\n");
    clock_t begin_normal = clock();
    /*multiplied_normal(pol1,pol2,resultNormal1,N);
    multiplied_normal(pol3,pol4,resultNormal2,N);*/
    multiplied_normal(pol6,pol7,resultNormal3,N);
    clock_t end_normal = clock();

    printf("Multiplying using NTT\n");
    clock_t begin_NTT = clock();
    /*forward_NTT2(pol1,NTT_forward,0,0,LEVEL,N);
    forward_NTT2(pol2,NTT_forward,0,0,LEVEL,N);
    forward_NTT2(pol3,NTT_forward,0,0,LEVEL,N);
    forward_NTT2(pol4,NTT_forward,0,0,LEVEL,N);*/
    forward_NTT2(pol6,NTT_forward,0,0,LEVEL,N);
    forward_NTT2(pol7,NTT_forward,0,0,LEVEL,N);

    clock_t begin_mult_NTT = clock();
    /*multiplied_NTT(pol1,pol2,resultNTT1,NTT_roots,sizeofpol,NUM_POLYNOMIALS);
    multiplied_NTT(pol3,pol4,resultNTT2,NTT_roots,sizeofpol,NUM_POLYNOMIALS);*/
    multiplied_NTT(pol6,pol7,resultNTT3,NTT_roots,sizeofpol,NUM_POLYNOMIALS);
    clock_t end_mult_NTT = clock();
    /*innverse_NTT2(resultNTT1,NTT_forward+move-1,move,0,LEVEL,sizeofpol*2);
    innverse_NTT2(resultNTT2,NTT_forward+move-1,move,0,LEVEL,sizeofpol*2);*/
    innverse_NTT2(resultNTT3,NTT_forward+move-1,move,0,LEVEL,sizeofpol*2);

    printf("%ld",inverses_power_of_two[LEVEL]);
    /*innverse_finnish(resultNTT1,inverses_power_of_two[LEVEL]);
    innverse_finnish(resultNTT2, inverses_power_of_two[LEVEL]);*/
    innverse_finnish(resultNTT3,inverses_power_of_two[LEVEL]);
    clock_t end_NTT = clock();

    double time_spent_norm = (double)(end_normal-begin_normal)/CLOCKS_PER_SEC;
    double time_spent_NTT = (double)(end_NTT-begin_NTT)/CLOCKS_PER_SEC;
    double time_spent_mult_NTT = (double)(end_mult_NTT-begin_mult_NTT)/CLOCKS_PER_SEC;

    /*checkEqual(resultNTT1,resultNormal1,N);
    checkEqual(resultNTT2,resultNormal2,N);*/
    checkEqual(resultNTT3,resultNormal3,N);


    printf("The time spent with normal multiplication is %fs\n"
           "Time spent with NTT multiplication is %fs\n"
           "Time spent on the actual multiplication part of NTT is %fs\n", time_spent_norm,time_spent_NTT,time_spent_mult_NTT);

    int difference= (end_normal-begin_normal)/(end_NTT-begin_NTT);
    printf("The NTT multiplication is roughly %d times faster",difference);
#if COUNTOPERATIONS==1
    printf("Normal multiplication:\nMultiplications: %d\nAdditions/subtractions: %d\n"
           "NTT multiplication:\nMultiplications: %d\nAdditions/subtractions:%d\n", Mult_Norm,AddSub_Norm,Mult_NTT,AddSub_NTT);
#endif
    return 0;
}