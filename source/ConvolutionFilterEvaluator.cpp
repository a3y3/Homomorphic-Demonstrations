//
// Created by soham on 10/13/20.
//

#include "ConvolutionFilterEvaluator.h"
#include <iostream>
#include "Encryptor.h"
#include "Util.h"


void ConvolutionFilterEvaluator::main() {
    int **input_data;
    input_data = new int *[INPUT_DATA_COLUMNS];
    for (int i = 0; i < INPUT_DATA_COLUMNS; ++i) {
        input_data[i] = new int[INPUT_DATA_ROWS];
    }
    int **filter;
    filter=new int *[FILTER_COLUMNS];
    for (int i = 0; i < FILTER_COLUMNS; ++i) {
        filter[i]=new int[FILTER_ROWS];
    }
    ConvolutionFilterEvaluator::accept_inputs(input_data, filter);
    COED::Util::info("Input data:");
    display_matrix(input_data, INPUT_DATA_COLUMNS, INPUT_DATA_ROWS);
    COED::Util::info("Filter:");
    display_matrix(filter, FILTER_COLUMNS, FILTER_ROWS);

}

void ConvolutionFilterEvaluator::accept_inputs(int **input_data, int **filter) {
    std::cout << "\nInput data is a 6x6 matrix.";
    for (int i = 0; i < INPUT_DATA_COLUMNS; ++i) {
        std::cout << "\nInput " << INPUT_DATA_COLUMNS << " values for input data's row number " << i
                  << " (Hit <Return> after each)";
        for (int j = 0; j < INPUT_DATA_COLUMNS; ++j) {
            std::cin >> input_data[i][j];
        }
    }

    std::cout << "\nThe filter is a 3x3 matrix.";
    for (int i = 0; i < FILTER_COLUMNS; ++i) {
        std::cout << "\nInput " << FILTER_COLUMNS << " values for input data's row number " << i
                  << " (Hit <Return> after each)";
        for (int j = 0; j < FILTER_ROWS; ++j) {
            std::cin >> filter[i][j];
        }
    }
}

void ConvolutionFilterEvaluator::display_matrix(int **a, int m, int n) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << a[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
