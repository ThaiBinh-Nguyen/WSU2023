/*
Full Name: Thai Binh Nguyen
Course: CS450
Instructor: Dr. Grant Williams
Date: 09/16/2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Function to calculate the Determinant of a Matrix
double Find_Determinant(double **Matrix, int size){
    double Determinant = 1.0;

    //Create the triangular matrix
    for (int Current_Row = 0; Current_Row < size; Current_Row++){

        //Find the row with the maximum absolute value in the current column
        int Find_Pivot = Current_Row;
        for(int Next_Row = Current_Row + 1; Next_Row < size; Next_Row++){
            if(fabs(Matrix[Next_Row][Current_Row]) > fabs(Matrix[Find_Pivot][Current_Row])){
                Find_Pivot = Next_Row;
            }
        }

        //If the Pivot isn't the Current_Row, swap rows and change determinant sign
        if(Find_Pivot != Current_Row){
            double *Temp_Row = Matrix[Current_Row];
            Matrix[Current_Row] = Matrix[Find_Pivot];
            Matrix[Find_Pivot] = Temp_Row;
            Determinant = Determinant * (-1); //Change determinant sign
        }

        //Eliminate rows below
        for(int Next_Row = Current_Row + 1; Next_Row < size; Next_Row++){
            double Factor = Matrix[Next_Row][Current_Row] / Matrix[Current_Row][Current_Row];
            for(int Column = Current_Row; Column < size; Column++){
                Matrix[Next_Row][Column] = Matrix[Next_Row][Column] - (Factor * Matrix[Current_Row][Column]);
            }
        }

        //Multipkying Determinant by the element on the current diagonal
        Determinant = Determinant * Matrix[Current_Row][Current_Row];
    }

    return Determinant;
}


int main(){
    int Size_of_Matrix_N;
    double **Matrix;

    //Read the siez of the Matrix
    scanf("%d", &Size_of_Matrix_N);

    //Initialixe the Matrix
    Matrix = (double **)malloc(Size_of_Matrix_N * sizeof(double *));
    for(int i = 0; i < Size_of_Matrix_N; i++){
        Matrix[i] = (double *)malloc(Size_of_Matrix_N * sizeof(double));
    }

    //Read the information of the Matrix from the input
    for(int i = 0; i < Size_of_Matrix_N; i++){
        for(int j = 0; j < Size_of_Matrix_N; j++){
            scanf("%lf", &Matrix[i][j]);
        }
    }

    //Calculate and prunt the Determinant of the Matrix
    double determinant = Find_Determinant(Matrix, Size_of_Matrix_N);
    printf("%.0lf\n", determinant);

    //Free the memory of the Matrix
    for(int i = 0; i < Size_of_Matrix_N; i++){
        free(Matrix[i]);
    }
    free(Matrix);

    return 0;
}