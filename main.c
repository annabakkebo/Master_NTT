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
#include "MatrixMultiplication.h"


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
    initiate(10,3);

    srand(time(NULL));
    struct A_1_marked *A_1 = malloc(sizeof(struct A_1_marked));
    for(int i=0;i<D;i++){
        for(int j=0;j<(K-D);j++) {
            struct pol pol1;
            random_numb(pol1.coeffs, get_N());
            A_1->pol[i][j] = pol1;
        }
    }

    struct A_2_marked *A_2=malloc(sizeof(struct A_2_marked));
    for(int i=0;i<L;i++){
        for(int j=0;j<(K-D-L);j++) {
            struct pol pol1;
            random_numb(pol1.coeffs, get_N());
            A_2->pol[i][j] = pol1;
        }
    }

    /*printf("A_1");
    for(int i=0;i<D;i++){
        for(int j=0;j<(K-D);j++) {
            printpolynomial(A_1->pol[i][j]);
        }printf("\n");
    }
    printf("A_2");
    for(int i=0;i<L;i++){
        for(int j=0;j<(K-D-L);j++) {
            printpolynomial(A_2->pol[i][j]);
        }printf("\n");
    }*/

    struct randomness_vector_K *r = malloc(sizeof(struct randomness_vector_K));
    for(int i=0;i<K;i++){
        struct pol pol1;
        random_numb(pol1.coeffs, get_N());
        r->pol[i] = pol1;
    }
    /*printf("r: ");
    for(int i=0;i<K;i++){
        printpolynomial(r->pol[i]);
    }printf("\n");*/
    struct comitment_vector_DL *c1=malloc(sizeof(struct comitment_vector_DL));
    struct comitment_vector_DL *c2=malloc(sizeof(struct comitment_vector_DL));
    /*printf("The commitment, c1, before committing \n");
    for(int i=0;i< D+L;i++){
        printpolynomial(c1->pol[i]);
    }

    printf("The commitment, c2, before committing \n");
    for(int i=0;i< D+L;i++){
        printpolynomial(c2->pol[i]);
    }*/

    struct message_vector_L *m = malloc(sizeof(struct message_vector_L));
    for(int i=0;i<L;i++){
        struct pol pol1;
        random_numb(pol1.coeffs, get_N());
        m->pol[i] = pol1;
    }

    /*printf("The commitment is \n");
    for(int i=0;i< D+L;i++){
        printpolynomial(c1->pol[i]);
    }

    pcommitNormal(A_1,A_2,r,m,c2);
    printf("The commitment using normal is \n");
    for(int i=0;i< D+L;i++){
        printpolynomial(c2->pol[i]);
    }
    pcommitNTT(A_1,A_2,r,m,c1);
    printf("The commitment using NTT is \n");
    for(int i=0;i< D+L;i++){
        printpolynomial(c1->pol[i]);
    }
    for(int i=0;i<D+L;i++){
        checkEqual(c1->pol,c2->pol,get_N());
    }*/

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
    int timestesting=6;
    for (int j = 0; j < timestesting; j++) {
        for (long i = 2; i < 16; i++) {

#if COUNTOPERATIONS == 1
            Mult_Norm=0;
            AddSub_Norm=0;
            Mult_NTT=0;
            AddSub_NTT=0;
#endif

            initiate(i, i - 1);
            printf("Levels: %ld     N: %ld\n", get_Level(), get_N());
            for(int i=0;i<D;i++){
                for(int j=0;j<(K-D);j++) {
                    struct pol pol1;
                    random_numb(pol1.coeffs, get_N());
                    A_1->pol[i][j] = pol1;
                }
            }
            for(int i=0;i<L;i++){
                for(int j=0;j<(K-D-L);j++) {
                    struct pol pol1;
                    random_numb(pol1.coeffs, get_N());
                    A_2->pol[i][j] = pol1;
                }
            }
            for(int i=0;i<K;i++){
                struct pol pol1;
                random_numb(pol1.coeffs, get_N());
                r->pol[i] = pol1;
            }
            for(int i=0;i<L;i++){
                struct pol pol1;
                random_numb(pol1.coeffs, get_N());
                m->pol[i] = pol1;
            }


            printf("Committing the ''Normal way''...   ");
            begin_normal = clock();
            pcommitNormal(A_1,A_2,r,m,c2);
            end_normal = clock();

            printf("Committing using NTT...\n");
            begin_NTT = clock();
            pcommitNTT(A_1,A_2,r,m,c1);

            end_NTT = clock();

            time_spent_norm = (double) (end_normal - begin_normal) / CLOCKS_PER_SEC;
            time_spent_NTT = (double) (end_NTT - begin_NTT) / CLOCKS_PER_SEC;
            //time_spent_mult_NTT = (double)(end_mult_NTT-begin_mult_NTT)/CLOCKS_PER_SEC;


            for(int i=0;i<D+L;i++){
                checkEqual(c1->pol->coeffs,c2->pol->coeffs,get_N());
            }


            printf("Normal multiplication: %fs      "
                   "NTT multiplication: %fs     ", time_spent_norm, time_spent_NTT);

            difference = (end_normal - begin_normal) / (end_NTT - begin_NTT);
            printf("The NTT multiplication is roughly %d times faster\n", difference);
            xaxis[i - 2] = (double) i;
            yaxisNTT[i - 2] += (double) (end_NTT - begin_NTT) / 2;
            yaxisNormal[i - 2] += (double) (end_normal - begin_normal) / 2;
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
    FILE *f = fopen("plot19.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    for (int i = 0; i < 14; i++) {
        yaxisNTT[i] = log2(yaxisNTT[i] / timestesting);
        yaxisNormal[i] = log2(yaxisNormal[i] / timestesting);
        fprintf(f,"$2^{%lf}$ & $2^{%lf}$ & $2^{%lf}$ \\\\ \\hline \n", xaxis[i], yaxisNormal[i], yaxisNTT[i]);
    }

    fclose(f);

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
    WriteToFile(pngdata, length, "plot19.png");
    DeleteImage(canvasReference->image);

    free(A_2);
    free(A_1);
    free(m);
    free(r);
    return 0;
}