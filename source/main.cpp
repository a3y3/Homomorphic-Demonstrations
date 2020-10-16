//
// Copyright SpiRITlab - Computations on Encrypted Data
// https://gitlab.com/SpiRITlab/coed
//
// Modified by soham on 10/10/20.

#include <iostream>
#include "Calculator.h"
#include "DotProduct.h"
#include "MatrixMultiplier.h"
#include "ConvolutionFilterEvaluator.h"
#include "Encryptor.h"
#include "Util.h"

int main() {
    int plaintext_prime_modulus = 53;
    int phiM = 2665;
    int lifting = 1;
    int numOfBitsOfModulusChain = 512;
    int numOfColOfKeySwitchingMatrix = 2;
    COED::Util::info("Creating encryptor ...");

    COED::Encryptor encryptor("/tmp/sk.txt", "/tmp/pk.txt",
                              plaintext_prime_modulus,
                              phiM,
                              lifting,
                              numOfBitsOfModulusChain,
                              numOfColOfKeySwitchingMatrix);
    COED::Util::info("Finished creating encryptor.");
    std::cout << "\n***** Demonstration 1: Homomorphic Calculator *****\n";
    Calculator::run_calculator(encryptor, plaintext_prime_modulus);

    std::cout << "\n***** Demonstration 2: Dot Product of 2 vectors *****\n";
    DotProduct::run_program(encryptor);

    std::cout << "\n***** Demonstration 3: Matrix Multiplier (3x3) *****\n";
    MatrixMultiplier::main(encryptor);

    std::cout << "\n***** Demonstration 4: Convolution Filter Evaluation *****\n";
    ConvolutionFilterEvaluator::main(encryptor);

    return 0;
}
