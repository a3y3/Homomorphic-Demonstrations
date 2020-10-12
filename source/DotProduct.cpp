//
// Created by soham on 10/12/20.
//

#include <iostream>
#include "DotProduct.h"
#define MAX_VECTOR_SIZE 5

void DotProduct::run_dot_product() {
    int a[MAX_VECTOR_SIZE];
    int b[MAX_VECTOR_SIZE];
    accept_inputs(a, b);
}

void DotProduct::accept_inputs(int *a, int *b) {
    std::cout<<"Enter "<<MAX_VECTOR_SIZE<<" elements for Vector A (Hit <Return> for inputting each element)"<<std::endl;
    for (int i = 0; i < MAX_VECTOR_SIZE; ++i) {
        std::cin>>a[i];
    }
    std::cout<<"Enter "<<MAX_VECTOR_SIZE<<" elements for Vector B"<<std::endl;
    for (int i = 0; i < MAX_VECTOR_SIZE; ++i) {
        std::cin>>b[i];
    }
}
