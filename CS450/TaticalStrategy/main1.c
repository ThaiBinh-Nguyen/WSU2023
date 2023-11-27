#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

#define MAX_ROWS 50
#define MAX_COLS 50

bool inOpenSet[MAX_ROWS][MAX_COLS];

typedef struct {
    int row;
    int col;
} Point;

typedef struct {
    Point point;
    int gScore;
    int fScore;
} Node;

typedef struct {
    Node nodes[MAX_ROWS * MAX_COLS];
    int size;
} PriorityQueue;

void initPriorityQueue(PriorityQueue *pq);
void push(PriorityQueue *pq, Node node);
Node pop(PriorityQueue *pq);
bool contains(Point point);
void reconstruct_path(Point cameFrom[MAX_ROWS][MAX_COLS], Point current, char **map);
void A_Star(char **map, Point start, Point goal, int rows, int cols);
char **readMapFromStdin(int *rows, int *cols);
void freeMap(char **map, int rows);
int h(Point p1, Point p2);
int d(char **map, Point p1, Point p2);
void findStartAndGoal(char **map, int rows, int cols, Point *start, Point *goal);
void initInOpenSet(bool inOpenSet[MAX_ROWS][MAX_COLS], int rows, int cols);


// Hàm để khởi tạo trạng thái của openSet
void initInOpenSet(bool inOpenSet[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            inOpenSet[row][col] = false;
        }
    }
}

// Hàm heuristic - Khoảng cách Manhattan
int h(Point p1, Point p2) {
    return abs(p1.row - p2.row) + abs(p1.col - p2.col);
}

// Hàm để tính chi phí di chuyển giữa hai ô
int d(char **map, Point p1, Point p2) {
    char terrain = map[p2.row][p2.col];
    int cost;

    // Kiểm tra nếu là nhân vật, không thể di chuyển qua
    if (terrain == '0' || terrain == '1' || terrain == '2' || terrain == '3') {
        return INT_MAX;
    }

    // Xác định chi phí dựa trên loại địa hình
    switch (terrain) {
        case '.': cost = 1; break;
        case ',': cost = 2; break;
        case 'o': cost = 3; break;
        case '=': cost = 50; // Không thể di chuyển qua tường
        default: cost = 0; // Loại địa hình không xác định
    }

    // Nếu di chuyển chéo, nhân chi phí với 1.5
    if (p1.row != p2.row && p1.col != p2.col) {
        cost *= 1.5;
    }
    return cost;
}
void findStartAndGoal(char **map, int rows, int cols, Point *start, Point *goal) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (map[row][col] == '0') { // Nhân vật điều khiển
                start->row = row;
                start->col = col;
            } else if (map[row][col] == '2') { // Nhân vật mục tiêu
                goal->row = row;
                goal->col = col;
            }
        }
    }
}


// Hàm khởi tạo PriorityQueue
void initPriorityQueue(PriorityQueue *pq) {
    pq->size = 0;
}

// Hàm để thêm một nút vào PriorityQueue, giữ cho nó được sắp xếp theo fScore tăng dần
void push(PriorityQueue *pq, Node node) {
    if (pq->size >= MAX_ROWS * MAX_COLS) {
        // Hàng đợi đã đầy
        return;
    }

    // Thêm nút mới vào cuối hàng đợi
    pq->nodes[pq->size] = node;
    int currentIndex = pq->size;
    pq->size++;

    // Bắt đầu từ nút cuối cùng, đổi chỗ cho đến khi nút cha có fScore nhỏ hơn hoặc bằng fScore của nút hiện tại
    while (currentIndex > 0) {
        int parentIndex = (currentIndex - 1) / 2;
        if (pq->nodes[parentIndex].fScore <= pq->nodes[currentIndex].fScore) {
            // Nếu nút cha đã có fScore nhỏ hơn hoặc bằng, dừng vòng lặp
            break;
        }
        
        // Đổi chỗ nút cha và nút hiện tại
        Node temp = pq->nodes[parentIndex];
        pq->nodes[parentIndex] = pq->nodes[currentIndex];
        pq->nodes[currentIndex] = temp;

        // Cập nhật currentIndex để tiếp tục vòng lặp nếu cần
        currentIndex = parentIndex;
    }
    inOpenSet[node.point.row][node.point.col] = true;
}

