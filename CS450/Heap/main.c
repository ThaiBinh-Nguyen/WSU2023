#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>


#define NUM_SKILLS 5

// Sorting in descending order
int compare(const void *a, const void *b) {
    return *((int*)b) - *((int*)a);
}

// Returns the correct position of the pivot element
int hoare_partition(int arr[], int low, int high) {
    
    // Choose the pivot as the first element
    int pivot = arr[low];
    int i = low - 1;
    int j = high + 1;

    while (1) {
        do {
            i++;
        } while (arr[i] > pivot);
        do {
            j--;
        } while (arr[j] < pivot);
        if (i >= j)
            return j;

        // Swap them
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Sorts the array in-place in descending order
void tail_recursive_quick_sort(int arr[], int low, int high) {
    while (low < high) {
        // Partition the array
        int p = hoare_partition(arr, low, high);
        if (p - low < high - p) {
            tail_recursive_quick_sort(arr, low, p);
            low = p + 1;
        } else {
            tail_recursive_quick_sort(arr, p + 1, high);
            high = p;
        }
    }
}



int main(int argc, char *argv[]) {
    int *Skill_Array = NULL; // Declare Skill_Array
    int n = 0; // The number of player

    while (1) {
        int flag = 1;

        // Increse tje size of Skill_Array to store information of player
        Skill_Array = (int *)realloc(Skill_Array, (n + 1) * NUM_SKILLS * sizeof(int));
        if (Skill_Array == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(1);
        }

        // Get information for skill of player
        for (int j = 0; j < NUM_SKILLS; ++j) {
            if (scanf("%d", &Skill_Array[n * NUM_SKILLS + j]) != 1) {
                flag = 0;
                break;
            }
        }

        // Stop the loop
        if (!flag) {
            break;
        }

        n++;
    }
    
    // Declare Sorted_array
    int *Sorted_array = (int *)malloc(n * sizeof(int)); 

    // Check the error
    if (Sorted_array == NULL) {
        exit(1);
    }
    const char *Skill_of_Player[] = {"SKILL_BREAKDANCING", "SKILL_APICULTURE", "SKILL_BASKET", "SKILL_XBASKET", "SKILL_SWORD", "TOTAL_XP"};

    for (int Position_of_Skill = 0; Position_of_Skill <= NUM_SKILLS; ++Position_of_Skill) {
        printf("%s\n", Skill_of_Player[Position_of_Skill]);

        // Prepare data for sorting
        for (int i = 0; i < n; ++i) {
            if (Position_of_Skill < NUM_SKILLS) {
                Sorted_array[i] = Skill_Array[i * NUM_SKILLS + Position_of_Skill];
            } else {
                Sorted_array[i] = 0;
                for (int j = 0; j < NUM_SKILLS; ++j) {
                    Sorted_array[i] += Skill_Array[i * NUM_SKILLS + j];
                }
            }
        }   
        
        struct timespec start, end;
        long time_in_micros;

        //If the user chooses the standard program
        if (strcmp(argv[1], "standard") == 0) {
            clock_gettime(CLOCK_MONOTONIC, &start);
            qsort(Sorted_array, n, sizeof(int), compare);
            clock_gettime(CLOCK_MONOTONIC, &end);

            time_in_micros = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;

            // Print Sorted_array values
            for (int i = 0; i < n; ++i) {
                printf("%d\n", Sorted_array[i]);
            }
            printf("time taken: %ld microseconds\n", time_in_micros);
            printf("\n");

        }
        //If the user chooses the custom program
        else if (strcmp(argv[1], "custom") == 0) {
            clock_gettime(CLOCK_MONOTONIC, &start);
            tail_recursive_quick_sort(Sorted_array, 0, n - 1);
            clock_gettime(CLOCK_MONOTONIC, &end);

            time_in_micros = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;

            // Print Sorted_array values
            for (int i = 0; i < n; ++i) {
                printf("%d\n", Sorted_array[i]);
            }
            printf("time taken: %ld microseconds\n", time_in_micros);
            printf("\n");
        }
    }
        
    //Free memory

    free(Sorted_array);
    free(Skill_Array);

    return 0;
}