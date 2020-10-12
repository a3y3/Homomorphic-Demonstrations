//
// Created by soham on 10/10/20.
//

#include "Calculator.h"
#include "Encryptor.h"
#include "Util.h"
#include <iostream>

void Calculator::add(helib::Ctxt *a, const helib::Ctxt& b) {
    a->addCtxt(b);
}

void Calculator::run_calculator() {
    int a, b;
    char op;
    int plaintext_prime_modulus = 53;
    int phiM = 2000;
    int lifting = 1;
    int numOfBitsOfModulusChain = 512;
    int numOfColOfKeySwitchingMatrix = 2;

    COED::Util::info("Starting calculator ...");

    COED::Encryptor encryptor("/tmp/sk.txt", "/tmp/pk.txt",
                              plaintext_prime_modulus,
                              phiM,
                              lifting,
                              numOfBitsOfModulusChain,
                              numOfColOfKeySwitchingMatrix);

    accept_inputs(&a, &b, &op);
    exit_on_invalid_op(op);

    helib::Ptxt<helib::BGV> ptxt_a(*(encryptor.getContext()));
    ptxt_a[1] = a;

    helib::Ptxt<helib::BGV> ptxt_b(*(encryptor.getContext()));
    ptxt_b[1] = b;

    helib::Ctxt ctxt_a(*(encryptor.getPublicKey()));
    encryptor.getPublicKey()->Encrypt(ctxt_a, ptxt_a);
    helib::Ctxt ctxt_b(*(encryptor.getPublicKey()));
    encryptor.getPublicKey()->Encrypt(ctxt_b, ptxt_b);

    switch (op) {
        case '+': add(&ctxt_a, ctxt_b);
            break;
    }
    std::vector<long> plaintext(encryptor.getEncryptedArray()->size());
    encryptor.getEncryptedArray()->decrypt(ctxt_a, *encryptor.getSecretKey(), plaintext);
    std::cout<<"Result: " <<plaintext[1];
}

void Calculator::accept_inputs(int *a, int *b, char *op) {
    std::cout << "X=? ";
    std::cin >> *a;

    std::cout << "Y=? ";
    std::cin >> *b;

    std::cout << "operator? (+-*/) ";
    std::cin >> *op;
}

void Calculator::exit_on_invalid_op(char op) {
    if (op != '+' && op != '-' && op != '*' && op != '/') {
        COED::Util::error("Fatal: Invalid operator, must be one of +-*/");
        exit(126);
    }
}
