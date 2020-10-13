//
// Created by soham on 10/13/20.
//

#include <iostream>
#include "MatrixMultiplier.h"

void MatrixMultiplier::main() {

}

void MatrixMultiplier::accept_inputs(int a[][NROWS], int b[][NROWS]) {
    std::cout << "\nMatrix 1:";
    int columns = NCOLS;
    int rows = NROWS;
    int total_values = columns * rows;
    std::cout << "\nEnter <<" << total_values << " values (Hit <Return> after each value)";
    MatrixMultiplier::accept_matrix(a);

    std::cout << "\nMatrix 1:";
    std::cout << "\nEnter <<" << total_values << " values (Hit <Return> after each value)";
    MatrixMultiplier::accept_matrix(b);
}

void MatrixMultiplier::accept_matrix(int a[][NCOLS]) {
    for (int i = 0; i < NCOLS; ++i) {
        for (int j = 0; j < NROWS; ++j) {
            std::cin >> a[i][j];
        }
    }
}

void MatrixMultiplier::transpose_matrix(int a[][NCOLS]) {
    for (int i = 0; i < NCOLS; ++i) {
        for (int j = 0; j < NROWS; ++j) {
            a[i][j] = a[j][i];
        }
    }
}