// Hàm để lấy điểm có fScore thấp nhất từ PriorityQueue
Node pop(PriorityQueue *pq) {
    if (pq->size == 0) {
        // Trả về một nút với giá trị mặc định nếu hàng đợi trống
        Node emptyNode;
        emptyNode.point.row = -1;
        emptyNode.point.col = -1;
        emptyNode.gScore = INT_MAX;
        emptyNode.fScore = INT_MAX;
        return emptyNode;
    }

    // Lưu giữ nút đầu tiên để trả về
    Node minNode = pq->nodes[0];

    // Di chuyển nút cuối cùng lên đầu và giảm kích thước hàng đợi
    pq->nodes[0] = pq->nodes[pq->size - 1];
    pq->size--;

    // Điều chỉnh lại cấu trúc min-heap
    int currentIndex = 0;
    while (true) {
        int leftChildIndex = 2 * currentIndex + 1;
        int rightChildIndex = 2 * currentIndex + 2;
        int smallestIndex = currentIndex;

        // Tìm nút con với fScore nhỏ nhất
        if (leftChildIndex < pq->size && pq->nodes[leftChildIndex].fScore < pq->nodes[smallestIndex].fScore) {
            smallestIndex = leftChildIndex;
        }
        if (rightChildIndex < pq->size && pq->nodes[rightChildIndex].fScore < pq->nodes[smallestIndex].fScore) {
            smallestIndex = rightChildIndex;
        }

        // Nếu nút hiện tại đã là nút nhỏ nhất, dừng vòng lặp
        if (smallestIndex == currentIndex) {
            break;
        }

        // Đổi chỗ nút hiện tại với nút con nhỏ nhất
        Node temp = pq->nodes[currentIndex];
        pq->nodes[currentIndex] = pq->nodes[smallestIndex];
        pq->nodes[smallestIndex] = temp;

        // Cập nhật currentIndex
        currentIndex = smallestIndex;
    }
    inOpenSet[minNode.point.row][minNode.point.col] = false;
    return minNode;
}

// Hàm kiểm tra xem một điểm có trong openSet hay không
bool contains(Point point) {
    return inOpenSet[point.row][point.col];
}

// Hàm để xây dựng đường dẫn từ điểm bắt đầu đến điểm hiện tại
void reconstruct_path(Point cameFrom[MAX_ROWS][MAX_COLS], Point current, char **map) {
    while (current.row != -1 && current.col != -1) {
        map[current.row][current.col] = '*'; // Đánh dấu đường đi trên bản đồ
        current = cameFrom[current.row][current.col];
    }
}

