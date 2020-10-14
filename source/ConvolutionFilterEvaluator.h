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


class ConvolutionFilterEvaluator {
public:
    static void main();

private:
    static void display_matrix(int **a, int m, int n);
    static void accept_inputs(int **input_data, int **filter);
};


#endif //HOMOMORPHIC_DEMONSTRATIONS_CONVOLUTIONFILTEREVALUATOR_H
