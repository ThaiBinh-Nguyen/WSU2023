#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double determinant(double **M, int size) {
    double det = 1.0;
    
    // Transform the matrix into upper triangular form
    for (int i = 0; i < size; i++) {
        // Find the pivot
        int pivotRow = i;
        for (int j = i + 1; j < size; j++) {
            if (fabs(M[j][i]) > fabs(M[pivotRow][i])) {
                pivotRow = j;
            }
        }
        
        if (pivotRow != i) {
            // Swap rows
            double *tempRow = M[i];
            M[i] = M[pivotRow];
            M[pivotRow] = tempRow;
            
            // Update determinant sign
            det *= -1.0;
        }
        
        if (fabs(M[i][i]) < 1e-10) {
            // Singular matrix, determinant is zero
            return 0.0;
        }
        
        // Eliminate other rows below
        for (int j = i + 1; j < size; j++) {
            double factor = M[j][i] / M[i][i];
            for (int k = i; k < size; k++) {
                M[j][k] -= factor * M[i][k];
            }
        }
        
        det *= M[i][i];
    }

    return det;
}

int main()  {
    FILE *file = fopen("project1_key.txt", "r");
    if (file == NULL) {
        printf("Can not open text file!!");
        return 1;
    }

    char line[100];
    int N = -1;
    double **matrix = NULL;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, "input:") != NULL) {
            if (matrix != NULL) {
                double det = determinant(matrix, N);
                printf("Det is: %.2lf\n", det);

                // Free memory for the current matrix
                for (int i = 0; i < N; i++) {
                    free(matrix[i]);
                }
                free(matrix);
            }

            fscanf(file, "%d", &N);
            printf("N is: %d\n", N);

            // Initialize matrix
            matrix = (double **)malloc(N * sizeof(double *));
            for (int i = 0; i < N; i++) {
                matrix[i] = (double *)malloc(N * sizeof(double));
            }

            // Read information for matrix.
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    fscanf(file, "%lf", &matrix[i][j]);
                }
            }
        }
    }

    // Calculate and print determinant for the last matrix
    if (matrix != NULL) {
        double det = determinant(matrix, N);
        printf("Det is: %.2lf\n", det);

        // Free memory for the last matrix
        for (int i = 0; i < N; i++) {
            free(matrix[i]);
        }
        free(matrix);
    }

    fclose(file);
    return 0;
}
