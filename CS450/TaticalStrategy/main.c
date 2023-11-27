#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>
//#include <heap.h>


#define DIAGONAL_COST_MULTIPLIER 1.5


typedef struct Node {
    int x, y; // Vị trí của nút
    double g; // Chi phí từ nút bắt đầu
    double h; // Heuristic (ước lượng chi phí đến đích)
    double f; // Tổng chi phí (g + h)
    struct Node* parent; // Nút cha để truy ngược đường đi
} Node;

typedef struct {
    Node** nodes;
    int size;
} MinHeap;

// Khai báo các hàm ở đây
Node* createNode(int x, int y, Node* parent, double g, double h);
void freeNode(Node* node);
double manhattanDistance(int x1, int y1, int x2, int y2);
double getMovementCost(char terrain);
double calculateCost(int currentX, int currentY, int newX, int newY, const char** map);
void findPath(char** map, int rows, int cols, Node* start, Node* end);
char** readMapFromStdin(int* rows, int* cols);
void freeMap(char** map, int rows);
MinHeap* createMinHeap(int capacity);
void swapNodes(Node** a, Node** b);
Node* extractMin(MinHeap* heap);
void insertHeap(MinHeap* heap, Node* node);
void freeMinHeap(MinHeap* heap);


MinHeap* createMinHeap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->nodes = (Node**)malloc(sizeof(Node*) * capacity);
    heap->size = 0;
    return heap;
}

void swapNodes(Node** a, Node** b) {
    Node* temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->nodes[left]->f < heap->nodes[smallest]->f)
        smallest = left;
    if (right < heap->size && heap->nodes[right]->f < heap->nodes[smallest]->f)
        smallest = right;
    if (smallest != idx) {
        swapNodes(&(heap->nodes[smallest]), &(heap->nodes[idx]));
        minHeapify(heap, smallest);
    }
}

Node* extractMin(MinHeap* heap) {
    if (heap->size == 0) return NULL;
    Node* root = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    minHeapify(heap, 0);
    return root;
}

void insertHeap(MinHeap* heap, Node* node) {
    heap->size++;
    int i = heap->size - 1;
    heap->nodes[i] = node;

    while (i != 0 && heap->nodes[(i - 1) / 2]->f > heap->nodes[i]->f) {
        swapNodes(&(heap->nodes[i]), &(heap->nodes[(i - 1) / 2]));
        i = (i - 1) / 2;
    }
}

// Hãy nhớ giải phóng bộ nhớ sau khi sử dụng
void freeMinHeap(MinHeap* heap) {
    free(heap->nodes);
    free(heap);
}

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

double calculateCost(int currentX, int currentY, int newX, int newY, const char** map) {
    char terrain = map[newX][newY];
    double baseCost = getMovementCost(terrain);

    if (currentX != newX && currentY != newY) {
        // Kiểm tra di chuyển chéo qua các nhân vật khác hoặc tường
        if (terrain == '1' || terrain == '3' || map[currentX][newY] == '=' || map[newX][currentY] == '=') {
            return DBL_MAX;
        }
        return baseCost * DIAGONAL_COST_MULTIPLIER;
    }
    return terrain == '1' || terrain == '3' ? DBL_MAX : baseCost;
}




void findPath(char** map, int rows, int cols, Node* start, Node* end) {
    
    double** gScores = (double**)malloc(sizeof(double*) * rows);
    for (int i = 0; i < rows; i++) {
        gScores[i] = (double*)malloc(sizeof(double) * cols);
        for (int j = 0; j < cols; j++) {
            gScores[i][j] = DBL_MAX;
        }
    }
    gScores[start->x][start->y] = 0;  // Chi phí di chuyển đến nút bắt đầu là 0
    MinHeap* openList = createMinHeap(10); // Ví dụ: sức chứa ban đầu là 10
    bool** closedList = (bool**)malloc(sizeof(bool*) * rows);
    for (int i = 0; i < rows; i++) {
        closedList[i] = (bool*)calloc(cols, sizeof(bool));
    }

    bool pathFound = false;

    insertHeap(openList, start); 

    while (openList->size > 0) { 
        Node* current = extractMin(openList);
        int x = current->x;
        int y = current->y;

        if (x == end->x && y == end->y) {
            end->parent = current->parent; 
            pathFound = true;
            break;
        }

        closedList[x][y] = true;

        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int newX = x + dx;
                int newY = y + dy;

                if (newX >= 0 && newX < rows && newY >= 0 && newY < cols && 
                    map[newX][newY] != '=' && map[newX][newY] != '1' && map[newX][newY] != '3' &&
                    !closedList[newX][newY]) {

                    double newG = current->g + calculateCost(x, y, newX, newY, map);
                    if (newG < gScores[newX][newY]) {
                        gScores[newX][newY] = newG; // Cập nhật gScore mới nếu thấp hơn
                        Node* neighbor = createNode(newX, newY, current, newG, manhattanDistance(newX, newY, end->x, end->y));
                        insertHeap(openList, neighbor);
                    }
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
                printf("%c", map[i][j]);
            }
            printf("\n");
        }
        printf("Total cost: %.1f\n", totalCost);
    } else {
        printf("Không tìm thấy đường đi.\n");
    }
    // Dọn dẹp
    for (int i = 0; i < rows; i++) {
        free(closedList[i]);
    }
    for (int i = 0; i < rows; i++) {
        free(gScores[i]);
    }
    free(gScores);
    free(closedList);
    freeMinHeap(openList); // Giải phóng MinHeap
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