// Thuật toán A*
void A_Star(char **map, Point start, Point goal, int rows, int cols) {
    PriorityQueue openSet;
    initPriorityQueue(&openSet);
    // Khởi tạo trạng thái của inOpenSet
    initInOpenSet(inOpenSet, rows, cols);
    Point cameFrom[MAX_ROWS][MAX_COLS];
    int gScore[MAX_ROWS][MAX_COLS];
    int fScore[MAX_ROWS][MAX_COLS];

    // Khởi tạo các mảng cameFrom, gScore và fScore
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            cameFrom[row][col].row = -1;
            cameFrom[row][col].col = -1;
            gScore[row][col] = INT_MAX;
            fScore[row][col] = INT_MAX;
        }
    }

    // Thêm điểm bắt đầu vào openSet
    Node startNode = {start, 0, h(start, goal)};
    push(&openSet, startNode);
    gScore[start.row][start.col] = 0;
    fScore[start.row][start.col] = h(start, goal);

    while (openSet.size > 0) {
        Node current = pop(&openSet);
        
        // Nếu đạt được mục tiêu, tái tạo đường đi và kết thúc
        if (current.point.row == goal.row && current.point.col == goal.col) {
            reconstruct_path(cameFrom, current.point, map);
            return;
        }

        // Xử lý các hàng xóm của nút hiện tại
        for (int dRow = -1; dRow <= 1; dRow++) {
            for (int dCol = -1; dCol <= 1; dCol++) {
                if (dRow == 0 && dCol == 0) continue; // Bỏ qua nút hiện tại

                Point neighbor = {current.point.row + dRow, current.point.col + dCol};

                // Kiểm tra nếu hàng xóm nằm ngoài lưới
                if (neighbor.row < 0 || neighbor.row >= rows || neighbor.col < 0 || neighbor.col >= cols) {
                    continue;
                }

                int tentative_gScore = gScore[current.point.row][current.point.col] + d(map, current.point, neighbor);

                if (tentative_gScore < gScore[neighbor.row][neighbor.col]) {
                    // Đường đi tốt hơn được tìm thấy, cập nhật cho hàng xóm
                    cameFrom[neighbor.row][neighbor.col] = current.point;
                    gScore[neighbor.row][neighbor.col] = tentative_gScore;
                    fScore[neighbor.row][neighbor.col] = tentative_gScore + h(neighbor, goal);

                    if (!inOpenSet[neighbor.row][neighbor.col]) { // Sử dụng inOpenSet thay vì contains
                        Node neighborNode = {neighbor, tentative_gScore, fScore[neighbor.row][neighbor.col]};
                        push(&openSet, neighborNode);
                    }
                }
            }
        }
    }
}

char **readMapFromStdin(int *rows, int *cols) {
    int rowSize = 1, colSize = 1;
    char **map = malloc(sizeof(char*));
    if (!map) return NULL;
    map[0] = malloc(sizeof(char));
    if (!map[0]) {
        free(map);
        return NULL;
    }
    char ch;
    int currentCol = 0;

    while ((ch = getchar()) != EOF) {
        if (ch != '\n') {
            if (currentCol >= colSize) {
                colSize++;
                for (int i = 0; i < rowSize; i++) {
                    char *tempRow = realloc(map[i], colSize * sizeof(char));
                    if (!tempRow) {
                        for (int j = 0; j < i; j++) free(map[j]);
                        free(map);
                        return NULL;
                    }
                    map[i] = tempRow;
                }
            }
            map[rowSize - 1][currentCol++] = ch;
        } else {
            char *tempRow = malloc(colSize * sizeof(char));
            if (!tempRow) {
                for (int i = 0; i < rowSize; i++) free(map[i]);
                free(map);
                return NULL;
            }
            rowSize++;
            char **tempMap = realloc(map, rowSize * sizeof(char*));
            if (!tempMap) {
                free(tempRow);
                for (int i = 0; i < rowSize - 1; i++) free(map[i]);
                free(map);
                return NULL;
            }
            map = tempMap;
            map[rowSize - 1] = tempRow;
            currentCol = 0;
        }
    }
    *rows = rowSize;
    *cols = currentCol; // Cập nhật kích thước cột
    return map;
}

void freeMap(char **map, int rows) {
    for (int i = 0; i < rows; i++) {
        free(map[i]);
    }
    free(map);
}

int main() {
    int rows, cols;
    char **map = readMapFromStdin(&rows, &cols);

    Point start, goal;
    findStartAndGoal(map, rows, cols, &start, &goal);

    // Khởi tạo trạng thái của inOpenSet
    initInOpenSet(inOpenSet, rows, cols);

    // Thực hiện thuật toán A*
    A_Star(map, start, goal, rows, cols);

    // Dọn dẹp
    freeMap(map, rows);

    return 0;
}


