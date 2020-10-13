//
// Created by soham on 10/13/20.
//

#include <iostream>
#include "MatrixMultiplier.h"
#include "Encryptor.h"
#include "Util.h"
#include "DotProduct.h"

/**
 * Runs the main functionality, including input, etc.
 * Matrix multiplication can be seen as a dot product of several vectors. This function takes matrix inputs, transposes
 * the second matrix (for ease with dot product, think about how the elements of a matrix are multiplied), stores them
 * in arrays of ciphertext object, then calls @code{DotProduct::dot_product{}} @code{NCOLS} * @code{NROWS} times.
 */
void MatrixMultiplier::main() {
    int plaintext_prime_modulus = 53;
    int phiM = 3000;
    int lifting = 1;
    int numOfBitsOfModulusChain = 512;
    int numOfColOfKeySwitchingMatrix = 2;

    COED::Util::info("Starting program ...");

    COED::Encryptor encryptor("/tmp/sk.txt", "/tmp/pk.txt",
                              plaintext_prime_modulus,
                              phiM,
                              lifting,
                              numOfBitsOfModulusChain,
                              numOfColOfKeySwitchingMatrix);

    int a[NCOLS][NROWS];
    int b[NCOLS][NROWS];
    MatrixMultiplier::accept_inputs(a, b);
    COED::Util::info("Matrix A:");
    display_matrix(a);
    COED::Util::info("Matrix B:");
    display_matrix(b);

    /* Transposing the second matrix allows us to effectively represent matrix multiplication as M*N dot products.
     * This makes the problem easy since we already have a dot product finder.
     */
    MatrixMultiplier::transpose_matrix(b);

    // Create plain text objects. Later, the array elements are added to these.
    helib::Ptxt<helib::BGV> ptxt_a[NCOLS];
    helib::Ptxt<helib::BGV> ptxt_b[NCOLS];
    initialize_plaintexts(ptxt_a, NCOLS, encryptor);
    initialize_plaintexts(ptxt_b, NCOLS, encryptor);

    // Create ciphertext objects. The plaintext objects are encrypted by the public key in the ciphertexts.
    helib::Ctxt ctxt_a[3]{helib::Ctxt(*(encryptor.getPublicKey())), helib::Ctxt(*(encryptor.getPublicKey())),
                          helib::Ctxt(*(encryptor.getPublicKey()))};
    helib::Ctxt ctxt_b[3]{helib::Ctxt(*(encryptor.getPublicKey())), helib::Ctxt(*(encryptor.getPublicKey())),
                          helib::Ctxt(*(encryptor.getPublicKey()))};

    for (int i = 0; i < NCOLS; ++i) {
        for (int j = 0; j < NROWS; ++j) {
            ptxt_a[i][j + 1] = a[i][j];
            ptxt_b[i][j + 1] = b[i][j];
        }
    }

    for (int i = 0; i < NCOLS; ++i) {
        encryptor.getPublicKey()->Encrypt(ctxt_a[i], ptxt_a[i]);
        encryptor.getPublicKey()->Encrypt(ctxt_b[i], ptxt_b[i]);
    }

    int result[NCOLS][NROWS];

    std::cout << "\nCalculating result...\n";
    matrix_multiplication(ctxt_a, ctxt_b, result, encryptor);

    COED::Util::info("Result:");
    display_matrix(result);
}

/**
 * Calculates matrix multiplication as a dot product of n vectors.
 * @param ctxt_a First array of vector.
 * @param ctxt_b Second array of vector. Note that this is the transposed second matrix.
 * @param result The m*n result matrix.
 * @param encryptor needed to pass to @code{DotProduct::dot_product()}.
 */
void MatrixMultiplier::matrix_multiplication(helib::Ctxt ctxt_a[], helib::Ctxt ctxt_b[], int result[][NROWS],
                                             const COED::Encryptor &encryptor) {
    for (int i = 0; i < NCOLS; ++i) {
        for (int j = 0; j < NROWS; ++j) {
            /*
             * Now, result[i][j]=dot_product(ctxt_a[i],ctxt_b[j]). However, we need to create a copy of ctxt_a first
             * as homomorphic dot product stores the result in the first vector (ctxt_a) and we are going to need
             * the vector again and again.
             */
            helib::Ctxt ctxt_a_copy(ctxt_a[i]);
            DotProduct::dot_product(&ctxt_a_copy, ctxt_b[j], encryptor);

            //Now, all elements of ctxt_a_copy are basically result[i][j]. Select any one of them (by decrypting it).
            std::vector<long> plaintext(encryptor.getEncryptedArray()->size());
            encryptor.getEncryptedArray()->decrypt(ctxt_a_copy, *encryptor.getSecretKey(), plaintext);
            result[i][j] = plaintext[1];
        }
    }
}

/**
 * Helper function. Initializes the plaintext objects in a loop.
 * @param ptxt Array of plaintext objects.
 * @param count The number of initializations to be perfomed.
 * @param encryptor needed to get the context.
 */
void
MatrixMultiplier::initialize_plaintexts(helib::Ptxt<helib::BGV> ptxt[], int count, const COED::Encryptor &encryptor) {
    for (int i = 0; i < count; ++i) {
        ptxt[i] = helib::Ptxt<helib::BGV>(*(encryptor.getContext()));
    }
}

/**
 * Inputs values for the given 2D matrices.
 * @param a First matrix
 * @param b Second matrix in which values are to be accepted from the user.
 */
void MatrixMultiplier::accept_inputs(int a[][NROWS], int b[][NROWS]) {
    std::cout << "\nMatrix 1:";
    int columns = NCOLS;
    int rows = NROWS;
    int total_values = columns * rows;
    std::cout << "\nEnter " << total_values << " values (Hit <Return> after each value)" << std::endl;
    MatrixMultiplier::accept_matrix(a);

    std::cout << "\nMatrix 2:";
    std::cout << "\nEnter " << total_values << " values (Hit <Return> after each value)" << std::endl;
    MatrixMultiplier::accept_matrix(b);
}

/**
 * Helper function for @code{accept_inputs()}.
 * @param a The matrix in which values are to be accepted.
 */
void MatrixMultiplier::accept_matrix(int a[][NCOLS]) {
    for (int i = 0; i < NCOLS; ++i) {
        for (int j = 0; j < NROWS; ++j) {
            std::cin >> a[i][j];
        }
    }
}

/**
 * Transposes an input matrix.
 * @param a Input matrix.
 */
void MatrixMultiplier::transpose_matrix(int a[][NCOLS]) {
    for (int i = 0; i < NCOLS; ++i) {
        for (int j = 0; j <= i; ++j) {
            int temp = a[i][j];
            a[i][j] = a[j][i];
            a[j][i] = temp;
        }
    }
}

/**
 * Displays the matrix in column row form.
 * @param a 2D Matrix
 */
void MatrixMultiplier::display_matrix(int a[][NCOLS]) {
    for (int i = 0; i < NCOLS; ++i) {
        for (int j = 0; j < NROWS; ++j) {
            std::cout << a[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

