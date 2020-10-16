//
// Created by soham on 10/12/20.
//

#include <iostream>
#include "DotProduct.h"
#include "Encryptor.h"

#define MAX_VECTOR_SIZE 5

/**
 * Runs the main functionality of the dot product.
 * This function creates 2 ciphertexts, calculates their product, then uses helib's totalSums operation to calculate
 * the sum.
 */
void DotProduct::run_program(const COED::Encryptor &encryptor) {
    int a[MAX_VECTOR_SIZE];
    int b[MAX_VECTOR_SIZE];
    accept_inputs(a, b);

    // Create plain text objects. Later, the array elements are added to these.
    helib::Ptxt<helib::BGV> ptxt_a(*(encryptor.getContext()));
    helib::Ptxt<helib::BGV> ptxt_b(*(encryptor.getContext()));

    // Create ciphertext objects. The plaintext objects are encrypted by the public key in the ciphertexts.
    helib::Ctxt ctxt_a(*(encryptor.getPublicKey()));
    helib::Ctxt ctxt_b(*(encryptor.getPublicKey()));

    for (int i = 1; i <= MAX_VECTOR_SIZE; ++i) {
        ptxt_a[i] = a[i - 1];
        ptxt_b[i] = b[i - 1];
    }

    encryptor.getPublicKey()->Encrypt(ctxt_a, ptxt_a);
    encryptor.getPublicKey()->Encrypt(ctxt_b, ptxt_b);

    DotProduct::dot_product(&ctxt_a, ctxt_b, encryptor);

    //Decrypt and show result
    std::vector<long> plaintext(encryptor.getEncryptedArray()->size());
    encryptor.getEncryptedArray()->decrypt(ctxt_a, *encryptor.getSecretKey(), plaintext);
    std::cout << "Dot Product: " << plaintext[1] << std::endl;
}

/**
 * Accepts inputs in arrays
 * @param a First array
 * @param b Second array
 */
void DotProduct::accept_inputs(int *a, int *b) {
    std::cout << "Enter " << MAX_VECTOR_SIZE << " elements for Vector A (Hit <Return> for inputting each element)"
              << std::endl;
    for (int i = 0; i < MAX_VECTOR_SIZE; ++i) {
        std::cin >> a[i];
    }
    std::cout << "Enter " << MAX_VECTOR_SIZE << " elements for Vector B" << std::endl;
    for (int i = 0; i < MAX_VECTOR_SIZE; ++i) {
        std::cin >> b[i];
    }
}

void DotProduct::dot_product(helib::Ctxt *a, helib::Ctxt &b, const COED::Encryptor& encryptor) {
    //Multiply and find sum
    a->multiplyBy(b);
    helib::totalSums(*encryptor.getEncryptedArray(), *a);
}

