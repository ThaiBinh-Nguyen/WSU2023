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
  float z = x / M_LN2;
  int m = round(z);
  float w = z - m;
  float u = w*M_LN2;

  // Compute e^u using the smallest_n found earlier (you can hard-code this value)
  float e_u = 1.0;  // Initialize to the last coefficient of the polynomial
  int smallest_n = 7;  // Replace with the smallest_n you found using rerr.c

  for (int n = smallest_n; n >= 1; n--) {
      e_u = 1 + (u/n)*e_u;
  }

  // Compute 2^m
  float two_m = ldexpf(1, m);

  return (float)(two_m * e_u);
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "usage: %s <number>\n", argv[0]);
    exit(1);
  }

  const float x = atof(argv[1]);
  const float y = myexp(x);

  printf("%0.9e\n", y);

  return 0;
}
