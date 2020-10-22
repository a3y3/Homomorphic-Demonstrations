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
void MatrixMultiplier::main(const COED::Encryptor &encryptor) {
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
            ptxt_a[i][j] = a[i][j];
            ptxt_b[i][j] = b[i][j];
        }
    }

    for (int i = 0; i < NCOLS; ++i) {
        encryptor.getPublicKey()->Encrypt(ctxt_a[i], ptxt_a[i]);
        encryptor.getPublicKey()->Encrypt(ctxt_b[i], ptxt_b[i]);
    }

    int result[NCOLS][NROWS];

    helib::Ptxt<helib::BGV> ptxt_result[NCOLS];
    initialize_plaintexts(ptxt_result, NCOLS, encryptor);
    helib::Ctxt ctxt_result[3]{helib::Ctxt(*(encryptor.getPublicKey())), helib::Ctxt(*(encryptor.getPublicKey())),
                               helib::Ctxt(*(encryptor.getPublicKey()))};

    std::cout << "\nCalculating result...\n";
    matrix_multiplication(ctxt_a, ctxt_b, ctxt_result, encryptor);

    for (int i = 0; i < NCOLS; ++i) {
        std::vector<long> plaintext(encryptor.getEncryptedArray()->size());
        encryptor.getEncryptedArray()->decrypt(ctxt_result[i], *encryptor.getSecretKey(), plaintext);
        for (int j = 0; j < NROWS; ++j) {
            result[i][j] = plaintext[j];
        }
    }

    COED::Util::info("Result:");
    display_matrix(result);
}

/**
 * Calculates matrix multiplication as a dot product of n vectors.
 * @param ctxt_a First array of vector.
 * @param ctxt_b Second array of vector. Note that this is the transposed second matrix.
 * @param ctxt_result The result is stored in this ciphertext array (each index contains one row of the result)
 * @param encryptor needed to pass to @code{DotProduct::dot_product()}.
 */
void MatrixMultiplier::matrix_multiplication(helib::Ctxt ctxt_a[], helib::Ctxt ctxt_b[], helib::Ctxt ctxt_result[],
                                             const COED::Encryptor &encryptor) {
    for (int i = 0; i < NCOLS; ++i) {
        for (int j = 0; j < NROWS; ++j) {
            helib::Ctxt ctxt_a_copy(ctxt_a[i]);
            ctxt_a_copy = DotProduct::dot_product(ctxt_a_copy, ctxt_b[j], encryptor);
            encryptor.getEncryptedArray()->rotate(ctxt_a_copy, j);
            ctxt_result[i].addCtxt(ctxt_a_copy);
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

