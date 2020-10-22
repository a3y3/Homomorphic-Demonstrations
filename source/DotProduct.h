//
// Created by soham on 10/12/20.
//

#ifndef HOMOMORPHIC_DEMONSTRATIONS_DOTPRODUCT_H
#define HOMOMORPHIC_DEMONSTRATIONS_DOTPRODUCT_H

#include "Encryptor.h"

class DotProduct {
public:
    static void run_program(const COED::Encryptor &encryptor);
    static helib::Ctxt dot_product(const helib::Ctxt& a, const helib::Ctxt &b, const COED::Encryptor& encryptor);
private:
    static void accept_inputs(int *a, int *b);
};

#endif //HOMOMORPHIC_DEMONSTRATIONS_DOTPRODUCT_H
