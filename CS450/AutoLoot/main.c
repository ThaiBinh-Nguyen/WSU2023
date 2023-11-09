/*
Name: Thai Binh Nguyen
Project 4: Auto Loot
Date: 11-06-2023
Instructor: Dr. Williams
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define Max_weight_of_Item 1200
#define Max_number_of_Item 128
#define Max_Length_of_Name 128

typedef struct {
    char Name_of_Item[Max_Length_of_Name];
    int Weight_of_Item;
    int Value_of_Item;
} Item;

int max(int a, int b) {
    return (a > b) ? a : b;
}

void knapsack(int n, int W, Item Items[]) {
    int m[Max_number_of_Item+1][Max_weight_of_Item+1], selected[Max_number_of_Item+1];
    int i, j;

    //Create dynamic programming.
    for (j = 0; j <= W; j++) {
        m[0][j] = 0;
    }
    for (i = 1; i <= n; i++) {
        m[i][0] = 0;
    }

    //Build the table
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= W; j++) {
            if (Items[i-1].Weight_of_Item > j) {
                m[i][j] = m[i-1][j];
            } else {
                m[i][j] = max(m[i-1][j], m[i-1][j-Items[i-1].Weight_of_Item] + Items[i-1].Value_of_Item);
            }
        }
    }

    //The selected Items
    i = n; j = W;
    memset(selected, 0, sizeof(selected)); 
    while (i > 0 && j > 0) {
        if (m[i][j] != m[i-1][j]) {
            selected[i] = 1; 
            j = j - Items[i-1].Weight_of_Item;
        }
        i--;
    }

    //Print selected Items
    int Total_Weight = 0, Total_Value = 0;
    for (i = 1; i <= n; i++) {
        if (selected[i]) {
            printf("%s, %d, %d\n", Items[i-1].Name_of_Item, Items[i-1].Weight_of_Item, Items[i-1].Value_of_Item);
            Total_Weight += Items[i-1].Weight_of_Item;
            Total_Value += Items[i-1].Value_of_Item;
        }
    }

    //Print final weight and value.
    printf("final weight: %d\n", Total_Weight);
    printf("final value: %d\n", Total_Value);
}

int main() {
    
    struct timespec start, end;
    long time_in_micros;
    int Carry_Weight;
    Item Items[Max_number_of_Item];
    int Counting_Items = 0;

    //Get the carry weight
    scanf("%d\n", &Carry_Weight);

    // Đọc thông tin từng món đồ
    while (scanf("%[^;];%d;%d\n", Items[Counting_Items].Name_of_Item, &Items[Counting_Items].Weight_of_Item, &Items[Counting_Items].Value_of_Item) != EOF) {
        Counting_Items++;
        if (Counting_Items >= Max_number_of_Item) {
            break;
        }
    }

    //Knapsack Function
    clock_gettime(CLOCK_MONOTONIC, &start);
    knapsack(Counting_Items, Carry_Weight, Items);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_in_micros = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    printf("time taken in microseconds: %ld \n", time_in_micros);
    return 0;
}
