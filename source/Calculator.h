//
// Created by soham on 10/10/20.
//

#ifndef HOMOMORPHIC_DEMONSTRATIONS_CALCULATOR_H
#define HOMOMORPHIC_DEMONSTRATIONS_CALCULATOR_H


#include "Encryptor.h"

class Calculator {
public:
    static void run_calculator();

    static void add(helib::Ctxt *a, const helib::Ctxt& b);

    static void subtract(helib::Ctxt *a, const helib::Ctxt& b);

    static void multiply(helib::Ctxt *a, const helib::Ctxt& b);

    static void divide(helib::Ctxt *a, const helib::Ctxt& b, int plaintext_prime_modulus);

private:
    static void accept_inputs(int *a, int *b, char* op);
    static void exit_on_invalid_op(char op);
};


#endif //HOMOMORPHIC_DEMONSTRATIONS_CALCULATOR_H
