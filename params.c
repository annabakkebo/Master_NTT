//
// Created by Anna Bakkebø on 01/11/2020.
//

#include "params.h"
#include <stdio.h>



void load_roots(int *roots){
    int w = 1;
    int q = Q;
    //printf("q=%d\n",q);
    for (int i = 1; i<PRIMITIVE_N+1; i++){
        w = (w*ROOT_OF_UNITY %q);
        roots[i]=w;
        /*w = w*ROOT_OF_UNITY %q;
        roots[i]=w;*/
    }
}