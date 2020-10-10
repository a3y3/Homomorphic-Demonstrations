//
// Created by soham on 10/10/20.
//

#ifndef HOMOMORPHIC_DEMONSTRATIONS_CALCULATOR_H
#define HOMOMORPHIC_DEMONSTRATIONS_CALCULATOR_H


class Calculator {
public:
    static void run_calculator();

    int add(int a, int b);

    int substract(int a, int b);

    int multiply(int a, int b);

    int divide(int a, int b);

private:
    static void accept_inputs(int *a, int *b, char* op);
    static void exit_on_invalid_op(char op);
};


#endif //HOMOMORPHIC_DEMONSTRATIONS_CALCULATOR_H
