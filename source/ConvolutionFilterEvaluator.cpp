//
// Created by soham on 10/13/20.
//

#include "ConvolutionFilterEvaluator.h"
#include <iostream>
#include "Encryptor.h"
#include "Util.h"
#include "DotProduct.h"

void ConvolutionFilterEvaluator::evaluate_convolutional_filter(helib::Ctxt *input_data, helib::Ctxt &filter,
                                                               const COED::Encryptor &encryptor, int **result) {
    int rotate_counter = 0;
    for (int i = 0; i < FEATURE_MAP_COLUMNS; ++i) {
        for (int j = 0; j < FEATURE_MAP_ROWS; ++j) {
            helib::Ctxt copy(*input_data);
            std::cout << std::endl;
//            COED::Util::debug("Finished copy");
            DotProduct::dot_product(&copy, filter, encryptor);
//            COED::Util::debug("Finished dot product");
            std::vector<long> plaintext(encryptor.getEncryptedArray()->size());
            encryptor.getEncryptedArray()->decrypt(copy, *encryptor.getSecretKey(), plaintext);
            result[i][j] = plaintext[1];
            std::cout << "Found result[" << i << "][" << j << "]=" << result[i][j];
            rotate_counter++;
            if (rotate_counter < 4) {
                encryptor.getEncryptedArray()->rotate(filter, 1);
            } else {
                encryptor.getEncryptedArray()->rotate(filter, 3);
                rotate_counter = 0;
            }
        }
    }
}

void ConvolutionFilterEvaluator::main() {
    int plaintext_prime_modulus = 53;
    int phiM = 7363;
    int lifting = 1;
    int numOfBitsOfModulusChain = 256;
    int numOfColOfKeySwitchingMatrix = 2;

    COED::Util::info("Starting program ...");

    COED::Encryptor encryptor("/tmp/sk.txt", "/tmp/pk.txt",
                              plaintext_prime_modulus,
                              phiM,
                              lifting,
                              numOfBitsOfModulusChain,
                              numOfColOfKeySwitchingMatrix);
    COED::Util::info("Finished creating encryptor.");

    int **input_data;
    input_data = new int *[INPUT_DATA_COLUMNS];
    for (int i = 0; i < INPUT_DATA_COLUMNS; ++i) {
        input_data[i] = new int[INPUT_DATA_ROWS];
    }
    int **filter;
    filter = new int *[FILTER_COLUMNS];
    for (int i = 0; i < FILTER_COLUMNS; ++i) {
        filter[i] = new int[FILTER_ROWS];
    }
    ConvolutionFilterEvaluator::accept_inputs(input_data, filter);
    COED::Util::info("Input data:");
    display_matrix(input_data, INPUT_DATA_COLUMNS, INPUT_DATA_ROWS);
    COED::Util::info("Filter:");
    display_matrix(filter, FILTER_COLUMNS, FILTER_ROWS);

    helib::Ptxt<helib::BGV> ptxt_input_data(*(encryptor.getContext()));
    helib::Ptxt<helib::BGV> ptxt_filter(*(encryptor.getContext()));

    helib::Ctxt ctxt_input_data(*(encryptor.getPublicKey()));
    helib::Ctxt ctxt_filter(*(encryptor.getPublicKey()));

    for (int i = 0; i < INPUT_DATA_COLUMNS; ++i) {
        for (int j = 0; j < INPUT_DATA_COLUMNS; ++j) {
            ptxt_input_data[(i * INPUT_DATA_COLUMNS) + j] = input_data[i][j];
        }
    }

    int fillerIndex = 0;
    for (int i = 0; i < INPUT_DATA_COLUMNS; ++i) {
        for (int j = 0; j < INPUT_DATA_ROWS; ++j) {
            if (i < FILTER_COLUMNS && j < FILTER_ROWS) {
                ptxt_filter[fillerIndex++] = filter[i][j];
            } else { fillerIndex++; }
        }
    }

    encryptor.getPublicKey()->Encrypt(ctxt_input_data, ptxt_input_data);
    encryptor.getPublicKey()->Encrypt(ctxt_filter, ptxt_filter);

    int **feature_map;
    feature_map = new int *[FEATURE_MAP_COLUMNS];
    for (int i = 0; i < FEATURE_MAP_COLUMNS; ++i) {
        feature_map[i] = new int[FEATURE_MAP_ROWS];
    }

    evaluate_convolutional_filter(&ctxt_input_data, ctxt_filter, encryptor, feature_map);

    std::cout << "\nResult [Feature Map]:\n";
    display_matrix(feature_map, FEATURE_MAP_COLUMNS, FEATURE_MAP_ROWS);
}


void ConvolutionFilterEvaluator::accept_inputs(int **input_data, int **filter) {
    std::cout << "\nType 'y' for predefined inputs, else type 'n' for inputting custom values\n";
    char choice = 'n';
    std::cin >> choice;
    int sample_data[] = {0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0,
                         1, 0, 0, 1};
    int sample_filter[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

    if (choice == 'y') {
        for (int i = 0; i < INPUT_DATA_COLUMNS; ++i) {
            for (int j = 0; j < INPUT_DATA_COLUMNS; ++j) {
                input_data[i][j] = sample_data[i * INPUT_DATA_COLUMNS + j];
            }
        }

        for (int i = 0; i < FILTER_COLUMNS; ++i) {
            for (int j = 0; j < FILTER_ROWS; ++j) {
                filter[i][j] = sample_filter[i * FILTER_COLUMNS + j];
            }
        }
    } else {
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

}

void ConvolutionFilterEvaluator::display_matrix(int **a, int m, int n) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << a[i][j] << " ";
        }
        std::cout << std::endl;
    }
}



