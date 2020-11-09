//
// Created by Anna Bakkebø on 04/11/2020.
//
#include "multiplication.h"
#include "params.h"
#include <stdio.h>

/**
 * Determines what position of the second polynomial that will be multiplied by the j-th position of the first
 * polynomial for the i-th position of the result
 * @param i integer
 * @param j integer
 * @param n Size of the modpol
 * @return The position of the second polynomial that is multiplied.
 * @author Anna Bakkebø
 *
 */
int f(int i, int j, int n) {
    if (i >= j) {
        return i - j;
    } else {
        return i - j + n;
    }
}

/**
 * Updates result to be the multiplication of pol1 and pol2
 *
 * @param pol1 The first polynomial
 * @param pol2  The second polynomial
 * @param result Polynomial/ array where the result is stored
 * @param w  The w when the modpol is X^N+w
 * @param n The degree of the modpol
 * @author Anna Bakkebø
 */
void step_multiplied_NTT(int *pol1, int *pol2, int *result,int w, int n){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int pos2 = f(i, j, n);
            if (j + pos2 >= n) {
                result[i] = (result[i] - w * pol1[j] * pol2[pos2]) % Q;
                Mult_NTT+=2;
                AddSub_NTT+=1;
            } else {
                result[i] = (result[i] + pol1[j] * pol2[pos2]) % Q;
                Mult_NTT+=1;
                AddSub_NTT+=1;
            }
        }
    }
}

void multiplied_NTT(int *pol1, int *pol2, int *result, int* roots, int sizeofpol, int amountofpol){
    int j=0;
    for(int i=0;i<amountofpol;i++){
        /*printf("Multiplying {");
        for(int k=0;k<sizeofpol;k++){
            printf("%d, ", (pol1+j)[k]);
        }
        printf("} with {");
        for(int k=0;k<sizeofpol;k++){
            printf("%d, ", (pol2+j)[k]);
        }
        printf("} using %d\n",roots[i]);*/
        step_multiplied_NTT(pol1+j,pol2+j,result+j,roots[i],sizeofpol);
        j+=sizeofpol;
    }
}


void multiplied_normal(int *pol1, int *pol2,int *result, int n){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int pos2 = f(i, j, n);
            if (j + pos2 >= n) {
                //printf("multipliserer %d og %d og lagrer det i posisjon %d (bruker minus)\n",pol1[j],pol2[pos2],i);
                result[i] = (result[i] -  pol1[j] * pol2[pos2]) % Q;
                Mult_Norm+=1;
                AddSub_Norm+=1;
            } else {
                //printf("multipliserer %d og %d og lagrer det i posisjon %d\n",pol1[j],pol2[pos2],i);
                result[i] = (result[i] + pol1[j] * pol2[pos2]) % Q;
                Mult_Norm+=1;
                AddSub_Norm+=1;
            }
        }
    }
}


void multiplied_rand(int *pol1, int *pol2, int *modpol, int n);