//
// Copyright SpiRITlab - Computations on Encrypted Data
// https://gitlab.com/SpiRITlab/coed
//
// Modified by soham on 10/10/20.

#include <iostream>
#include "BasicExamples.h"
#include "Calculator.h"
#include "DotProduct.h"

int main() {
    std::cout << "Demonstration 1: Homomorphic Calculator" << std::endl;
//    Calculator::run_calculator();
    DotProduct::run_dot_product();

    //BasicExamples::determine_plaintext_slot();

//    BasicExamples::decimal_arithmetic_example();

    //BasicExamples::packed_binary_arithmetic_example();

    //BasicExamples::packed_decimal_arithmetic_example();

//    BasicExamples::packed_decimal_arithmetic_scalar_product_example();

    return 0;
}
