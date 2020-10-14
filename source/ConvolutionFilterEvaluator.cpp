//
// Created by soham on 10/13/20.
//

#include "ConvolutionFilterEvaluator.h"
#include <iostream>
#include "Encryptor.h"
#include "Util.h"

#define INPUT_DATA_COLUMNS 6
#define INPUT_DATA_ROWS 6
#define FILTER_ROWS 3
#define FILTER_COLUMNS 3
#define FEATURE_MAP_COLUMNS 4
#define FEATURE_MAP_ROWS 4

void ConvolutionFilterEvaluator::main() {

}

void accept_inputs(int input_data[][INPUT_DATA_ROWS], int filter[][FILTER_ROWS]) {
    std::cout << "\nInput data is a 6x6 matrix.";
    for (int i = 0; i < INPUT_DATA_COLUMNS; ++i) {
        std::cout << "\nInput " << INPUT_DATA_COLUMNS << " values for input data's row number" << i
                  << " (Hit <Return> after each)";
        for (int j = 0; j < INPUT_DATA_COLUMNS; ++j) {
            std::cin >> input_data[i][j];
        }
    }

    std::cout << "\n The filter is a 3x3 matrix.";
    for (int i = 0; i < FILTER_COLUMNS; ++i) {
        std::cout << "\nInput " << FILTER_COLUMNS << " values for input data's row number" << i
                  << " (Hit <Return> after each)";
        for (int j = 0; j < FILTER_ROWS; ++j) {
            std::cin >> filter[i][j];
        }
    }
}

void ConvolutionFilterEvaluator::display_matrix(int a[6][6], int m, int n) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << a[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
