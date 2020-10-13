//
// Created by soham on 10/12/20.
//

#include <iostream>
#include "DotProduct.h"
#include "Util.h"
#include "Encryptor.h"

#define MAX_VECTOR_SIZE 5

void DotProduct::run_dot_product() {
    int plaintext_prime_modulus = 53;
    int phiM = 3000;
    int lifting = 1;
    int numOfBitsOfModulusChain = 512;
    int numOfColOfKeySwitchingMatrix = 2;

    COED::Util::info("Starting program ...");

    COED::Encryptor encryptor("/tmp/sk.txt", "/tmp/pk.txt",
                              plaintext_prime_modulus,
                              phiM,
                              lifting,
                              numOfBitsOfModulusChain,
                              numOfColOfKeySwitchingMatrix);

    int a[MAX_VECTOR_SIZE];
    int b[MAX_VECTOR_SIZE];
    accept_inputs(a, b);

    helib::Ptxt<helib::BGV> ptxt_a(*(encryptor.getContext()));
    helib::Ptxt<helib::BGV> ptxt_b(*(encryptor.getContext()));

    helib::Ctxt ctxt_a(*(encryptor.getPublicKey()));
    helib::Ctxt ctxt_b(*(encryptor.getPublicKey()));

    for (int i = 1; i <= MAX_VECTOR_SIZE; ++i) {
        ptxt_a[i] = a[i-1];
        ptxt_b[i] = b[i-1];
    }

    encryptor.getPublicKey()->Encrypt(ctxt_a, ptxt_a);
    encryptor.getPublicKey()->Encrypt(ctxt_b, ptxt_b);

    ctxt_a.multiplyBy(ctxt_b);
    helib::totalSums(*encryptor.getEncryptedArray(), ctxt_a);

    std::vector<long> plaintext(encryptor.getEncryptedArray()->size());
    encryptor.getEncryptedArray()->decrypt(ctxt_a, *encryptor.getSecretKey(), plaintext);
    std::cout << "Dot Product: " << plaintext[1]<<std::endl;

}

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