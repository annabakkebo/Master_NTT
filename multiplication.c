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
long f(long i, long j, long n) {
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
void step_multiplied_NTT(long *pol1, long *pol2, long *result,long w, long n){
    //int multiplications = 0;
    for (long i = 0; i < n; i++) {
        for (long j = 0; j < n; j++) {
            long pos2 = f(i, j, n);
            if (j + pos2 >= n) {
                result[i] = (result[i] - w * pol1[j] * pol2[pos2]) % Q;
                //multiplications+=2;
#if COUNTOPERATIONS==1
                Mult_NTT+=2;
                AddSub_NTT+=1;
#endif
            } else {
                result[i] = (result[i] + pol1[j] * pol2[pos2]) % Q;
                //multiplications+=1;
#if COUNTOPERATIONS==1
                Mult_NTT+=1;
                AddSub_NTT+=1;
#endif
            }
        }
    }
    //printf("Mult: %d ",multiplications);
}

void multiplied_NTT(long *pol1, long *pol2, long *result, long* roots, long sizeofpol, long amountofpol){
    long j=0;
    //printf("number of polynomials: %ld",amountofpol);
    for(long i=0;i<amountofpol;i++){
        //printf("root used %ld",roots[i]);
        step_multiplied_NTT(pol1+j,pol2+j,result+j,roots[i],sizeofpol);
        j+=sizeofpol;
    }
}
void multiplied_NTT2(struct pol *ppol1, struct pol *ppol2, struct pol *presult, long* roots, long sizeofpol, long amountofpol){
    multiplied_NTT(ppol1->coeffs,ppol2->coeffs,presult->coeffs,roots,sizeofpol,amountofpol);
}

void multiplied_normal(long *pol1, long *pol2,long *result, long n){
    for (long i = 0; i < n; i++) {
        for (long j = 0; j < n; j++) {
            long pos2 = f(i, j, n);
            if (j + pos2 >= n) {
                //printf("multipliserer %d og %d og lagrer det i posisjon %d (bruker minus)\n",pol1[j],pol2[pos2],i);
                result[i] = (result[i] -  pol1[j] * pol2[pos2]) % Q;
#if COUNTOPERATIONS==1
                Mult_Norm+=1;
                AddSub_Norm+=1;
#endif
            } else {
                //printf("multipliserer %d og %d og lagrer det i posisjon %d\n",pol1[j],pol2[pos2],i);
                result[i] = (result[i] + pol1[j] * pol2[pos2]) % Q;
#if COUNTOPERATIONS==1
                Mult_Norm+=1;
                AddSub_Norm+=1;
#endif
            }
        }
    }
}
void multiplied_normal2(struct pol *pol1, struct pol *pol2,struct pol *result, long n){
    multiplied_normal(pol1->coeffs,pol2->coeffs,result->coeffs,n);
}


long lpow(long base, long power){
    long result=1;
    for(long i=0;i<power;i++){
        result=result*base;
    }
    return result;
}

