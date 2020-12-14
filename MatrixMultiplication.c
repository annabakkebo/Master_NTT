//
// Created by Anna Bakkebø on 06/12/2020.
//
#include "MatrixMultiplication.h"
#include "NTT.h"
#include <stdio.h>
#include "multiplication.h"

struct pol addPolynomials(struct pol pol1, struct pol pol2, long size){
    struct pol result;
    for(int i =0;i<size; i++){
        result.coeffs[i]= (pol1.coeffs[i]+pol2.coeffs[i])%Q;
#if COUNTOPERATIONS==1
        AddSub_Norm+=1;
#endif
    }
    return result;
}

struct pol multiplyRowByVectorNormal(struct pol *row, struct pol *vector, int size){
    struct pol result;
    struct pol zeropol;
    for(int i=0;i<get_N();i++){
        zeropol.coeffs[i]=0;
    }
    result=zeropol;
    for(int i = 0; i < size; i++){
        struct pol step_result = zeropol;
        /*printf("multiplying ");
        printpolynomial(row[i]);
        printf(" with ");
        printpolynomial(vector[i]);
        printf("before multiplication");
        printpolynomial(step_result);*/
        multiplied_normal2(&row[i],&vector[i],&step_result,get_N());
        /*printf("Resulting in ");
        printpolynomial(step_result);*/
        result = addPolynomials(result, step_result, get_N());
        /*printf("Added together it ends up being");
        printpolynomial(result);
        printf("\n");*/
    }
    return result;
}


void matrixTimesVectorNormalA_1(struct A_1_marked A_1_marked, struct randomness_vector_K randomness,
                                struct comitment_vector_DL *commit) {
    for(int i=0; i< D;i++){
        struct pol step_result;
        step_result= multiplyRowByVectorNormal(A_1_marked.pol[i], randomness.pol+D, K - D);
        /*//printf("step_result:");
        //printpolynomial(step_result);
        //printf("Then we are adding ");
        //printpolynomial(randomness.pol[i]);*/
        step_result=addPolynomials(step_result,randomness.pol[i],get_N());
        /*//printpolynomial(step_result);
        //printf("Now storing it in the %d component of commit\n",i);*/
        commit->pol[i]=step_result;
        //printpolynomial(commit->pol[i]);
    }
}

void matrixTimesVectorNormalA_2(struct A_2_marked A_2_marked, struct randomness_vector_K randomness,
                                struct comitment_vector_DL *commit) {
    for(int i=0; i< L;i++){
        struct pol step_result;
        step_result= multiplyRowByVectorNormal(A_2_marked.pol[i], randomness.pol+(D+L), K - D-L);
        //printf("step_result:");
        //printpolynomial(step_result);
        //printf("Then we are adding ");
        //printpolynomial(randomness.pol[i]);
        step_result=addPolynomials(step_result,randomness.pol[i+D],get_N());
        //printpolynomial(step_result);
        //printf("Now storing it in the %d component of commit\n",i);
        commit->pol[i+D]=step_result;
        //printpolynomial(commit->pol[i+D]);
    }
}

void commitNormal(struct A_1_marked A_1_marked, struct A_2_marked A_2_marked, struct randomness_vector_K randomness,
                  struct message_vector_L message, struct comitment_vector_DL *commit) {
    matrixTimesVectorNormalA_1(A_1_marked,randomness,commit);
    matrixTimesVectorNormalA_2(A_2_marked,randomness, commit);
    for(int i=0; i<D; i++){
        commit->pol[i+L]=addPolynomials(commit->pol[i],message.pol[i],get_N());
    }
}

void pmatrixTimesVectorNormalA_1(struct A_1_marked *A_1_marked, struct randomness_vector_K *randomness,
                                struct comitment_vector_DL *commit) {
    for(int i=0; i< D;i++){
        struct pol step_result;
        step_result= multiplyRowByVectorNormal(A_1_marked->pol[i], randomness->pol+D, K - D);
        /*//printf("step_result:");
        //printpolynomial(step_result);
        //printf("Then we are adding ");
        //printpolynomial(randomness.pol[i]);*/
        step_result=addPolynomials(step_result,randomness->pol[i],get_N());
        /*//printpolynomial(step_result);
        //printf("Now storing it in the %d component of commit\n",i);*/
        commit->pol[i]=step_result;
        //printpolynomial(commit->pol[i]);
    }
}

void pmatrixTimesVectorNormalA_2(struct A_2_marked *A_2_marked, struct randomness_vector_K *randomness,
                                struct comitment_vector_DL *commit) {
    for(int i=0; i< L;i++){
        struct pol step_result;
        step_result= multiplyRowByVectorNormal(A_2_marked->pol[i], randomness->pol+(D+L), K - D-L);
        //printf("step_result:");
        //printpolynomial(step_result);
        //printf("Then we are adding ");
        //printpolynomial(randomness.pol[i]);
        step_result=addPolynomials(step_result,randomness->pol[i+D],get_N());
        //printpolynomial(step_result);
        //printf("Now storing it in the %d component of commit\n",i);
        commit->pol[i+D]=step_result;
        //printpolynomial(commit->pol[i+D]);
    }
}

