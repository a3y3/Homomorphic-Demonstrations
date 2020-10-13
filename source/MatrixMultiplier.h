//
// Created by soham on 10/13/20.
//

#ifndef HOMOMORPHIC_DEMONSTRATIONS_MATRIXMULTIPLIER_H
#define HOMOMORPHIC_DEMONSTRATIONS_MATRIXMULTIPLIER_H
#define NROWS 3
#define NCOLS 3

#include "Encryptor.h"

class MatrixMultiplier {
public:
    static void main();

private:
    static void accept_inputs(int a[][NCOLS], int b[][NROWS]);

    static void transpose_matrix(int a[][NCOLS]);

    static void accept_matrix(int a[][NCOLS]);

    static void display_matrix(int a[][NCOLS]);

    static void initialize_plaintexts(helib::Ptxt<helib::BGV> ptxt[], int count, const COED::Encryptor &encryptor);

    static void initialize_ciphertexts(helib::Ctxt ctxt[], int count, const COED::Encryptor &encryptor);
};


#endif //HOMOMORPHIC_DEMONSTRATIONS_MATRIXMULTIPLIER_H
