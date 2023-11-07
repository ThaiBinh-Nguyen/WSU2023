#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 128
#define MAX_W 1200
#define MAX_ITEMS 128
#define MAX_NAME_LENGTH 128

typedef struct {
    char name[MAX_NAME_LENGTH];
    int weight;
    int value;
} Item;

int max(int a, int b) {
    return (a > b) ? a : b;
}

void knapsack(int n, int W, Item items[]) {
    int m[MAX_N+1][MAX_W+1], selected[MAX_N+1];
    int i, j;

    // Khởi tạo cơ sở của quy hoạch động
    for (j = 0; j <= W; j++) {
        m[0][j] = 0;
    }
    for (i = 1; i <= n; i++) {
        m[i][0] = 0;
    }

    // Xây dựng bảng m theo nguyên tắc quy hoạch động
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= W; j++) {
            if (items[i-1].weight > j) {
                m[i][j] = m[i-1][j];
            } else {
                m[i][j] = max(m[i-1][j], m[i-1][j-items[i-1].weight] + items[i-1].value);
            }
        }
    }
    // Truy ngược để xác định các món đồ được chọn
    i = n; j = W;
    memset(selected, 0, sizeof(selected)); // Khởi tạo mảng selected với giá trị 0
    while (i > 0 && j > 0) {
        if (m[i][j] != m[i-1][j]) {
            selected[i] = 1; // Đánh dấu món đồ này được chọn
            j = j - items[i-1].weight;
        }
        i--;
    }

    // In danh sách các món đồ được chọn
    int totalWeight = 0, totalValue = 0;
    for (i = 1; i <= n; i++) {
        if (selected[i]) {
            printf("%s, %d, %d\n", items[i-1].name, items[i-1].weight, items[i-1].value);
            totalWeight += items[i-1].weight;
            totalValue += items[i-1].value;
        }
    }

    // In tổng trọng lượng và tổng giá trị
    printf("final weight: %d\n", totalWeight);
    printf("final value: %d\n", totalValue);
}

int main() {
    int carryWeight;
    Item items[MAX_ITEMS];
    int itemCount = 0;

    // Đọc trọng lượng tối đa
    scanf("%d\n", &carryWeight);

    // Đọc thông tin từng món đồ
    while (scanf("%[^;];%d;%d\n", items[itemCount].name, &items[itemCount].weight, &items[itemCount].value) != EOF) {
        itemCount++;
        if (itemCount >= MAX_ITEMS) {
            break;
        }
    }

    // Gọi hàm knapsack với dữ liệu đã đọc
    knapsack(itemCount, carryWeight, items);

    return 0;
}
