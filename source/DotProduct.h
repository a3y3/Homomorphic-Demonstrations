//
// Created by soham on 10/12/20.
//

#ifndef HOMOMORPHIC_DEMONSTRATIONS_DOTPRODUCT_H
#define HOMOMORPHIC_DEMONSTRATIONS_DOTPRODUCT_H

#include "Encryptor.h"

class DotProduct {
public:
    static void run_dot_product();
private:
    static void accept_inputs(int *a, int *b);
    static void sum_ctxt_elements(helib::Ctxt *a, COED::Encryptor);
};

#endif //HOMOMORPHIC_DEMONSTRATIONS_DOTPRODUCT_H
