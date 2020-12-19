//
// Created by Anna Bakkebø on 05/12/2020.
//

#ifndef NTT_MATRIXMULTIPLICATION_H
#define NTT_MATRIXMULTIPLICATION_H

#include "params.h"
/**
 * Adding all coefficients of two polynomials
 * @param pol1 The first polynomial that is to be added
 * @param pol2 The second polynomial that is to be added
 * @param size The degree of the two polynomials
 * @return The sum of the two polynomials
 */
struct pol addPolynomials(struct pol pol1, struct pol pol2, long size);

/**
 * Computes A_1 times randomness,
 * <p style="text-indent:40px;">A_1 = [ I<sub>D</sub> &emsp; A_1_marked],</p>
 * and randomness is a vector of length K, using normal multiplication
 * @param A_1_marked D times K-D vector used as the last part of the A_1 vector
 * @param randomness vector of length K
 * @param commit commitment vector of length D+L
 */
void matrixTimesVectorNormalA_1(struct A_1_marked A_1_marked, struct randomness_vector_K randomness,
                                struct comitment_vector_DL *commit);

/**
 * Computes A_2 times randomness, where
 * <p style="text-indent:40px;">A_2 = [0<sup>(LxD)</sup> &emsp; I<sub>L</sub> &emsp; A_2_marked],</p>
 * and randomness is a vector of length K, using normal multiplication
 * @param A_2_marked L times (K-D-L) vector used as the last part of the A_1 vector
 * @param randomness vector of length K
 * @param commit commitment vector of length D+L
 */
void matrixTimesVectorNormalA_2(struct A_2_marked A_2_marked, struct randomness_vector_K randomness,
                                struct comitment_vector_DL *commit);

/**
 * Commits the message m by computing
 *
 * <table style="margin-left:40px;">
   <tbody>
      <tr>
      <td> commit = </td>
      <td>[A_1] <br> [A_2]</td>
      <td> * r + </td>
      <td> [0<sup>D</sup>]<br> [m] </td>
      </tr>
   </tbody>
   </table>
 * Using normal multiplication
 * @param A_1_marked D times K-D vector used as the last part of the A_1 vector
 * @param A_2_marked L times (K-D-L) vector used as the last part of the A_1 vector
 * @param randomness randomnessvector r of length K
 * @param message message m of length L
 * @param commit commitment vector of length D+L
 */
void commitNormal(struct A_1_marked A_1_marked, struct A_2_marked A_2_marked, struct randomness_vector_K randomness,
                  struct message_vector_L message, struct comitment_vector_DL *commit);

/**
 * Commits the message m by computing
 *
 * <table style="margin-left:40px;">
   <tbody>
      <tr>
      <td> commit = </td>
      <td>[A_1] <br> [A_2]</td>
      <td> * r + </td>
      <td> [0<sup>D</sup>]<br> [m] </td>
      </tr>
   </tbody>
   </table>
 * Using normal multiplication
 * @param A_1_marked pointer to a D times K-D vector used as the last part of the A_1 vector
 * @param A_2_marked pointer to a L times (K-D-L) vector used as the last part of the A_1 vector
 * @param randomness pointer to the randomnessvector r of length K
 * @param message pointer to the message m of length L
 * @param commit pointer to the commitment vector of length D+L
 */
void pcommitNormal(struct A_1_marked *A_1_marked, struct A_2_marked *A_2_marked, struct randomness_vector_K *randomness,
                   struct message_vector_L *message, struct comitment_vector_DL *commit);


/**
 * Computes NTT forward of all the polynomials in the matrices and vector
 * @param A_1_marked D times (K-D) matrix
 * @param A_2_marked L times (K-D-L) matrix
 * @param randomness vector of length K
 */
void forwardNTT_matrices_vector(struct A_1_marked *A_1_marked, struct A_2_marked *A_2_marked,
                                struct randomness_vector_K *randomness);

/**
 * Computes A_1 times randomness, where
 * <p style="text-indent:40px;">A_1 = [ I<sub>D</sub> &emsp; A_1_marked],</p>
 * and randomness is a vector of length K, using NTT multiplication
 * @param A_1_marked D times K-D vector used as the last part of the A_1 vector in NTT version
 * @param randomness vector of length K in NTT version
 * @param commit commitment vector of length D+L
 */
void matrixTimesVectorNTTA_1(struct A_1_marked A_1_marked, struct randomness_vector_K randomness,
                             struct comitment_vector_DL *commit);

/**
 * Computes A_2 times randomness, where
 * <p style="text-indent:40px;">A_2 = [0<sup>(LxD)</sup> &emsp; I<sub>L</sub> &emsp; A_2_marked],</p>
 * and randomness is a vector of length K, using NTT multiplication
 * @param A_2_marked L times (K-D-L) vector used as the last part of the A_1 vector
 * @param randomness vector of length K in NTT version
 * @param commit commitment vector of length D+L
 */
void matrixTimesVectorNTTA_2(struct A_2_marked A_2_marked, struct randomness_vector_K randomness,
                             struct comitment_vector_DL *commit);

/**
 * Inverse NTT of a vector with length D+L
 * @param vector Vector of length D+L in NTT version
 */
void inverseNTT_commitmentvectorDL(struct comitment_vector_DL *vector);

/**
 * Commits the message m by computing
 *
 * <table style="margin-left:40px;">
   <tbody>
      <tr>
      <td>[A_1] <br> [A_2]</td>
      <td> * r + </td>
      <td> [0<sup>D</sup>]<br> [m] </td>
      </tr>
   </tbody>
   </table>
 * Using NTT multiplication
 * @param A_1_marked D times K-D vector used as the last part of the A_1 vector
 * @param A_2_marked L times (K-D-L) vector used as the last part of the A_1 vector
 * @param randomness randomnessvector r of length K
 * @param message message m of length L
 * @param commit commitment vector of length D+L
 */
void commitNTT(struct A_1_marked A_1_marked, struct A_2_marked A_2_marked, struct randomness_vector_K randomness,
               struct message_vector_L message, struct comitment_vector_DL *commit);

void pcommitNTT(struct A_1_marked *A_1_marked, struct A_2_marked *A_2_marked, struct randomness_vector_K *randomness,
                struct message_vector_L *message, struct comitment_vector_DL *commit);


struct pol multiplyRowByVectorNTT(struct pol *row, struct pol *vector, int size);
struct pol multiplyRowByVectorNormal(struct pol *row, struct pol *vector, int size);

#endif //NTT_MATRIXMULTIPLICATION_H
