//
// Created by Anna Bakkebø on 01/11/2020.
//

#include "params.h"
#include <stdio.h>
#include "multiplication.h"




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

long the_level;
void load_level(long level){
    if(level<LEVEL){
        the_level=level;
    } else{
        the_level=LEVEL;
    }
}
long get_Level(){
    return the_level;
}

long num_polynomials;
void load_num_polynomials(){
    num_polynomials=lpow(2,get_Level());
}
long get_num_polynomials(){
    return num_polynomials;
};


void load_inverses(long *inverses){
    long w=1;
    inverses[0]=1;
    for (long i = 1; i<LEVEL+1; i++){
        w = (w*INNVERSE_POWER_OF_TWO %Q);
        inverses[i]=w;
    }
}

void print_inverses(){
    printf("The inverses are: {");
    for(long i=0;i<LEVEL+1;i++){
        printf("%ld, ",inverses_power_of_two[i]);
    }
    printf("}\n");
}

void print_roots(){
    printf("The roots of are: {");
    for(long i=0;i<PRIMITIVE_N;i++){
        printf("%ld, ",roots[i]);
    }
    printf("}\n");
}
#if COUNTOPERATIONS==1
Mult_Norm=0;
AddSub_Norm=0;

Mult_NTT=0;
AddSub_NTT=0;
#endif