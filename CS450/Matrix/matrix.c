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
    int matrixSize;
    double **matrix = NULL;

    // Read the size of the first matrix
    scanf("%d", &matrixSize);

    // Consume the trailing newline character
    getchar();

    // Initialize the matrix
    matrix = (double **)malloc(matrixSize * sizeof(double *));
    for (int i = 0; i < matrixSize; i++) {
        matrix[i] = (double *)malloc(matrixSize * sizeof(double));
    }

    // Read data for the matrix
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            scanf("%lf", &matrix[i][j]);
        }
    }

    // Calculate and print the determinant of the matrix
    double det = calculateDeterminant(matrix, matrixSize);
    printf("%.0lf\n", det);

    // Free the memory
    for (int i = 0; i < matrixSize; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}