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


/**
 * Stores a random number from 0 to Q in each of the first n inputs of the array pol
 * @param pol array where the random values will be stored
 * @param n size of the array
 */
void random_numb(long *pol, long n) {

    for (long i = 0; i < n; i++) {
        pol[i] = rand() % Q;
    }
}

/**
 * Check that two arrays are equal mod Q. Prints "The polynomials are NOT equal mod Q" if they are not equal
 * @param pol1 the first array
 * @param pol2 the second array
 * @param n the size of the two arrays
 */
void checkEqual(long *pol1, long *pol2, long n) {
    long result = 0;
    for (long i = 0; i < n; i++) {
        if ((pol1[i] - pol2[i]) % Q != 0) {
            result = 1;
            //printf("Fail at the %ld th element ",i);
        }
    }
    if (result == 0) {
        //printf("The polynomials are equal mod %d ",Q);
    } else {
        printf("The polynomials are NOT equal mod %d ", Q);
    }
}

/**
 * Stores all the necessary NTT values needed.
 * @param power N=2^power
 * @param level number of levels. Should be less than power
 */
void initiate(long power, long level) {
    load_roots(roots); //creates list of the roots of unity
    if (power < level) {
        printf("The power can't be higher than the level. We therefore changed so the level is %ld and power is %ld\n",
               power, level);
        set_N(level);

        load_level(power);
    } else {
        set_N(power);

        load_level(level);
    }

    //printf("2^%ld=%ld\n",get_Level(),get_num_polynomials());
    initiate_NTT_forward(roots, NTT_forward, PRIMITIVE_N / 2, 1, false, 0,
                         get_Level()); //creates list for the rules for NTT forward
    initiate_NTT_roots(NTT_forward, get_Level(),
                       NTT_roots);//list of the roots that is used for the multiplication when the polynomial is in NTT version
    load_inverses(inverses_power_of_two);
    load_size_of_pol_move();
}