void pcommitNormal(struct A_1_marked *A_1_marked, struct A_2_marked *A_2_marked, struct randomness_vector_K *randomness,
                  struct message_vector_L *message, struct comitment_vector_DL *commit) {
    pmatrixTimesVectorNormalA_1(A_1_marked,randomness,commit);
    pmatrixTimesVectorNormalA_2(A_2_marked,randomness, commit);
    for(int i=0; i<D; i++){
        commit->pol[i+L]=addPolynomials(commit->pol[i],message->pol[i],get_N());
    }
}



struct pol multiplyRowByVectorNTT(struct pol *row, struct pol *vector, int size){
    struct pol result;
    struct pol zeropol;
    for(int i=0;i<get_N();i++){
        zeropol.coeffs[i]=0;
    }
    result=zeropol;
    for(int i = 0; i < size; i++){
        struct pol step_result = zeropol;
        /*printf("multiplying ");
        printpolynomial(row[i]);
        printf(" with ");
        printpolynomial(vector[i]);
        printf("before multiplication");
        printpolynomial(step_result);*/
        multiplied_NTT2(&row[i],&vector[i],&step_result,NTT_roots, get_sizeofpol(),get_num_polynomials());
        /*printf("Resulting in ");
        printpolynomial(step_result);*/
        result = addPolynomials(result, step_result, get_N());
        /*printf("Added together it ends up being");
        printpolynomial(result);
        printf("\n");*/
    }
    return result;
}


void forwardNTT_matrices_vector(struct A_1_marked *A_1_marked, struct A_2_marked *A_2_marked,
                                struct randomness_vector_K *randomness) {
    for(int i = 0; i<D;i++){
        for(int j = 0;j<K-D;j++){
            forward_NTT2(&A_1_marked->pol[i][j], NTT_forward, 0,0,get_Level(),get_N());
        }
    }
    for(int i = 0; i<L;i++){
        for(int j = 0;j<K-D-L;j++){
            forward_NTT2(&A_2_marked->pol[i][j], NTT_forward, 0,0,get_Level(),get_N());
        }
    }
    for(int i=0;i<K;i++){
        forward_NTT2(&randomness->pol[i], NTT_forward,0,0,get_Level(),get_N());
    }
}

void matrixTimesVectorNTTA_1(struct A_1_marked A_1_marked, struct randomness_vector_K randomness,
                             struct comitment_vector_DL *commit) {
    for(int i=0; i< D;i++){
        struct pol step_result;
        step_result= multiplyRowByVectorNTT(A_1_marked.pol[i], randomness.pol+D, K - D);
        //printf("step_result:");
        //printpolynomial(step_result);
        //printf("Then we are adding ");
        //printpolynomial(randomness.pol[i]);
        step_result=addPolynomials(step_result,randomness.pol[i],get_N());
        //printpolynomial(step_result);
        //printf("Now storing it in the %d component of commit\n",i);
        commit->pol[i]=step_result;
        //printpolynomial(commit->pol[i]);
    }
}

void matrixTimesVectorNTTA_2(struct A_2_marked A_2_marked, struct randomness_vector_K randomness,
                             struct comitment_vector_DL *commit) {
    for(int i=0; i< L;i++){
        struct pol step_result;
        step_result= multiplyRowByVectorNTT(A_2_marked.pol[i], randomness.pol+(D+L), K - D-L);
        //printf("step_result:");
        //printpolynomial(step_result);
        //printf("Then we are adding ");
        //printpolynomial(randomness.pol[i]);
        step_result=addPolynomials(step_result,randomness.pol[i+D],get_N());
        //printpolynomial(step_result);
        //printf("Now storing it in the %d component of commit\n",i);
        commit->pol[i+D]=step_result;
        //printpolynomial(commit->pol[i]);
    }
}


