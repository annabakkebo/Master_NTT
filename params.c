//
// Created by Anna Bakkebø on 01/11/2020.
//

#include "params.h"
#include <stdio.h>



void load_roots(int *roots){
    int w = 1;
    int q = Q;
    //printf("q=%d\n",q);
    roots[0]=1;
    for (int i = 1; i<PRIMITIVE_N; i++){
        w = (w*ROOT_OF_UNITY %q);
        roots[i]=w;
        /*w = w*ROOT_OF_UNITY %q;
        roots[i]=w;*/
    }
}

void print_roots(){
    printf("The roots of are: {");
    for(int i=0;i<PRIMITIVE_N;i++){
        printf("%d, ",roots[i]);
    }
    printf("}\n");
}