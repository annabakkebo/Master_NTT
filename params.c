//
// Created by Anna Bakkebø on 01/11/2020.
//

#include "params.h"
#include <stdio.h>




void load_roots(long *roots){
    long w = 1;
    long q = Q;
    //printf("q=%d\n",q);
    roots[0]=1;
    for (long i = 1; i<PRIMITIVE_N; i++){
        w = (w*ROOT_OF_UNITY %q);
        roots[i]=w;
        /*w = w*ROOT_OF_UNITY %q;
        roots[i]=w;*/
    }
}

void print_roots(){
    printf("The roots of are: {");
    for(long i=0;i<PRIMITIVE_N;i++){
        printf("%d, ",roots[i]);
    }
    printf("}\n");
}
#if COUNTOPERATIONS==1
Mult_Norm=0;
AddSub_Norm=0;

Mult_NTT=0;
AddSub_NTT=0;
#endif