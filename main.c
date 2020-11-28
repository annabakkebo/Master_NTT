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
#include "pbPlots/pbPlots.h"
#include "pbPlots/supportLib.h"



void random_numb(long *pol, long n){

    for(long i=0;i<n;i++){
        pol[i]=rand()%Q;
    }
}

void checkEqual(long*pol1, long *pol2, long n){
    long result=0;
    for(long i =0;i<n;i++){
        if((pol1[i]-pol2[i])%Q!=0){
            result=1;
            //printf("Fail at the %ld th element ",i);
        }
    }
    if(result==0){
        //printf("The polynomials are equal mod %d ",Q);
    } else{
        printf("The polynomials are NOT equal mod %d ",Q);
    }
}

void initiate(long power, long level){
    load_roots( roots); //creates list of the roots of unity
    if(power<level){
        printf("The power can't be higher than the level. I therefore changed so the level is %ld and power is %ld\n",power,level);
        set_N(level);

        load_level(power);
    }else{
        set_N(power);

        load_level(level);
    }

    //printf("2^%ld=%ld\n",get_Level(),get_num_polynomials());
    initiate_NTT_forward(roots, NTT_forward, PRIMITIVE_N / 2, 1, false, 0, get_Level()); //creates list for the rules for NTT forward
    initiate_NTT_roots(NTT_forward,get_Level(),NTT_roots);//list of the roots that is used for the multiplication when the polynomial is in NTT version
    load_inverses(inverses_power_of_two);
    load_size_of_pol_move();
}

int main() {
    long pol1[N]={0};
    long pol2[N]={0};
    long pol6[N]={0};
    long pol7[N]={0};
    long resultNormal1[N]={0};
    long resultNTT1[N]={0};
    long resultNormal3[N]={0};
    long resultNTT3[N]={0};
    srand(time(NULL));

    clock_t begin_normal;
    clock_t end_normal;
    clock_t begin_NTT;

    clock_t begin_mult_NTT = clock();
    clock_t end_mult_NTT = clock();
    clock_t end_NTT = clock();

    double time_spent_norm;
    double time_spent_NTT;
    double time_spent_mult_NTT ;

    int difference;

    double xaxis[14]={0};
    double yaxisNTT[14]={0};
    double yaxisNormal[14]={0};

    for(long i=0;i<5;i++){

#if COUNTOPERATIONS==1
        Mult_Norm=0;
        AddSub_Norm=0;
        Mult_NTT=0;
        AddSub_NTT=0;

        addsub_NTT_multiplication=0;
        addsub_NTT_inverse=0;
        addsub_NTT_forward=0;
        mult_NTT_forward=0;
        mult_NTT_multiplication=0;
        mult_NTT_inverse=0;
#endif

        initiate(10,10-i);
        printf("Levels: %ld     N: %ld\n",get_Level(),get_N());

        random_numb(pol1,N);
        random_numb(pol2,N);
        /*random_numb(pol3,N);
        random_numb(pol4,N);*/
        random_numb(pol6,N);
        random_numb(pol7,N);

        for(int j=0;j<N;j++){
            resultNormal1[j]=0;
            resultNTT1[j]=0;

            /*resultNormal2[j]=0;
            resultNTT2[j]=0;*/

            resultNormal3[j]=0;
            resultNTT3[j]=0;
        }

        printf("Multiplying the ''Normal way''...   ");
        begin_normal = clock();

        multiplied_normal(pol1,pol2,resultNormal1,get_N());
        /*multiplied_normal(pol3,pol4,resultNormal2,get_N());*/
        //multiplied_normal(pol6,pol7,resultNormal3,get_N());
        end_normal = clock();

        printf("Multiplying using NTT...\n");
        begin_NTT = clock();
        //printf("first pol:");
        forward_NTT2(pol1,NTT_forward,0,0,get_Level(),get_N());
        //printf("second pol:");
        forward_NTT2(pol2,NTT_forward,0,0,get_Level(),get_N());
        /*forward_NTT2(pol3,NTT_forward,0,0,get_Level(),get_N());
        forward_NTT2(pol4,NTT_forward,0,0,get_Level(),get_N());*/
        //printf("third pol:");
        //forward_NTT2(pol6,NTT_forward,0,0,get_Level(),get_N());
        //printf("fourth pol:");
        //forward_NTT2(pol7,NTT_forward,0,0,get_Level(),get_N());

        //begin_mult_NTT = clock();

        multiplied_NTT(pol1,pol2,resultNTT1,NTT_roots,get_sizeofpol(),get_num_polynomials());
        /*multiplied_NTT(pol3,pol4,resultNTT2,NTT_roots,get_sizeofpol(),get_num_polynomials());*/
        //multiplied_NTT(pol6,pol7,resultNTT3,NTT_roots,get_sizeofpol(),get_num_polynomials());
        //end_mult_NTT = clock();

        innverse_NTT2(resultNTT1,NTT_forward+get_move()-1,get_move(),0,get_Level(),get_sizeofpol()*2);
        /*innverse_NTT2(resultNTT2,NTT_forward+get_move()-1,get_move(),0,get_Level(),get_sizeofpol()*2);*/
        //innverse_NTT2(resultNTT3,NTT_forward+get_move()-1,get_move(),0,get_Level(),get_sizeofpol()*2);


        innverse_finnish(resultNTT1,inverses_power_of_two[get_Level()]);
        /*innverse_finnish(resultNTT2,inverses_power_of_two[get_Level()]);*/
        //innverse_finnish(resultNTT3,inverses_power_of_two[get_Level()]);
        end_NTT = clock();

        time_spent_norm = (double)(end_normal-begin_normal)/CLOCKS_PER_SEC;
        time_spent_NTT = (double)(end_NTT-begin_NTT)/CLOCKS_PER_SEC;
        //time_spent_mult_NTT = (double)(end_mult_NTT-begin_mult_NTT)/CLOCKS_PER_SEC;


        checkEqual(resultNTT1,resultNormal1,get_N());
        //checkEqual(resultNTT3,resultNormal3,get_N());


        printf("Normal multiplication: %fs      "
               "NTT multiplication: %fs     ", time_spent_norm,time_spent_NTT);

        difference= (end_normal-begin_normal)/(end_NTT-begin_NTT);
        printf("The NTT multiplication is roughly %d times faster\n",difference);
        xaxis[i-3]=(double )i;
        yaxisNTT[i-3]+=((double)(end_NTT-begin_NTT)/2);
        yaxisNormal[i-3]+=((double)(end_normal-begin_normal)/2);
#if COUNTOPERATIONS==1
        printf("Multiplication:\n"
               "Normal: %lld        NTT: %ld       Normal:mult+adsub  %lld\n"
               "Add/sub:                                                  \n"
               "Normal: %lld       NTT: %ld      NTT:mult+adsub  %ld            \n",
               Mult_Norm,Mult_NTT,Mult_Norm+AddSub_Norm,AddSub_Norm,AddSub_NTT,Mult_NTT+AddSub_NTT);
        printf("Multiplications :   "
               "Forward: %ld        Multiplication: %ld         Inverse: %ld\n"
               "Addition:           "
               "Forward: %ld        Multiplication: %ld         Inverse: %ld\n",
               mult_NTT_forward, mult_NTT_multiplication, mult_NTT_inverse,addsub_NTT_forward,addsub_NTT_multiplication,addsub_NTT_inverse);

#endif
        printf("\n");


        }




    return 0;
}