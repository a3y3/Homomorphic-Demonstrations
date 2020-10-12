//
// Created by soham on 10/10/20.
//

#include "Calculator.h"
#include "Encryptor.h"
#include "Util.h"
#include <iostream>

/**
 * Adds 2 ciphertexts, storing the result in @param a.
 * @param a First ciphertext
 * @param b Second ciphertext
**/
void Calculator::add(helib::Ctxt *a, const helib::Ctxt &b) {
    a->addCtxt(b);
}

/**
 * Subtracts 2 ciphertexts, storing the result in @param a.
 * @param a First ciphertext
 * @param b Second ciphertext
**/
void Calculator::subtract(helib::Ctxt *a, const helib::Ctxt &b) {
    a->addCtxt(b, true);
}

/**
 * Multiplies 2 ciphertexts, storing the result in @param a.
 * @param a First ciphertext
 * @param b Second ciphertext
**/
void Calculator::multiply(helib::Ctxt *a, const helib::Ctxt &b) {
    a->multiplyBy(b);
}

/**
 * Divides 2 ciphertexts, storing the result in @param a.
 * @param a First ciphertext
 * @param b Second ciphertext
**/
void Calculator::divide(helib::Ctxt *a, const helib::Ctxt &b, int plaintext_prime_modulus) {
    //Division uses Fermat's Little Theorm, a^-1=a^(p-2) mod p
    helib::Ctxt ctxt_divisor(b);
    ctxt_divisor.power(plaintext_prime_modulus - 2);
    a->multiplyBy(ctxt_divisor);
}

/**
 * Driver function. Runs the calculator in a loop until the user chooses to exit.
**/
void Calculator::run_calculator() {
    int a, b;
    char op;
    char continue_or_exit;
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
    // a and b are integers, we need to convert them into "plaintext" objects.
    helib::Ptxt<helib::BGV> ptxt_a(*(encryptor.getContext()));
    helib::Ptxt<helib::BGV> ptxt_b(*(encryptor.getContext()));

    // create ciphertext objects for each and encrypt them using the public key
    helib::Ctxt ctxt_a(*(encryptor.getPublicKey()));
    helib::Ctxt ctxt_b(*(encryptor.getPublicKey()));

    do {
        accept_inputs(&a, &b, &op);
        exit_on_invalid_op(op);

        ptxt_a[1] = a;
        ptxt_b[1] = b;

        encryptor.getPublicKey()->Encrypt(ctxt_a, ptxt_a);
        encryptor.getPublicKey()->Encrypt(ctxt_b, ptxt_b);

        switch (op) {
            case '+':
                add(&ctxt_a, ctxt_b);
                break;
            case '-':
                subtract(&ctxt_a, ctxt_b);
                break;
            case '*':
                multiply(&ctxt_a, ctxt_b);
                break;
            case '/':
                divide(&ctxt_a, ctxt_b, plaintext_prime_modulus);
                break;
            default:
                COED::Util::error(
                        "Fatal: Program flow reached unintended destination, exit_on_invalid_op() didn't work as intended!");
                exit(1);
        }
        std::vector<long> plaintext(encryptor.getEncryptedArray()->size());
        encryptor.getEncryptedArray()->decrypt(ctxt_a, *encryptor.getSecretKey(), plaintext);
        std::cout << "Result: " << plaintext[1]<<std::endl;

        std::cout << "Try another operation? (y/n)" << std::endl;
        std::cin >> continue_or_exit;
    } while (continue_or_exit == 'y');
}

/**
 * Accepts int a, int b and an operator from the user. This function doesn't filter inputs, that functionality should be
 * offloaded to another method.
 * @param a pointer to the first input
 * @param b pointer to the second input
 * @param op the operator to apply on operands a and b. Currently supports +, -, *, and /.
**/
void Calculator::accept_inputs(int *a, int *b, char *op) {
    std::cout << "X=? ";
    std::cin >> *a;

    std::cout << "Y=? ";
    std::cin >> *b;

    std::cout << "operator? (+-*/) ";
    std::cin >> *op;
}
/**
 * If the operator is not supported, this method throws a fatal error and exits with 126.
 * @param op the operator to be validated.
*/
void Calculator::exit_on_invalid_op(char op) {
    if (op != '+' && op != '-' && op != '*' && op != '/') {
        COED::Util::error("Fatal: Invalid operator, must be one of +-*/");
        exit(126);
    }
}




