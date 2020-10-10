//
// Created by soham on 10/10/20.
//

#include "Calculator.h"
#include <iostream>

int Calculator::add(int a, int b) {
    return 0;
}

void Calculator::run_calculator() {
    int a, b;
    char op;
    accept_inputs(&a, &b, &op);
    exit_on_invalid_op(op);

}

void Calculator::accept_inputs(int *a, int *b, char *op) {
    std::cout << "X=? ";
    std::cin >> *a;

    std::cout << "Y=? ";
    std::cin >> *b;

    std::cout << "operator? (+-*/)";
    std::cin >> *op;
}

void Calculator::exit_on_invalid_op(char op) {
    if (op != '+' && op != '-' && op != '*' && op != '/') {
        std::cout<<"Fatal: Invalid operator, must be one of +-*/";
        exit(126);
    }
}
