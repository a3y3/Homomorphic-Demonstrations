//
// Created by soham on 10/13/20.
//

#ifndef HOMOMORPHIC_DEMONSTRATIONS_CONVOLUTIONFILTEREVALUATOR_H
#define HOMOMORPHIC_DEMONSTRATIONS_CONVOLUTIONFILTEREVALUATOR_H
#define INPUT_DATA_COLUMNS 6
#define INPUT_DATA_ROWS 6
#define FILTER_ROWS 3
#define FILTER_COLUMNS 3
#define FEATURE_MAP_COLUMNS 4
#define FEATURE_MAP_ROWS 4

#include "Encryptor.h"

class ConvolutionFilterEvaluator {
public:
    static void main();

    static void
    evaluate_convolutional_filter_seq(helib::Ctxt *input_data, helib::Ctxt &filter, const COED::Encryptor &,
                                      int **result);

    static void
    evaluate_convolutional_filter_parallel(helib::Ctxt *input_data, helib::Ctxt &filter, const COED::Encryptor &,
                                           int **result);

private:
    static void display_matrix(int **a, int m, int n);

    static void accept_inputs(int **input_data, int **filter);

    static void print_vector(const std::string &msg, const std::vector<long> &vec, bool space);
};


#endif //HOMOMORPHIC_DEMONSTRATIONS_CONVOLUTIONFILTEREVALUATOR_H
