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

int main() {
    load_roots( roots);
    initiate_NTT_forward(roots, NTT_forward, PRIMITIVE_N / 2, 1, false, 0, LEVEL);
    initiate_NTT_roots(NTT_forward,LEVEL,NTT_roots);
    printNTT_Forward();
    printNTTRoots();
    print_roots();


    /*printf("NTT_forward: {");
    for(int i=0;i<32;i++){
        printf( " %d", NTT_forward[i]);
    }printf("}\n");
    for(int i=0;i<N/2+1;i++){
        printf( "roots[%d] : %d\n", i,roots[i]);
    }*/
    /*int pol1[N]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    forward_NTT(pol1,N/2,N/2,1,roots,0,LEVEL);
    printf("pol1: {");
    for(int i =0; i<N;i++){
        printf("%d, ",pol1[i]);
    }
    printf("}\n");*/

    /*int pol2[N]={1,1,1,1,1,1,1,1};
    printf("pol2: {");
    for(int i =0; i<N;i++){
        printf("%d, ",pol2[i]);
    }
    printf("}\n");

    forward_NTT2(pol2,NTT_forward,0,0,LEVEL,N);
    printf("pol2 after NTT forward2: {");
    for(int i =0; i<N;i++){
        printf("%d, ",pol2[i]);
    }
    printf("}\n");


    innverse_NTT2(pol2,NTT_forward+3,4,0,LEVEL,4);
    printf("pol2 after inverse: {");
    for(int i =0; i<N;i++){
        printf("%d, ",pol2[i]);
    }
    printf("}\n");
    innverse_finnish(pol2);
    printf("pol2 after inverse_finnish: {");
    for(int i =0; i<N;i++){
        printf("%d, ",pol2[i]);
    }
    printf("}\n");*/
    /*
    int pol1[N]={1,3,4,2,1,10,12,4,16,2,3,4,5,6,7,8};
    int pol3[N]={1,6,2,4,3,1,7,3,2,1,5,6,1,3,1,5};
    int result[N]={0};
    forward_NTT2(pol1,NTT_forward,0,0,LEVEL,N);
    forward_NTT2(pol3,NTT_forward,0,0,LEVEL,N);

    printf("pol1 after forward NTT: {");
    for(int i=0;i<N;i++){
        printf("%d, ",pol1[i]);
    }
    printf("}\n");

    printf("pol2 after forward NTT: {");
    for(int i=0;i<N;i++){
        printf("%d, ",pol3[i]);
    }
    printf("}\n");

    multiplied_NTT(pol1,pol3,result,NTT_roots,2,8);


    innverse_NTT2(result,NTT_forward+3,4,0,LEVEL,4);
    innverse_finnish(result);

    printf("result after inverse_finnish: {");
    for(int i =0; i<N;i++){
        if(result[i]>0){
            printf("%d, ",result[i]);
        } else{
            printf("%d, ",result[i]+Q);
        }
    }
    printf("}\n");

    int pol4[N]={1,3,4,2,1,10,12,4,16,2,3,4,5,6,7,8};
    int pol5[N]={1,6,2,4,3,1,7,3,2,1,5,6,1,3,1,5};
    int result2[N]={0};
    printf("pol1: {");
    for(int i =0; i<N;i++){
        printf("%d, ",pol4[i]);
    }
    printf("}\n");
    printf("pol2: {");
    for(int i =0; i<N;i++){
        printf("%d, ",pol5[i]);
    }
    printf("}\n");
    multiplied_normal(pol4,pol5,result2,N);
    printf("Result normal way: {");
    for(int i =0; i<N;i++){
        if(result2[i]>0){
            printf("%d, ",result2[i]);
        } else{
            printf("%d, ",result2[i]+Q);
        }

    }
    printf("}\n");*/

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
    printf("} with { ");
    for(int i=0;i<N;i++){
        printf("%d, ",pol7[i]);
    }
    printf("}\n");
    clock_t begin_normal = clock();
    multiplied_normal(pol6,pol7,resultNormal,N);
    clock_t end_normal = clock();
    printf("Checking that the multiplication is the same after the norm mult {");
    for(int i=0;i<N;i++){
        printf("%d, ",pol6[i]);
    }
    printf("} with { ");
    for(int i=0;i<N;i++){
        printf("%d, ",pol7[i]);
    }
    printf("}\n");
    clock_t begin_NTT = clock();
    forward_NTT2(pol6,NTT_forward,0,0,LEVEL,N);
    forward_NTT2(pol7,NTT_forward,0,0,LEVEL,N);
    //multiplied_NTT(pol1,pol3,result,NTT_roots,2,8);
    multiplied_NTT(pol6,pol7,resultNTT,NTT_roots,2,8);

    innverse_NTT2(resultNTT,NTT_forward+NUM_POLYNOMIALS,4,0,LEVEL,4);

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
            printf("The multiplication failed!\n");
            break;
        }
    }
    double time_spent_norm = (double)(end_normal-begin_normal)/CLOCKS_PER_SEC;
    double time_spent_NTT = (double)(end_NTT-begin_NTT)/CLOCKS_PER_SEC;

    printf("The time spent with normal multiplication is %f\nTime spent with NTT multiplication is %f\n", time_spent_norm,time_spent_NTT);

    int pol8[N]={0};
    for(int i =0;i<N;i++){
        pol8[i]=pol6[i];
    }

    forward_NTT2(pol8,NTT_forward,0,0,LEVEL,N);
    innverse_NTT2(pol8,NTT_forward+128,64,0,LEVEL,4);
    innverse_finnish(pol8);
    for(int i =0;i<N;i++){
        if((pol8[i]-pol6[i])%Q!=0){
            printf("The forward and innverse didn't work\n");
            break;
        }
    }

};