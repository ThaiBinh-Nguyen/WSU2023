#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_SKILLS 5

int compare(const void *a, const void *b) {
    return *((int*)b) - *((int*)a);
}

// int partition(int arr[], int low, int high) {
//     int pivot = arr[high];
//     int i = (low - 1);
//     for (int j = low; j <= high - 1; j++) {
//         // Thay đổi điều kiện so sánh ở đây
//         if (arr[j] >= pivot) {
//             i++;
//             int temp = arr[i];
//             arr[i] = arr[j];
//             arr[j] = temp;
//         }
//     }
//     int temp = arr[i + 1];
//     arr[i + 1] = arr[high];
//     arr[high] = temp;
//     return (i + 1);
// }




// void quickSort(int arr[], int low, int high) {
//     if (low < high) {
//         int pi = partition(arr, low, high);
//         quickSort(arr, low, pi - 1);
//         quickSort(arr, pi + 1, high);
//     }
// }

void countingSort(int array[], int size, int place) {
  int output[size + 1];
  int max = array[0];
  for (int i = 1; i < size; i++) {
    if (array[i] > max)
      max = array[i];
  }
  int count[max + 1];

  for (int i = 0; i <= max; ++i) {
    count[i] = 0;
  }
  for (int i = 0; i < size; i++) {
    count[(array[i] / place) % 10]++;
  }
  for (int i = 1; i <= max; i++) {
    count[i] += count[i - 1];
  }
  for (int i = size - 1; i >= 0; i--) {
    output[count[(array[i] / place) % 10] - 1] = array[i];
    count[(array[i] / place) % 10]--;
  }
  for (int i = 0; i < size; i++) {
    array[i] = output[i];
  }
}

void radixsort(int array[], int size) {
  int max = array[0];
  for (int i = 1; i < size; i++) {
    if (array[i] > max)
      max = array[i];
  }
  for (int place = 1; max / place > 0; place *= 10) {
    countingSort(array, size, place);
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

        // Kiểm tra việc cấp phát bộ nhớ có thành công hay không
        if (skills == NULL) {
            // Xử lý lỗi ở đây
            exit(1);
        }

        // Đọc thông tin kỹ năng
        for (int j = 0; j < NUM_SKILLS; ++j) {
            if (scanf("%d", &skills[n * NUM_SKILLS + j]) != 1) {
                flag = 0;
                //printf("Break, không đọc thêm được.\n"); // Thêm dòng này
                break;
            }
        }

        // ...

        if (!flag) {
           // printf("Thoát khỏi vòng lặp.\n"); // Thêm dòng này
            break;
        }

        printf("Đọc thông tin cho người chơi số %d.\n", n); // Thêm dòng này
        n++;

    }

    // Khai báo và cấp phát mảng sorted
    int *sorted = (int *)malloc(n * sizeof(int)); 

    // Kiểm tra việc cấp phát bộ nhớ có thành công hay không
    if (sorted == NULL) {
        // Xử lý lỗi ở đây
        exit(1);
    }


    if (strcmp(argv[1], "standard") == 0) {
        const char *skill_names[] = {"SKILL_BREAKDANCING", "SKILL_APICULTURE", "SKILL_BASKET", "SKILL_XBASKET", "SKILL_SWORD", "TOTAL_XP"};

        for (int skill = 0; skill <= NUM_SKILLS; ++skill) {
            clock_gettime(CLOCK_MONOTONIC, &start);
            printf("%s\n", skill_names[skill]);
            
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

            qsort(sorted, n, sizeof(int), compare);
            clock_gettime(CLOCK_MONOTONIC, &end);

            long time_in_micros = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
            for (int i = 0; i < n; ++i) {
                printf("%d\n", sorted[i]);
            }
            printf("time taken: %ld\n", time_in_micros);

            printf("\n");
        }
    } else {
        const char *skill_names[] = {"SKILL_BREAKDANCING", "SKILL_APICULTURE", "SKILL_BASKET", "SKILL_XBASKET", "SKILL_SWORD", "TOTAL_XP"};

        for (int skill = 0; skill <= NUM_SKILLS; ++skill) {
            clock_gettime(CLOCK_MONOTONIC, &start);
            printf("%s\n", skill_names[skill]);
            
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

            //quickSort(sorted, 0, n-1);
            radixsort(sorted, n);
            clock_gettime(CLOCK_MONOTONIC, &end);

            long time_in_micros = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
            for (int i = 0; i < n; ++i) {
                printf("%d\n", sorted[i]);
            }
            printf("time taken: %ld\n", time_in_micros);

            printf("\n");
        }
    }

    // Giải phóng bộ nhớ

    free(sorted);
    free(skills);

    return 0;
}
