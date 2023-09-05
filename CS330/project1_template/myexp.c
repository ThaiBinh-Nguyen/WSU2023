#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_LOG2E
#define M_LOG2E   1.44269504088896340735992468100189214
#endif

#ifndef M_LN2
#define M_LN2  0.693147180559945309417232121458176568
#endif

float myexp(float x) {
  /* Your code here */
  
  return 0.0;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s <number>\n", argv[0]);
    exit(1);
  }

  const float x = atof(argv[1]);
  const float y = myexp(x);

  printf("%0.9e\n", y);

  return 0;
}