void commitNTT(struct A_1_marked A_1_marked, struct A_2_marked A_2_marked, struct randomness_vector_K randomness,
               struct message_vector_L message, struct comitment_vector_DL *commit) {
    /*printf("A_1");
    for(int i=0;i<D;i++){
        for(int j=0;j<(K-D);j++) {
            printpolynomial(A_1_marked.pol[i][j]);
        }printf("\n");
    }
    printf("A_2");
    for(int i=0;i<L;i++){
        for(int j=0;j<(K-D-L);j++) {
            printpolynomial(A_2_marked.pol[i][j]);
        }printf("\n");
    }*/
    forwardNTT_matrices_vector(&A_1_marked,&A_2_marked,&randomness);
    /*printf("A_1");
    for(int i=0;i<D;i++){
        for(int j=0;j<(K-D);j++) {
            printpolynomial(A_1_marked.pol[i][j]);
        }printf("\n");
    }
    printf("A_2");
    for(int i=0;i<L;i++){
        for(int j=0;j<(K-D-L);j++) {
            printpolynomial(A_2_marked.pol[i][j]);
        }printf("\n");
    }*/
    matrixTimesVectorNTTA_1(A_1_marked,randomness,commit);
    matrixTimesVectorNTTA_2(A_2_marked,randomness,commit);
    printf("commitment after matrix multiplication\n");
    /*for(int i=0;i<D+L;i++){
        printpolynomial(commit->pol[i]);
    }*/
    inverseNTT_commitmentvectorDL(commit);
    /*for(int i=0;i<D+L;i++){
        printpolynomial(commit->pol[i]);
    }*/
    for(int i=0; i<D; i++){
        commit->pol[i+L]=addPolynomials(commit->pol[i],message.pol[i],get_N());
    }
}

void inverseNTT_commitmentvectorDL(struct comitment_vector_DL *vector) {
    for(int i=0;i<D+L;i++){
        inverse_NTT2(&vector->pol[i],NTT_forward+get_move()-1,get_move(),0,get_Level(),get_sizeofpol()*2);
        inverse_finnish2(&vector->pol[i],inverses_power_of_two[get_Level()]);
    }
}

void pmatrixTimesVectorNTTA_1(struct A_1_marked *A_1_marked, struct randomness_vector_K *randomness,
                             struct comitment_vector_DL *commit) {
    for(int i=0; i< D;i++){
        struct pol step_result;
        step_result= multiplyRowByVectorNTT(A_1_marked->pol[i], randomness->pol+D, K - D);
        //printf("step_result:");
        //printpolynomial(step_result);
        //printf("Then we are adding ");
        //printpolynomial(randomness.pol[i]);
        step_result=addPolynomials(step_result,randomness->pol[i],get_N());
        //printpolynomial(step_result);
        //printf("Now storing it in the %d component of commit\n",i);
        commit->pol[i]=step_result;
        //printpolynomial(commit->pol[i]);
    }
}

void pmatrixTimesVectorNTTA_2(struct A_2_marked *A_2_marked, struct randomness_vector_K *randomness,
                             struct comitment_vector_DL *commit) {
    for(int i=0; i< L;i++){
        struct pol step_result;
        step_result= multiplyRowByVectorNTT(A_2_marked->pol[i], randomness->pol+(D+L), K - D-L);
        //printf("step_result:");
        //printpolynomial(step_result);
        //printf("Then we are adding ");
        //printpolynomial(randomness.pol[i]);
        step_result=addPolynomials(step_result,randomness->pol[i+D],get_N());
        //printpolynomial(step_result);
        //printf("Now storing it in the %d component of commit\n",i);
        commit->pol[i+D]=step_result;
        //printpolynomial(commit->pol[i]);
    }
}


void pcommitNTT(struct A_1_marked *A_1_marked, struct A_2_marked *A_2_marked, struct randomness_vector_K *randomness,
               struct message_vector_L *message, struct comitment_vector_DL *commit) {
    /*printf("A_1");
    for(int i=0;i<D;i++){
        for(int j=0;j<(K-D);j++) {
            printpolynomial(A_1_marked.pol[i][j]);
        }printf("\n");
    }
    printf("A_2");
    for(int i=0;i<L;i++){
        for(int j=0;j<(K-D-L);j++) {
            printpolynomial(A_2_marked.pol[i][j]);
        }printf("\n");
    }*/
    forwardNTT_matrices_vector(A_1_marked,A_2_marked,randomness);
    /*printf("A_1");
    for(int i=0;i<D;i++){
        for(int j=0;j<(K-D);j++) {
            printpolynomial(A_1_marked.pol[i][j]);
        }printf("\n");
    }
    printf("A_2");
    for(int i=0;i<L;i++){
        for(int j=0;j<(K-D-L);j++) {
            printpolynomial(A_2_marked.pol[i][j]);
        }printf("\n");
    }*/
    pmatrixTimesVectorNTTA_1(A_1_marked,randomness,commit);
    pmatrixTimesVectorNTTA_2(A_2_marked,randomness,commit);
    //printf("commitment after matrix multiplication\n");
    /*for(int i=0;i<D+L;i++){
        printpolynomial(commit->pol[i]);
    }*/
    inverseNTT_commitmentvectorDL(commit);
    /*for(int i=0;i<D+L;i++){
        printpolynomial(commit->pol[i]);
    }*/
    for(int i=0; i<D; i++){
        commit->pol[i+L]=addPolynomials(commit->pol[i],message->pol[i],get_N());
    }
}


