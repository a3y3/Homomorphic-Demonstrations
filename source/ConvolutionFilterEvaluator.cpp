//
// Created by soham on 10/13/20.
//

#include "ConvolutionFilterEvaluator.h"
#include <iostream>
#include <omp.h>
#include "Encryptor.h"
#include "Util.h"
#include "DotProduct.h"
#include <chrono>

/**
 * Evaluates the convolution filter.
 * Given a 6x6 input_data and a 3x3 filter, the evaluation is defined as a 4x4 filter matrix where each value in the
 * filter matrix is a dot product of the input_data with the filter. In total, there will be 16 dot products.
 * The inputs are expected to be 1d ciphertexts of the matrices. This function will calculate the dot product, rotate
 * the ciphertext, calculate the next dot product, and so on.
 * @param input_data the input matrix.
 * @param filter the filter to be applied on the input matrix.
 * @param encryptor needed for rotating the filter ciphertext.
 * @param result_ctxt final values are put inside this ciphertext.
 */
void ConvolutionFilterEvaluator::evaluate_convolutional_filter_seq(helib::Ctxt *input_data, helib::Ctxt &filter,
                                                                   const COED::Encryptor &encryptor,
                                                                   helib::Ctxt *result_ctxt) {
    for (int i = 0; i < FEATURE_MAP_COLUMNS; ++i) {
        for (int j = 0; j < FEATURE_MAP_ROWS; ++j) {
            helib::Ctxt copy(*input_data);
            copy = DotProduct::dot_product(copy, filter, encryptor);

            encryptor.getEncryptedArray()->rotate(copy, i * FEATURE_MAP_COLUMNS + j);
            result_ctxt->addCtxt(copy);

            encryptor.getEncryptedArray()->rotate(filter, 1);
        }
        encryptor.getEncryptedArray()->rotate(filter, 2);
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"

/**
 * The parallel version of @code{evaluate_convolutional_filter_seq}.
 * This function will only work properly if there are exactly 4 threads, no more, no less.
 * The idea is to use one thread for each row of the output 4x4 resultant matrix.
 * @param input_data the input matrix.
 * @param filter the filter to be applied on the input matrix.
 * @param encryptor needed for rotating the filter ciphertext.
 * @param result_ctxt final values are put inside this ciphertext.
 */
void ConvolutionFilterEvaluator::evaluate_convolutional_filter_parallel(helib::Ctxt *input_data, helib::Ctxt &filter,
                                                                        const COED::Encryptor &encryptor,
                                                                        helib::Ctxt *result_ctxt) {
    helib::Ctxt filter_t0(filter);
    helib::Ctxt filter_t1(filter);
    helib::Ctxt filter_t2(filter);
    helib::Ctxt filter_t3(filter);

#pragma omp parallel
    {
        omp_set_num_threads(4);
        int threadID = omp_get_thread_num();
        if (threadID == 1) {
            encryptor.getEncryptedArray()->rotate(filter_t1, 6);
        } else if (threadID == 2) {
            encryptor.getEncryptedArray()->rotate(filter_t2, 12);
        } else if (threadID == 3) {
            encryptor.getEncryptedArray()->rotate(filter_t3, 18);
        }
#pragma omp for
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                helib::Ctxt copy(*input_data);
                if (threadID == 0) {
                    copy = DotProduct::dot_product(copy, filter_t0, encryptor);
                    encryptor.getEncryptedArray()->rotate(filter_t0, 1);
                } else if (threadID == 1) {
                    copy = DotProduct::dot_product(copy, filter_t1, encryptor);
                    encryptor.getEncryptedArray()->rotate(filter_t1, 1);
                } else if (threadID == 2) {
                    copy = DotProduct::dot_product(copy, filter_t2, encryptor);
                    encryptor.getEncryptedArray()->rotate(filter_t2, 1);
                } else if (threadID == 3) {
                    copy = DotProduct::dot_product(copy, filter_t3, encryptor);
                    encryptor.getEncryptedArray()->rotate(filter_t3, 1);
                }

                encryptor.getEncryptedArray()->rotate(copy, i * FEATURE_MAP_COLUMNS + j);
                result_ctxt->addCtxt(copy);
//                std::vector<long> plaintext(encryptor.getEncryptedArray()->size());
//                encryptor.getEncryptedArray()->decrypt(copy, *encryptor.getSecretKey(), plaintext);
//                result[i][j] = plaintext[1];
            }
        }
    }
}

