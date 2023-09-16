#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Function to calculate the Determinant of a Matrix
double Find_Determinant(double **Matrix, int size) {
    double Determinant = 1.0;

    for (int Current_Row = 0; Current_Row < size; Current_Row++) {
        // Find the row with the maximum absolute value in the current column
        int Find_Pivot = Current_Row;
        for (int Next_Row = Current_Row + 1; Next_Row < size; Next_Row++) {
            if (fabs(Matrix[Next_Row][Current_Row]) > fabs(Matrix[Find_Pivot][Current_Row])) {
                Find_Pivot = Next_Row;
            }
        }

        // If the pivot row is not the current row, swap rows
        if (Find_Pivot != Current_Row) {
            double *Temp_Row = Matrix[Current_Row];
            Matrix[Current_Row] = Matrix[Find_Pivot];
            Matrix[Find_Pivot] = Temp_Row;
            Determinant = Determinant * (-1); // Change Determinant sign
        }

        // Eliminate rows below (to keep elements below diagonal equal to 0)
        for (int Next_Row = Current_Row + 1; Next_Row < size; Next_Row++) {
            double Factor = Matrix[Next_Row][Current_Row] / Matrix[Current_Row][Current_Row];
            for (int Column = Current_Row; Column < size; Column++) {
                Matrix[Next_Row][Column] = Matrix[Next_Row][Column] - Factor * Matrix[Current_Row][Column];
            }
        }

        // Multiply Determinant by the element on the current diagonal
        Determinant = Determinant * Matrix[Current_Row][Current_Row];
    }

    return Determinant;
}

int main() {
    int Size_of_Matrix;
    double **Matrix = NULL;

    // Read the size of the first Matrix
    scanf("%d", &Size_of_Matrix);

    // Consume the trailing newline character
    getchar();

    // Initialize the Matrix
    Matrix = (double **)malloc(Size_of_Matrix * sizeof(double *));
    for (int i = 0; i < Size_of_Matrix; i++) {
        Matrix[i] = (double *)malloc(Size_of_Matrix * sizeof(double));
    }

    // Read data for the Matrix
    for (int i = 0; i < Size_of_Matrix; i++) {
        for (int j = 0; j < Size_of_Matrix; j++) {
            scanf("%lf", &Matrix[i][j]);
        }
    }

    // Calculate and print the Determinant of the Matrix
    double det = Find_Determinant(Matrix, Size_of_Matrix);
    printf("%.0lf\n", det);

    // Free the memory
    for (int i = 0; i < Size_of_Matrix; i++) {
        free(Matrix[i]);
    }
    free(Matrix);

    return 0;
}