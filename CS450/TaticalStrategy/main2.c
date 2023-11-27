#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>



#define DIAGONAL_COST_MULTIPLIER 1.5


typedef struct Node {
    int x, y; // Vị trí của nút
    double g; // Chi phí từ nút bắt đầu
    double h; // Heuristic (ước lượng chi phí đến đích)
    double f; // Tổng chi phí (g + h)
    struct Node* parent; // Nút cha để truy ngược đường đi
} Node;

typedef struct {
    Node** nodes; // Mảng các nút
    int size;     // Số lượng phần tử hiện tại
    int capacity; // Dung lượng tối đa
} PriorityQueue;


// Khai báo các hàm ở đây
Node* createNode(int x, int y, Node* parent, double g, double h);
void freeNode(Node* node);
Node* popNode(PriorityQueue* pq);
PriorityQueue* createPriorityQueue();
void freePriorityQueue(PriorityQueue* pq);
bool isQueueEmpty(PriorityQueue* pq);
void resizeQueueIfNeeded(PriorityQueue* pq);
void insertNode(PriorityQueue* pq, Node* node);
double manhattanDistance(int x1, int y1, int x2, int y2);
double getMovementCost(char terrain);
double calculateCost(Node* current, Node* neighbor, char** map);
void findPath(char** map, int rows, int cols, Node* start, Node* end);
char** readMapFromStdin(int* rows, int* cols);
void freeMap(char** map, int rows);


Node* createNode(int x, int y, Node* parent, double g, double h) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return NULL;

    newNode->x = x;
    newNode->y = y;
    newNode->g = g;
    newNode->h = h;
    newNode->f = g + h;
    newNode->parent = parent;
    return newNode;
}

Node* popNode(PriorityQueue* pq) {
    if (isQueueEmpty(pq)) {
        return NULL;
    }

    // Lấy nút đầu tiên (có giá trị f thấp nhất)
    Node* topNode = pq->nodes[0];

    // Di chuyển nút cuối cùng lên vị trí đầu tiên
    pq->nodes[0] = pq->nodes[pq->size - 1];
    pq->size--;

    // Bubble down
    int currentIndex = 0;
    while (true) {
        int childIndex = 2 * currentIndex + 1;
        if (childIndex >= pq->size) {
            break; // Đã đến nút lá
        }

        // Tìm nút con có giá trị f thấp nhất
        if (childIndex + 1 < pq->size && pq->nodes[childIndex + 1]->f < pq->nodes[childIndex]->f) {
            childIndex++;
        }

        // Nếu nút hiện tại đã nhỏ hơn nút con, dừng lại
        if (pq->nodes[currentIndex]->f <= pq->nodes[childIndex]->f) {
            break;
        }

        // Đổi chỗ nút hiện tại với con nó
        Node* temp = pq->nodes[currentIndex];
        pq->nodes[currentIndex] = pq->nodes[childIndex];
        pq->nodes[childIndex] = temp;

        // Cập nhật chỉ số cho lần lặp tiếp theo
        currentIndex = childIndex;
    }

    return topNode;
}


PriorityQueue* createPriorityQueue() {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    if (!pq) return NULL;

    pq->size = 0;
    pq->capacity = 10;
    pq->nodes = (Node**)malloc(sizeof(Node*) * pq->capacity);
    if (!pq->nodes) {
        free(pq);
        return NULL;
    }

    return pq;
}

void freePriorityQueue(PriorityQueue* pq) {
    for (int i = 0; i < pq->size; i++) {
        freeNode(pq->nodes[i]);
    }
    free(pq->nodes);
    free(pq);
}



bool isQueueEmpty(PriorityQueue* pq) {
    return pq->size == 0;
}

void freeNode(Node* node) {
    free(node);
}

double manhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

double getMovementCost(char terrain) {
    switch (terrain) {
        case '.': return 1.0;  // Road
        case ',': return 2.0;  // Grass
        case 'o': return 3.0;  // Rocks
        case '=': return 50.0; // Walls
        default:  return 0.0;  // Other characters
    }
}

double calculateCost(Node* current, Node* neighbor, char** map) {
    char terrain = map[neighbor->x][neighbor->y];
    double baseCost = getMovementCost(terrain);

    // Tránh di chuyển chéo qua các ô chứa nhân vật
    if (current->x != neighbor->x && current->y != neighbor->y) {
        if (terrain == '1' || terrain == '3' || map[current->x][neighbor->y] == '=' || map[neighbor->x][current->y] == '=') {
            return DBL_MAX;
        }
        return baseCost * DIAGONAL_COST_MULTIPLIER;
    }
    return terrain == '1' || terrain == '3' ? DBL_MAX : baseCost;
}



