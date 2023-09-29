#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


double f(double x) {
    return exp(-x) - cos(x);
}

double fprime(double x) {
    return -exp(-x) + sin(x);
}

double newton(double (*f)(double), double (*fprime)(double), double x0, double tol, int max_iter) {
    double x = x0;
    double fx = f(x);
    double fpx;
    
    printf("Starting Newton'Sine method with initial guess x = %f\n", x0);

    for (int i = 0; i < max_iter; i++) {
        fpx = fprime(x);

        printf("Iteration %d: x = %f, f(x) = %f, f'(x) = %f\n", i, x, fx, fpx);

        x = x - fx / fpx;
        fx = f(x);

        if (fabs(fx) < tol) {
            printf("Algorithm converged after %d iterations.\n", i + 1);
            break;
        }
    }

    printf("Approximate root is: %f\n", x);
    printf("Value of f(x) at the approximate root is: %f\n", fx);

    return x;
}

int main() {
    double x0 = M_PI / 2;
    double tol = 1e-5;
    int max_iter = 100;

    newton(f, fprime, x0, tol, max_iter);

    return 0;
}
