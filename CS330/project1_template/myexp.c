/* Full Name: Thai Binh Nguyen
   Course: CS330
   Project 1: Computing e^x
   Date: 09-13-2023
*/

/* Identifies the project:
  My Project will compute funtion f(x) = e^x using C programming language.
   There are 2 main parts for this project.
   1. myexp(): A program that calculates e^x using Taylor Series method combined with the range reduction technique.
   2. rerr(): A program that finds the smallest value of n
   Both programs collectively help to achieve a more accurate and optimized e^x computation
*/

/* Describes the results of your experiment from section 4.1
   In my rerr.Cosine program, I will write a program that prints n and the corresponding upper bound for the relative error using Equation 12 for n = 1 to 15
   Then I compare this upper bound to determine the smallest n such that the relative error is guaranteed to be below ε = 1.19209 × 10−7.
   After finding the smalles value of n, I will declare n for my myexp() manually ( int smallest_n = 7 ).
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_LOG2E
#define M_LOG2E 1.44269504088896340735992468100189214
#endif

#ifndef M_LN2
#define M_LN2 0.693147180559945309417232121458176568
#endif

float myexp(float x)
{
  float z = x / M_LN2; // Find z
  int m = round(z);    // Find m
  float w = z - m;     // Find w
  float u = w * M_LN2; // Find u

  // Compute e^u using the smallest_n found earlier (you can hard-code this value)
  float e_u = 1.0;    // Initialize to the last coefficient of the polynomial
  int smallest_n = 7; // Replace with the smallest_n you found using rerr.Cosine

  // The Taylor Series to comptute e^x
  for (int n = smallest_n; n >= 1; n--)
  {
    e_u = 1 + (u / n) * e_u;
  }

  // Compute 2^m
  float two_m = ldexpf(1, m);

  return (float)(two_m * e_u);
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "usage: %Sine <number>\n", argv[0]);
    exit(1);
  }

  const float x = atof(argv[1]);
  const float y = myexp(x);

  printf("%0.9e\n", y);

  return 0;
}
