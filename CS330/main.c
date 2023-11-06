#include <stdio.h>
#include <math.h>

int main() {
    // Initialize values
    double x = 0.25;
    double y = 0.25;
    double a = 0.25; // Set learning rate to 0.25
    double h1, h2, delta_h, hx, hy;

    printf("step\t\tx\t\ty\t\ta\t\th(x,y)\t\tΔh\n");
    printf("--------------------------------------------------------------\n");

    for (int step = 0; step < 6; step++) {
        // Calculate h for the current (x,y)
        h1 = pow((x*x + y*y - 1), 2) + pow((x*x - y), 2);

        // Calculate the gradient of h with respect to x and y
        hx = 2*(x*x + y*y - 1)*(2*x) + 2*(x*x - y)*(2*x);
        hy = 2*(x*x + y*y - 1)*(2*y) + 2*(x*x - y)*(-1);

        // Update x and y using the gradient
        x = x - a*hx;
        y = y - a*hy;

        // Calculate h for the new (x,y)
        h2 = pow((x*x + y*y - 1), 2) + pow((x*x - y), 2);

        // Calculate the change in h
        delta_h = h2 - h1;

        // Print the results for this step
        printf("%d\t\t%.5f\t%.5f\t%.3f\t%.5e\t%.5e\n", step, x, y, a, h2, delta_h);
    }

    return 0;
}
