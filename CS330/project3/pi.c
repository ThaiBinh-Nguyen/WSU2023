/* Full Name: Thai Binh Nguyen
   Course: CS330
   Project 3: Estimating pi via Numerical Integration
   Date: 11-24-2023
*/

/* Usage:
        1. use "make" to run Makefile to compile all of files
        2. use "./pi" to show the the absolute error values
        3. use "./test.pl" to test my code
*/

/* Identifies the project:
My program implements numerical methods for approximating definite integrals, particularly focusing on estimating the value of π. 

1. Numerical Integration Methods

Trapezoidal Rule (trap function): This method approximates the area under the curve as a series of trapezoids. The function takes the function f(x), 
integration limits a and b, and the number of intervals n, then calculates the integral using the trapezoidal rule.

Simpson's 1/3 Rule (simpsons function): This method provides a more accurate approximation by fitting a parabola through each pair of points. 
The implementation checks that n is even, as required by this method, and then applies Simpson's 1/3 rule.

Simpson's 3/8 Rule (simpsons38 function): Similar to Simpson's 1/3, but it uses groups of three intervals for a better approximation. 
It ensures that n is a multiple of 3.

Boole's Rule (booles function): This is an extension of Simpson's rules, using 4th-degree polynomials. 
It adjusts n to be a multiple of 4 and then applies Boole's rule for integration.



*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

// Define M_PI if it's not already defined
#ifndef M_PI 
#define M_PI 3.14159265358979323846264338327950288
#endif


// Trapezoidal Rule for numerical integration
long double trap(long double(*f)(long double), long double a, long double b, int n) {

  const long double h = (b - a)/n;
  long double Integral = 0.5L * (f(a) + f(b));
    for (int i = 1; i < n; i++) {
        Integral += f(a + i * h);
    }
    return Integral * h;

}

// Simpson's 1/3 Rule for numerical integration
long double simpsons(long double(*f)(long double), long double a, long double b, int n) {
    assert(n % 2 == 0); // n phải là số chẵn
    const long double h = (b - a) / n;
    long double Integral = f(a) + f(b);
    
    for (int i = 1; i < n; i++) {
        Integral += (i % 2 == 0) ? 2 * f(a + i * h) : 4 * f(a + i * h);
    }
    return Integral * h / 3.0L;
}

// Simpson's 3/8 Rule for numerical integration
long double simpsons38(long double(*f)(long double), long double a, long double b, int n) {
    assert(n % 3 == 0); // n phải chia hết cho 3
    const long double h = (b - a) / n;
    long double Integral = f(a) + f(b);
    
    for (int i = 1; i < n; i++) {
        Integral += (i % 3 == 0) ? 2 * f(a + i * h) : 3 * f(a + i * h);
    }
    return Integral * 3.0L * h / 8.0L;
}

// Boole's Rule for numerical integration
long double booles(long double(*f)(long double), long double a, long double b, int n) {
  // Adjust n to be a multiple of 4
  while (n % 4 != 0) n++;
  const long double h = (b - a) / n;
  long double Integral = 7 * (f(a) + f(b));
  for (int i = 1; i < n; i++) {
    if (i % 4 == 0) {
      Integral += 14 * f(a + i * h);
    } else if (i % 4 == 1 || i % 4 == 3) {
      Integral += 32 * f(a + i * h);
    } else if (i % 4 == 2) {
      Integral += 12 * f(a + i * h);
    }
  }
  Integral *= 2 * h / 45.0L;
  return Integral;
}


// f(x)
long double f(long double x) {
  return 4.0L/(1.0L + x*x);
}

int main(void) {
    long double a = 0.0L, b = 1.0L; // Integration limits
    long double Result_of_Trap, Result_of_Simp13, Result_of_Simp38, Result_of_Booles;
    long double Error_of_Trap, Error_of_Simp13, Error_of_Simp38, Error_of_Booles;
    
    // Print headers for the results table
    printf("N\tTrapezoid\tSimpson1/3\tSimpson3/8\tBoole\n");

    for (int i = 0; i <= 16; i++) {
        int n = 12 * pow(2, i); // n is a multiple of 12


        // Compute integrals using different methods
        Result_of_Trap = trap(f, a, b, n);
        Result_of_Simp13 = simpsons(f, a, b, n);
        Result_of_Simp38 = simpsons38(f, a, b, n);
        Result_of_Booles = booles(f, a, b, n);


        // Calculate absolute errors
        Error_of_Trap = fabsl(Result_of_Trap - M_PI);
        Error_of_Simp13 = fabsl(Result_of_Simp13 - M_PI);
        Error_of_Simp38 = fabsl(Result_of_Simp38 - M_PI);
        Error_of_Booles = fabsl(Result_of_Booles - M_PI);

        // Print the results with specified precision
        printf("%d\t%0.10Le\t%0.10Le\t%0.10Le\t%0.10Le\n", n, Error_of_Trap, Error_of_Simp13, Error_of_Simp38, Error_of_Booles);
    }

    return 0;
}