void resizeQueueIfNeeded(PriorityQueue* pq) {
    if (pq->size == pq->capacity) {
        pq->capacity *= 2;
        pq->nodes = (Node**)realloc(pq->nodes, sizeof(Node*) * pq->capacity);
    }
}

void insertNode(PriorityQueue* pq, Node* node) {
    resizeQueueIfNeeded(pq);

    // Thêm nút mới vào cuối hàng đợi
    pq->nodes[pq->size] = node;
    pq->size++;

    // Bubble up
    int currentIndex = pq->size - 1;
    while (currentIndex > 0) {
        int parentIndex = (currentIndex - 1) / 2;
        if (pq->nodes[currentIndex]->f >= pq->nodes[parentIndex]->f) {
            break;
        }

        // Đổi chỗ nút hiện tại với cha nó
        Node* temp = pq->nodes[currentIndex];
        pq->nodes[currentIndex] = pq->nodes[parentIndex];
        pq->nodes[parentIndex] = temp;

        // Cập nhật chỉ số cho lần lặp tiếp theo
        currentIndex = parentIndex;
    }
}


void findPath(char** map, int rows, int cols, Node* start, Node* end) {
    PriorityQueue* openList = createPriorityQueue();
    bool** closedList = (bool**)malloc(sizeof(bool*) * rows);
    for (int i = 0; i < rows; i++) {
        closedList[i] = (bool*)calloc(cols, sizeof(bool));
    }

    insertNode(openList, start);

    bool pathFound = false;

    while (!isQueueEmpty(openList)) {
        Node* current = popNode(openList);
        int x = current->x;
        int y = current->y;

        if (x == end->x && y == end->y) {
            end->parent = current->parent; // Đảm bảo liên kết đúng đường đi
            pathFound = true;
            break;
        }

        closedList[x][y] = true;

        // Xét các nút láng giềng
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue; // Bỏ qua nút hiện tại

                int newX = x + dx;
                int newY = y + dy;

                // Kiểm tra điều kiện hợp lệ của nút mới
                if (newX < 0 || newX >= rows || newY < 0 || newY >= cols) continue;
                if (map[newX][newY] == '=' || closedList[newX][newY]) continue; // Bỏ qua tường và nút đã xét
                if (map[newX][newY] == '1' || map[newX][newY] == '3') continue;

                double newG = current->g + calculateCost(current, createNode(newX, newY, NULL, 0, 0), map);
                Node* neighbor = createNode(newX, newY, current, newG, manhattanDistance(newX, newY, end->x, end->y));

                // Thêm nút láng giềng vào danh sách mở nếu chưa có
                if (!closedList[newX][newY]) {
                    insertNode(openList, neighbor);
                }
            }
        }
    }
    if (pathFound) {
        // Đánh dấu đường đi trên bản đồ
        Node* pathNode = end;
        double totalCost = 0;
        while (pathNode != start) {
            map[pathNode->x][pathNode->y] = '*';
            totalCost += getMovementCost(map[pathNode->x][pathNode->y]);
            pathNode = pathNode->parent;
        }
        map[start->x][start->y] = '0'; // Đánh dấu nút bắt đầu

        // In bản đồ kết quả ra màn hình
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                printf("%c ", map[i][j]);
            }
            printf("\n");
        }
        printf("Total cost: %.1f\n", totalCost);
    } else {
        printf("Không tìm thấy đường đi.\n");
    }
    // Dọn dẹp
    freePriorityQueue(openList);
    for (int i = 0; i < rows; i++) {
        free(closedList[i]);
    }
    free(closedList);
}

char **readMapFromStdin(int *rows, int *cols) {
    int rowSize = 1, colSize = 0;  // Khởi tạo colSize là 0
    char **map = malloc(sizeof(char*));
    if (!map) return NULL;
    map[0] = NULL;  // Khởi tạo dòng đầu tiên là NULL
    char ch;
    int currentCol = 0;

    while ((ch = getchar()) != EOF) {
        if (ch != '\n') {
            if (ch != ' ') {  // Bỏ qua khoảng trắng
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
            }
        } else {
            if (currentCol == 0) {  // Bỏ qua dòng trống
                continue;
            }
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
    *cols = colSize; // Cập nhật kích thước cột
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

    // Tìm vị trí của nhân vật điều khiển và mục tiêu
    Node *start, *end;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (map[i][j] == '0') {
                start = createNode(i, j, NULL, 0, 0);
            } else if (map[i][j] == '2') {
                end = createNode(i, j, NULL, 0, 0);
            }
        }
    }

    // Gọi hàm findPath
    findPath(map, rows, cols, start, end);

    // Dọn dẹp
    freeMap(map, rows);
    freeNode(start);
    freeNode(end);

    return 0;
}