#pragma clang diagnostic pop

/**
 * Runner of the core demonstration functionality.
 * This function accepts data, converts the 2d matrices into 1d plain texts, enrypts them, then calls
 * @code{evaluate_convolutional_filter()} to calculate the result.
 */
void ConvolutionFilterEvaluator::main(const COED::Encryptor &encryptor) {

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
    helib::Ptxt<helib::BGV> ptxt_result(*(encryptor.getContext()));

    helib::Ctxt ctxt_input_data(*(encryptor.getPublicKey()));
    helib::Ctxt ctxt_filter(*(encryptor.getPublicKey()));
    helib::Ctxt ctxt_result_seq(*(encryptor.getPublicKey()));
    helib::Ctxt ctxt_result_parallel(*(encryptor.getPublicKey()));

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

    helib::Ctxt seq_filter_copy(ctxt_filter);

    auto seq_start_time = std::chrono::high_resolution_clock::now();
    evaluate_convolutional_filter_seq(&ctxt_input_data, seq_filter_copy, encryptor, &ctxt_result_seq);
    auto seq_stop_time = std::chrono::high_resolution_clock::now();

    std::vector<long> plaintext_feature_map(encryptor.getEncryptedArray()->size());
    encryptor.getEncryptedArray()->decrypt(ctxt_result_seq, *encryptor.getSecretKey(), plaintext_feature_map);
    for (int i = 0; i < FEATURE_MAP_COLUMNS; ++i) {
        for (int j = 0; j < FEATURE_MAP_ROWS; ++j) {
            feature_map[i][j] = plaintext_feature_map[i * FEATURE_MAP_COLUMNS + j];
        }
    }

    auto seq_duration = std::chrono::duration_cast<std::chrono::milliseconds>(seq_stop_time - seq_start_time).count();
    std::cout << "\nResult [Feature Map]:\n";
    display_matrix(feature_map, FEATURE_MAP_COLUMNS, FEATURE_MAP_ROWS);
    std::cout << "\nTime taken for sequential execution: " << seq_duration << "ms";

    for (int i = 0; i < FEATURE_MAP_COLUMNS; ++i) {
        for (int j = 0; j < FEATURE_MAP_ROWS; ++j) {
            feature_map[i][j] = 0;
        }
    }


    auto parallel_start_time = std::chrono::high_resolution_clock::now();
    evaluate_convolutional_filter_parallel(&ctxt_input_data, ctxt_filter, encryptor, &ctxt_result_parallel);
    auto parallel_stop_time = std::chrono::high_resolution_clock::now();
    auto parallel_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            parallel_stop_time - parallel_start_time).count();

    std::vector<long> plaintext_feature_map_parallel(encryptor.getEncryptedArray()->size());
    encryptor.getEncryptedArray()->decrypt(ctxt_result_parallel, *encryptor.getSecretKey(),
                                           plaintext_feature_map_parallel);
    for (int i = 0; i < FEATURE_MAP_COLUMNS; ++i) {
        for (int j = 0; j < FEATURE_MAP_ROWS; ++j) {
            feature_map[i][j] = plaintext_feature_map_parallel[i * FEATURE_MAP_COLUMNS + j];
        }
    }
    std::cout << "\nResult [Feature Map]:\n";
    display_matrix(feature_map, FEATURE_MAP_COLUMNS, FEATURE_MAP_ROWS);
    std::cout << "\nTime taken for parallel execution: " << parallel_duration << "ms.";
}

/**
 * Accepts input from the user for input_data and the filter.
 * If the user chooses, this function also has predefined values, which serve for a good demonstration.
 * @param input_data 2D array for the input matrix.
 * @param filter 2D filter array.
 */
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

/**
 * Pretty print a given matrix, given the number of rows and columns.
 * @param a 2D matrix.
 * @param m number of columns.
 * @param n number of rows.
 */
void ConvolutionFilterEvaluator::display_matrix(int **a, int m, int n) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << a[i][j] << " ";
        }
        std::cout << std::endl;
    }
}



