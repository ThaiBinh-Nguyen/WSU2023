#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Function to calculate the determinant of a matrix
double calculateDeterminant(double **matrix, int size) {
    double determinant = 1.0;

    for (int currentRow = 0; currentRow < size; currentRow++) {
        // Find the row with the maximum absolute value in the current column
        int pivotRow = currentRow;
        for (int nextRow = currentRow + 1; nextRow < size; nextRow++) {
            if (fabs(matrix[nextRow][currentRow]) > fabs(matrix[pivotRow][currentRow])) {
                pivotRow = nextRow;
            }
        }

        // If the pivot row is not the current row, swap rows
        if (pivotRow != currentRow) {
            double *tempRow = matrix[currentRow];
            matrix[currentRow] = matrix[pivotRow];
            matrix[pivotRow] = tempRow;
            determinant = -determinant; // Change determinant sign
        }

        // Eliminate rows below (to keep elements below diagonal equal to 0)
        for (int nextRow = currentRow + 1; nextRow < size; nextRow++) {
            double factor = matrix[nextRow][currentRow] / matrix[currentRow][currentRow];
            for (int col = currentRow; col < size; col++) {
                matrix[nextRow][col] -= factor * matrix[currentRow][col];
            }
        }

        // Multiply determinant by the element on the current diagonal
        determinant *= matrix[currentRow][currentRow];
    }

    return determinant;
}

int main() {
    FILE *file = fopen("project1_key.txt", "r");

    char line[100];
    int matrixSize = -1;
    double **matrix = NULL;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, "input:") != NULL) {
            // Free memory for the previous matrix, if any
            if (matrix != NULL) {
                double det = calculateDeterminant(matrix, matrixSize);
                printf("Determinant is: %.0lf\n", det);

                // Free memory for the current matrix
                for (int i = 0; i < matrixSize; i++) {
                    free(matrix[i]);
                }
                free(matrix);
            }

            // Read matrix size from the file
            fscanf(file, "%d", &matrixSize);

            // Initialize the matrix
            matrix = (double **)malloc(matrixSize * sizeof(double *));
            for (int i = 0; i < matrixSize; i++) {
                matrix[i] = (double *)malloc(matrixSize * sizeof(double));
            }

            // Read data for the matrix
            for (int i = 0; i < matrixSize; i++) {
                for (int j = 0; j < matrixSize; j++) {
                    fscanf(file, "%lf", &matrix[i][j]);
                }
            }
        }
    }

    // Calculate and print determinant for the last matrix
    if (matrix != NULL) {
        double det = calculateDeterminant(matrix, matrixSize);
        printf("Determinant is: %.0lf\n", det);

        // Free memory for the last matrix
        for (int i = 0; i < matrixSize; i++) {
            free(matrix[i]);
        }
        free(matrix);
    }

    fclose(file);
    return 0;
}
