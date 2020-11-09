//
// Created by Anna Bakkebø on 31/10/2020.
//

#include <stdbool.h>
#include <stdio.h>
#include "NTT.h"
#include "params.h"
#include "multiplication.h"

void printarray(int *array, int n)
{
    printf("{");
    for(int i=0; i<n;i++){
        printf("%d, ",array[i]);
    }
    printf("}\n");
}

void initiate_NTT_forward(int *roots, int *NTT_forward, int n, int move, bool direction , int start, int  stop){
    if (start==stop){
        return;
    }
    else{
        NTT_forward[0] = roots[n/2];
        int current = start +1;
        int next_move= move*2;
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

void initiate_NTT_roots(int *NTT_forward, int level, int *NTT_roots){
    int polynomials=1;
    for (int i=0;i<level-1; i++){
        polynomials=polynomials*2;
    }
    //printf("%d", polynomials);
    for (int i =0; i<polynomials;i++){
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
void splitting(int *pol, int n, int w){

    int a;
    for(int i=0; i<n; i++){
        a = pol[i+n]*w;
        pol[n+i]= (pol[i]-a)%Q;
        pol[i]=(pol[i]+a)%Q;
    }
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
void merging(int * pol, int n, int w){
    int a;
    for (int i =0; i < n; i++){
        a = pol[i]+pol[n+i];
        pol[n+i]= ((pol[n+i]-pol[i])*w)%Q;
        pol[i]=a;
    }

}

void forward_NTT(int *pol,int  i,int  n,int  m, int *roots, int start, int stop){
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
    int current=start+1;
    i=i/2;
    n=n/2;
    forward_NTT(pol, i,n,m,roots, current, stop);
    forward_NTT(pol+n*2, i+m/4, n, m, roots, current, stop);
}



void forward_NTT2(int *pol, int *NTT_forward,int move, int start, int levels, int n){
    if(start==levels){
        return;
    }
    start++;
    if(move==0){
        //printf("Performing splitting mod %d\n This belongs to the %dth level\n",NTT_forward[0],start-1);
        splitting(pol,n/2,NTT_forward[0]);
        //printarray(pol,n);
        forward_NTT2(pol, NTT_forward+1, 1, start, levels,  n/2);

    }
    else{
        move=move*2;
        for(int i=0; i<move;i++){
            splitting(pol+i*n,n/2,NTT_forward[i]);
            //printf("performing splitting mod %d of the %dth part of the polynomial\n ",NTT_forward[i],n);
        }
        //printf("This belong to the %dth level\n", start-1);
        //printarray(pol,move*n);
        forward_NTT2(pol, NTT_forward+move, move, start, levels,  n/2);
    }

}

void innverse_NTT2(int *pol, int *NTT_forward,int move, int start, int levels, int n){
    if(start==levels){
        return;
    }

    int i=move-1;
    //printf("Working on the %dth level:\n",start);
    for(int j=0;j<move;j++){
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
    move=move/2;
    start++;
    innverse_NTT2(pol, NTT_forward-move,move, start, levels,  n*2);
}

void innverse_finnish(int *pol){
    for(int i=0; i<N; i++){
        pol[i]=(pol[i]*INNVERSE_POWER_OF_TWO)%Q;
    }
}

void printNTTRoots(){
    printf("The NTT roots are {");
    for (int i =0;i<NUM_POLYNOMIALS;i++){
        printf("%d, " ,NTT_roots[i]);
    }
    printf("}\n");
}
void printNTT_Forward(){
    printf("The NTT forward are {");
    for (int i =0;i<NUM_POLYNOMIALS;i++){
        printf("%d, " ,NTT_forward[i]);
    }
    printf("}\n");
}