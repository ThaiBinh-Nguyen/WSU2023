/* Full Name: Thai Binh Nguyen
   Course: CS330
   Project 1: Computing e^x
   Date: 09-12-2023
*/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int fact(int n)
{
  if (n <= 1)
    return 1;
  return n * fact(n - 1);
}

int main()
{
  double eps = 1.19209e-7; // machine-epsilon for floats, explicitly declared as double
  int smallest_n = -1;     // To store the smallest n that meets the criteria
  double upper_bound;      // explicitly declared as double

  printf(" n  |  Upper bound for relative error\n");
  printf("----|-------------------------------\n");

  for (int n = 1; n <= 15; ++n)
  {
    upper_bound = (2.0 / fact(n + 1)) * pow((log(2.0) / 2.0), (double)(n + 1));

    printf("%2d  |  %.10e\n", n, upper_bound);

    if (upper_bound < eps && smallest_n == -1)
    {
      smallest_n = n;
    }
  }

  if (smallest_n != -1)
  {
    printf("\nThe smallest n such that the relative error is guaranteed to be below machine-epsilon is: %d\n", smallest_n);
  }
  else
  {
    printf("\nNo value of n from 1 to 15 meets the criteria.\n");
  }
  return 0;
}