int main() {
    long pol1[N] = {0}; // polynomial that will be multiplied with pol2
    long pol2[N] = {0}; // polynomial that will be multiplied with pol1
    long pol6[N] = {0}; // polynomial that will be multiplied with pol7
    long pol7[N] = {0}; // polynomial that will be multiplied with pol6
    long resultNormal1[N] = {0}; //where the result of pol1 * pol2 will be stored using normal multiplication
    long resultNTT1[N] = {0}; //where the result of pol1 * pol2 will be stored using NTT multiplication
    long resultNormal3[N] = {0}; //where the result of pol6 * pol7 will be stored using normal multiplication
    long resultNTT3[N] = {0}; //where the result of pol6 * pol7 will be stored using NTT multiplication
    srand(time(NULL));

    clock_t begin_normal; //start time for the normal multiplication
    clock_t end_normal; // end time for the normal multiplication
    clock_t begin_NTT; // start time for NTT multiplication
    //clock_t begin_mult_NTT = clock();
    //clock_t end_mult_NTT = clock();
    clock_t end_NTT = clock(); //end time for NTT multiplication

    double time_spent_norm; // time spent during normal multiplication
    double time_spent_NTT; // time spent during NTT multiplication
    double time_spent_mult_NTT;

    int difference; //how much faster the NTT time was compared to normal multiplication

    double xaxis[14] = {0}; //where the values for the x-axis will be stored
    double yaxisNTT[14] = {0}; // values corresponding to xaxis with running time for NTT multiplication
    double yaxisNormal[14] = {0}; // values corresponding to xaxis with running time for normal multiplication
    int timestesting = 2; //how many times the multiplication should be tested. The higher the value the more accurat expectant value we get
    for (int j = 0; j < timestesting; j++) {
        for (long i = 3; i < 17; i++) {

#if COUNTOPERATIONS == 1
            Mult_Norm=0;
            AddSub_Norm=0;
            Mult_NTT=0;
            AddSub_NTT=0;
#endif

            initiate(i, i - 1);
            printf("Levels: %ld     N: %ld\n", get_Level(), get_N());

            random_numb(pol1, N);
            random_numb(pol2, N);
            /*random_numb(pol3,N);
            random_numb(pol4,N);*/
            random_numb(pol6, N);
            random_numb(pol7, N);

            for (int j = 0; j < N; j++) {
                resultNormal1[j] = 0;
                resultNTT1[j] = 0;

                /*resultNormal2[j]=0;
                resultNTT2[j]=0;*/

                resultNormal3[j] = 0;
                resultNTT3[j] = 0;
            }

            printf("Multiplying the ''Normal way''...   ");
            begin_normal = clock();

            multiplied_normal(pol1, pol2, resultNormal1, get_N());
            /*multiplied_normal(pol3,pol4,resultNormal2,get_N());*/
            multiplied_normal(pol6, pol7, resultNormal3, get_N());
            end_normal = clock();

            printf("Multiplying using NTT...\n");
            begin_NTT = clock();
            //printf("first pol:");
            forward_NTT(pol1, NTT_forward, 0, 0, get_Level(), get_N());
            //printf("second pol:");
            forward_NTT(pol2, NTT_forward, 0, 0, get_Level(), get_N());
            /*forward_NTT2(pol3,NTT_forward,0,0,get_Level(),get_N());
            forward_NTT(pol4,NTT_forward,0,0,get_Level(),get_N());*/
            //printf("third pol:");
            forward_NTT(pol6, NTT_forward, 0, 0, get_Level(), get_N());
            //printf("fourth pol:");
            forward_NTT(pol7, NTT_forward, 0, 0, get_Level(), get_N());

            //begin_mult_NTT = clock();

            multiplied_NTT(pol1, pol2, resultNTT1, NTT_roots, get_sizeofpol(), get_num_polynomials());
            /*multiplied_NTT(pol3,pol4,resultNTT2,NTT_roots,get_sizeofpol(),get_num_polynomials());*/
            multiplied_NTT(pol6, pol7, resultNTT3, NTT_roots, get_sizeofpol(), get_num_polynomials());
            //end_mult_NTT = clock();

            inverse_NTT(resultNTT1, NTT_forward + get_move() - 1, get_move(), 0, get_Level(), get_sizeofpol() * 2);
            /*inverse_NTT(resultNTT2,NTT_forward+get_move()-1,get_move(),0,get_Level(),get_sizeofpol()*2);*/
            inverse_NTT(resultNTT3, NTT_forward + get_move() - 1, get_move(), 0, get_Level(), get_sizeofpol() * 2);


            inverse_finnish(resultNTT1, inverses_power_of_two[get_Level()]);
            /*inverse_finnish(resultNTT2,inverses_power_of_two[get_Level()]);*/
            inverse_finnish(resultNTT3, inverses_power_of_two[get_Level()]);
            end_NTT = clock();

            time_spent_norm = (double) (end_normal - begin_normal) / CLOCKS_PER_SEC;
            time_spent_NTT = (double) (end_NTT - begin_NTT) / CLOCKS_PER_SEC;
            //time_spent_mult_NTT = (double)(end_mult_NTT-begin_mult_NTT)/CLOCKS_PER_SEC;


            checkEqual(resultNTT1, resultNormal1, get_N());
            checkEqual(resultNTT3, resultNormal3, get_N());


            printf("Normal multiplication: %fs      "
                   "NTT multiplication: %fs     ", time_spent_norm, time_spent_NTT);

            difference = (end_normal - begin_normal) / (end_NTT - begin_NTT);
            printf("The NTT multiplication is roughly %d times faster\n", difference);
            xaxis[i - 3] = (double) i;
            yaxisNTT[i - 3] += (double) (end_NTT - begin_NTT) / 2;
            yaxisNormal[i - 3] += (double) (end_normal - begin_normal) / 2;
#if COUNTOPERATIONS == 1
            printf("Multiplication:\n"
                   "Normal: %lld        NTT: %ld       Normal:mult+adsub  %lld\n"
                   "Add/sub:                                                  \n"
                   "Normal: %lld       NTT: %ld      NTT:mult+adsub  %ld            \n",
                   Mult_Norm,Mult_NTT,Mult_Norm+AddSub_Norm,AddSub_Norm,AddSub_NTT,Mult_NTT+AddSub_NTT);

#endif
            printf("\n");


        }
    }


    printf("\n\n");
    printf("");
    for (int i = 0; i < 14; i++) {
        yaxisNTT[i] = log2(yaxisNTT[i] / timestesting);
        yaxisNormal[i] = log2(yaxisNormal[i] / timestesting);
        printf("$2^{%lf}$ & $2^{%lf}$ & $2^{%lf}$ \\\\ \\hline \n", xaxis[i], yaxisNormal[i], yaxisNTT[i]);
    }
    ScatterPlotSeries *series1 = GetDefaultScatterPlotSeriesSettings();
    series1->xs = xaxis;
    series1->xsLength = 14;
    series1->ys = yaxisNTT;
    series1->ysLength = 14;
    series1->linearInterpolation = true;
    /*series1->lineType = L"dashed";
    series1->lineTypeLength = wcslen(series1->lineType);*/
    series1->lineThickness = 2;
    series1->color = GetGray(0.9);

    ScatterPlotSeries *series2 = GetDefaultScatterPlotSeriesSettings();
    series2->xs = xaxis;
    series2->xsLength = 14;
    series2->ys = yaxisNormal;
    series2->ysLength = 14;
    series2->linearInterpolation = true;
    /*series2->lineType = L"dashed";
    series2->lineTypeLength = wcslen(series1->lineType);*/
    series2->lineThickness = 2;
    series2->color = GetGray(0.2);

    ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
    settings->width = 600;
    settings->height = 400;
    settings->autoBoundaries = true;
    settings->autoPadding = true;
    settings->title = L"grey - normal mult    black - NTT mult";
    settings->titleLength = wcslen(settings->title);
    settings->xLabel = L"2^Y";
    settings->xLabelLength = wcslen(settings->xLabel);
    settings->yLabel = L"2^X";
    settings->yLabelLength = wcslen(settings->yLabel);

    ScatterPlotSeries *s1[] = {series1, series2};
    settings->scatterPlotSeries = s1;
    settings->scatterPlotSeriesLength = 2;

    RGBABitmapImageReference *canvasReference = CreateRGBABitmapImageReference();
    DrawScatterPlotFromSettings(canvasReference, settings);

    size_t length;
    double *pngdata = ConvertToPNG(&length, canvasReference->image);
    WriteToFile(pngdata, length, "plot7.png");
    DeleteImage(canvasReference->image);


    return 0;
}