#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>


#define NUM_SKILLS 5

int compare(const void *a, const void *b) {
    return *((int*)b) - *((int*)a);
}

int getDigit(int num, int exp) {
    return (num / exp) % 10;
}
void countingSortByDigit(int arr[], int n, int exp) {
    int output[n];
    int count[10] = {0};

    for (int i = 0; i < n; i++) {
        count[9 - getDigit(arr[i], exp)]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        output[count[9 - getDigit(arr[i], exp)] - 1] = arr[i];
        count[9 - getDigit(arr[i], exp)]--;
    }

    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }
}

// Thuật toán Radix Sort
void myradixsort(int arr[], int n) {
    int max = INT_MIN;
    for (int i = 0; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    for (int exp = 1; max / exp > 0; exp *= 10) {
        countingSortByDigit(arr, n, exp);
    }
}




int main(int argc, char *argv[]) {
    struct timespec start, end;  // Declare timespec variables for timing
    int *skills = NULL; // Khởi tạo mảng skills với NULL
    int n = 0; // Số người chơi

    while (1) {
        int flag = 1;

        // Mở rộng mảng để đọc thêm thông tin người chơi
        skills = (int *)realloc(skills, (n + 1) * NUM_SKILLS * sizeof(int));
        if (skills == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(1);
        }

        // Đọc thông tin kỹ năng
        for (int j = 0; j < NUM_SKILLS; ++j) {
            if (scanf("%d", &skills[n * NUM_SKILLS + j]) != 1) {
                flag = 0;
                break;
            }
        }

        if (!flag) {
            break;
        }

        n++;  // Tăng n chỉ khi đã đọc đầy đủ thông tin cho một người chơi
    }
    
    // Khai báo và cấp phát mảng sorted
    int *sorted = (int *)malloc(n * sizeof(int)); 

    // Kiểm tra việc cấp phát bộ nhớ có thành công hay không
    if (sorted == NULL) {
        // Xử lý lỗi ở đây
        exit(1);
    }
    const char *skill_names[] = {"SKILL_BREAKDANCING", "SKILL_APICULTURE", "SKILL_BASKET", "SKILL_XBASKET", "SKILL_SWORD", "TOTAL_XP"};

    for (int skill = 0; skill <= NUM_SKILLS; ++skill) {
        printf("%s\n", skill_names[skill]);

        // Prepare data for sorting
        for (int i = 0; i < n; ++i) {
            if (skill < NUM_SKILLS) {
                sorted[i] = skills[i * NUM_SKILLS + skill];
            } else {
                sorted[i] = 0;
                for (int j = 0; j < NUM_SKILLS; ++j) {
                    sorted[i] += skills[i * NUM_SKILLS + j];
                }
            }
        }

        // Sort and print based on command-line argument
        struct timespec start, end;
        long time_in_micros;
        if (strcmp(argv[1], "standard") == 0) {
            clock_gettime(CLOCK_MONOTONIC, &start);
            qsort(sorted, n, sizeof(int), compare);
            clock_gettime(CLOCK_MONOTONIC, &end);

            time_in_micros = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;


            // Print sorted values
            for (int i = 0; i < n; ++i) {
                printf("%d\n", sorted[i]);
            }
            printf("\n");
            printf("time taken: %ld microseconds\n", time_in_micros);
            printf("\n");

        } else if (strcmp(argv[1], "custom") == 0) {
            clock_gettime(CLOCK_MONOTONIC, &start);
            myradixsort(sorted, n);
            clock_gettime(CLOCK_MONOTONIC, &end);

            time_in_micros = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;


            // Print sorted values
            for (int i = 0; i < n; ++i) {
                printf("%d\n", sorted[i]);
            }
            printf("\n");
            printf("time taken: %ld microseconds\n", time_in_micros);
            printf("\n");

        }
    }


    // Giải phóng bộ nhớ

    free(sorted);
    free(skills);

    return 0;
}
