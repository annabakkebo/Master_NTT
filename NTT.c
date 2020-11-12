//
// Created by Anna Bakkebø on 31/10/2020.
//

#include <stdbool.h>
#include <stdio.h>
#include "NTT.h"
#include "params.h"
#include "multiplication.h"


void printarray(long *array, long n)
{
    printf("{");
    for(long i=0; i<n;i++){
        printf("%d, ",array[i]);
    }
    printf("}\n");
}

void initiate_NTT_forward(long *roots, long *NTT_forward, long n, long move, bool direction , long start, long  stop){
    if (start==stop){
        return;
    }
    else{
        NTT_forward[0] = roots[n/2];
        long current = start +1;
        long next_move= move*2;
        if(direction){
            initiate_NTT_forward(roots, NTT_forward + next_move, n / 2, next_move, true, current, stop);
            initiate_NTT_forward(roots, NTT_forward + next_move + 1, n / 2 + PRIMITIVE_N / 2, next_move + 1, false,
                                 current, stop);
        } else{
            initiate_NTT_forward(roots, NTT_forward + next_move - 1, n / 2, next_move - 1, true, current, stop);
            initiate_NTT_forward(roots, NTT_forward + next_move, n / 2 + PRIMITIVE_N / 2, next_move, false, current,
                                 stop);
        }
    }
}

void initiate_NTT_roots(long *NTT_forward, long level, long *NTT_roots){
    long polynomials=1;
    for (long i=0;i<level-1; i++){
        polynomials=polynomials*2;
    }
    //printf("%d", polynomials);
    for (long i =0; i<polynomials;i++){
        NTT_roots[i*2]=-NTT_forward[i+polynomials-1];
        NTT_roots[i*2+1]=NTT_forward[i+polynomials-1];
    }
}

/**
 * Updates the array of polynomial to contain the two splitted polynomials. <br>
 * The first half of the array will be modulos x^n-w, the second half modulos x^n+w
 *
 * @param pol polynomial that will be splitted
 * @param n the degree of the two new polynomials, ie half the degree of the input polynomial
 * @param w from x^n+w or x^n-w, ie the w in the two new modulos polynomials
 *
 * @author Anna Bakkebø
 */
void splitting(long *pol, long n, long w){

    long a;
    for(long i=0; i<n; i++){
        a = pol[i+n]*w;//printf("%d, ",pol[i]);
        pol[n+i]= (pol[i]-a)%Q;
        pol[i]=(pol[i]+a)%Q;
#if COUNTOPERATIONS==1
        Mult_NTT+=1;
        AddSub_NTT+=1;
#endif
    }
    //printf("\n");
}

/**
 *Updates the array to contain the merging of the two previous polynomials.
 *
 * @param pol array containing the two polynomials that will be merged
 * @param n degree of the two polynomials
 * @param w some power of the root of unity used for merging
 *
 * @author Anna Bakkebø
 */
void merging(long * pol, long n, long w){
    long a;
    for (long i =0; i < n; i++){
        a = pol[i]+pol[n+i];
        pol[n+i]= ((pol[n+i]-pol[i])*w)%Q;
        pol[i]=a;
#if COUNTOPERATIONS==1
        Mult_NTT+=1;
        AddSub_NTT+=2;
#endif
    }

}

void forward_NTT(long *pol,long  i,long  n,long  m, long *roots, long start, long stop){
    if(start==stop){
        return;
    }
    else{
        /*for (int j = 0; j < n; j++) {
            int a = pol[j + n] * roots[i];
            pol[n + j] = pol[j] - a;
            pol[j] = pol[j] + a;
        }*/
        splitting(pol, n, roots[i]);
        //printf("performing splitting mod %d\n",roots[i]);
    }
    long current=start+1;
    i=i/2;
    n=n/2;
    forward_NTT(pol, i,n,m,roots, current, stop);
    forward_NTT(pol+n*2, i+m/4, n, m, roots, current, stop);
}



void forward_NTT2(long *pol, long *NTT_forward,long move, long start, long levels, long n){
    if(start==levels){
        return;
    }
    start++;
    if(move==0){
        //printarray(pol,n);
        //printf("Performing splitting mod %d\n This belongs to the %dth level\n \n \n  The polnomial ends up being:",NTT_forward[0],start-1);
        splitting(pol,n/2,NTT_forward[0]);
        //printarray(pol,n);
        forward_NTT2(pol, NTT_forward+1, 1, start, levels,  n/2);

    }
    else{
        move=move*2;
        for(long i=0; i<move;i++){
            splitting(pol+i*n,n/2,NTT_forward[i]);
            //printf("performing splitting mod %d of the %dth part of the polynomial\n ",NTT_forward[i],n);
        }
        //printf("This belong to the %dth level\n", start-1);
        //printarray(pol,move*n);
        forward_NTT2(pol, NTT_forward+move, move, start, levels,  n/2);
    }

}

void innverse_NTT2(long *pol, long *NTT_forward,long move, long start, long levels, long n){
    if(start==levels){
        return;
    }

    long i=move-1;
    //printf("Working on the %dth level:\n",start);
    for(long j=0;j<move;j++){
        //printf("Merging ");
        //printarray(pol+j*n,n);
        //printf(" using %d\n",NTT_forward[i]);
        merging(pol+j*n,n/2,NTT_forward[i]);
        i=i-1;
    }
    /*printf("Resulting in {");
    for(int i =0; i<N;i++){
        printf("%d, ",pol[i]);
    }
    printf("}\n");*/
    //printf("performing the %dth level of NTT innverse\n",start);
    move=move/2;
    start++;
    innverse_NTT2(pol, NTT_forward-move,move, start, levels,  n*2);
}

void innverse_finnish(long *pol){
    for(long i=0; i<N; i++){
        pol[i]=(pol[i]*INNVERSE_POWER_OF_TWO)%Q;
#if COUNTOPERATIONS==1
        Mult_NTT+=1;
#endif
    }
}

void printNTTRoots(){
    printf("The NTT roots are {");
    for (long i =0;i<NUM_POLYNOMIALS;i++){
        printf("%d, " ,NTT_roots[i]);
    }
    printf("}\n");
}
void printNTT_Forward(){
    printf("The NTT forward are {");
    for (long i =0;i<NUM_POLYNOMIALS;i++){
        printf("%d, " ,NTT_forward[i]);
    }
    printf("}\n");
}