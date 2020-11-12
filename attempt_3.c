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

void random_numb(int *pol, int n){

    for(int i=0;i<n;i++){
        pol[i]=rand()%Q;
    }
}

void checkEqual(int*pol1, int *pol2, int n){
    int result=0;
    for(int i =0;i<n;i++){
        if((pol1[i]-pol2[i])%Q!=0){
            result=1;
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
    int sizeofpol = N/NUM_POLYNOMIALS; //size of the polynomials in the NTT version
    int move = NUM_POLYNOMIALS/2;
    printNTT_Forward();
    printNTTRoots();
    print_roots();




    int pol6[N]={0};
    int pol7[N]={0};
    int resultNormal[N]={0};
    int resultNTT[N]={0};
    srand(time(NULL));
    random_numb(pol6,N);
    random_numb(pol7,N);

    printf("We will now multiply {");
    for(int i=0;i<N;i++){
        printf("%d, ",pol6[i]);
    }
    printf("} \nwith { ");
    for(int i=0;i<N;i++){
        printf("%d, ",pol7[i]);
    }
    printf("}\n");
    clock_t begin_normal = clock();
    multiplied_normal(pol6,pol7,resultNormal,N);
    clock_t end_normal = clock();
    /*printf("Checking that the multiplication is the same after the norm mult {");
    for(int i=0;i<N;i++){
        printf("%d, ",pol6[i]);
    }
    printf("} with { ");
    for(int i=0;i<N;i++){
        printf("%d, ",pol7[i]);
    }
    printf("}\n");*/
    clock_t begin_NTT = clock();
    forward_NTT2(pol6,NTT_forward,0,0,LEVEL,N);
    forward_NTT2(pol7,NTT_forward,0,0,LEVEL,N);
    //multiplied_NTT(pol1,pol3,result,NTT_roots,2,8);

    printf("After NTT forward {");
    for(int i=0;i<N;i++){
        printf("%d, ",pol6[i]);
    }
    printf("} \nwith { ");
    for(int i=0;i<N;i++){
        printf("%d, ",pol7[i]);
    }
    printf("}\n");
    multiplied_NTT(pol6,pol7,resultNTT,NTT_roots,sizeofpol,NUM_POLYNOMIALS);
    printf("Result before innverse { ");
    for(int i=0;i<N;i++){
        printf("%d, ",resultNTT[i]);
    }
    printf("} with NTT mult\n");
    innverse_NTT2(resultNTT,NTT_forward+move-1,move,0,LEVEL,sizeofpol*2);

    innverse_finnish(resultNTT);
    clock_t end_NTT = clock();
    printf("The result is  {");
    for(int i=0;i<N;i++){
        printf("%d, ",resultNormal[i]);
    }
    printf("} with normal mult, \n and { ");
    for(int i=0;i<N;i++){
        printf("%d, ",resultNTT[i]);
    }
    printf("} with NTT mult\n");

    for(int i=0;i<N;i++){
        if((resultNTT[i]-resultNormal[i])%Q!=0){
            printf("The multiplication failed!\n!!!\n!!!\n");
            break;
        }
    }
    double time_spent_norm = (double)(end_normal-begin_normal)/CLOCKS_PER_SEC;
    double time_spent_NTT = (double)(end_NTT-begin_NTT)/CLOCKS_PER_SEC;

    printf("The time spent with normal multiplication is %fs\nTime spent with NTT multiplication is %fs\n", time_spent_norm,time_spent_NTT);

    int pol8[N]={0};
    for(int i =0;i<N;i++){
        pol8[i]=pol6[i];
    }
    printf("Checking forward and innverse of {");
    for(int i =0;i<N;i++){
        printf("%d, ",pol8[i]);
    }
    printf("}\n");
    forward_NTT2(pol8,NTT_forward,0,0,LEVEL,N);
    innverse_NTT2(pol8,NTT_forward+move-1,move,0,LEVEL,sizeofpol*2);
    innverse_finnish(pol8);
    printf("The polynomial after forward and inverse { ");
    for(int i=0;i<N;i++){
        printf("%d, ",pol8[i]);
    }
    printf("}\n");
    checkEqual(pol8,pol6,N);
    for(int i =0;i<N;i++){
        if((pol8[i]-pol6[i])%Q!=0){
            printf("The forward and inverse didn't work\n");
            break;
        }
    }
#if COUNTOPERATIONS==1
    printf("Normal multiplication:\nMultiplications: %d\nAdditions/subtractions: %d\n"
           "NTT multiplication:\nMultiplications: %d\nAdditions/subtractions:%d\n", Mult_Norm,AddSub_Norm,Mult_NTT,AddSub_NTT);
#endif
    int pol9[N]={1};
    for(int i=0;i<N;i++){
        pol9[i]=1;
    }
    printf("The polynomial before forward and inverse { ");
    for(int i=0;i<N;i++){
        printf("%d, ",pol9[i]);
    }
    printf("} \n");
    forward_NTT2(pol9,NTT_forward,0,0,LEVEL,N);
    printf("The polynomial after forward{ ");
    for(int i=0;i<N;i++){
        printf("%d, ",pol9[i]);
    }
    printf("}\n");

    innverse_NTT2(pol9,NTT_forward+move-1,move,0,LEVEL,sizeofpol*2);
    innverse_finnish(pol9);
    printf("The polynomial after forward and inverse { ");
    for(int i=0;i<N;i++){
        printf("%d, ",pol9[i]);
    }
    printf("}\n");
};