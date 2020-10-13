//
// Created by soham on 10/13/20.
//

#ifndef HOMOMORPHIC_DEMONSTRATIONS_MATRIXMULTIPLIER_H
#define HOMOMORPHIC_DEMONSTRATIONS_MATRIXMULTIPLIER_H
#define NROWS 3
#define NCOLS 3

class MatrixMultiplier {
public:
    static void main();

private:
    static void accept_inputs(int a[][NCOLS], int b[][NROWS]);
    static void transpose_matrix(int a[][NCOLS]);
    static void accept_matrix(int a[][NCOLS]);

};


#endif //HOMOMORPHIC_DEMONSTRATIONS_MATRIXMULTIPLIER_H
