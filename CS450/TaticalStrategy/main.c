#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>



#define DIAGONAL_COST_MULTIPLIER 1.5


typedef struct Node {
    int x, y; // Vị trí của nút
    double G_Score; // Chi phí từ nút bắt đầu
    double H_Score; // Heuristic (ước lượng chi phí đến đích)
    double F_Score; // Tổng chi phí (G_Score + H_Score)
    struct Node* parent; // Nút cha để truy ngược đường đi
} Node;

typedef struct {
    Node** nodes; // Mảng các nút
    int Number_of_Elements;     // Số lượng phần tử hiện tại
    int Maximum; // Dung lượng tối đa
} PriorityQueue;


// Khai báo các hàm ở đây
Node* Create_New_Node(int x, int y, Node* parent, double G_Score, double H_Score);
void freeNode(Node* node);
Node* popNode(PriorityQueue* pq);
PriorityQueue* createPriorityQueue();
void freePriorityQueue(PriorityQueue* pq);
bool isQueueEmpty(PriorityQueue* pq);
void resizeQueueIfNeeded(PriorityQueue* pq);
void Insert_Node(PriorityQueue* pq, Node* node);
double Manhattan_Distance(int x1, int y1, int x2, int y2);
double Movement_Cost(char terrain, bool Check_Diagonal);
double Calculate_Cost_of_Path(Node* current, Node* Neightbor_of_Node, char** map);
void Find_The_best_Path(char** map, int rows, int cols, Node* start, Node* end);
char** Read_Map_From_Input(int* rows, int* cols);
void freeMap(char** map, int rows);
void Update_Node_in_Open_List(PriorityQueue* Open_List, Node* Neightbor_of_Node);




// Hàm kiểm tra và cập nhật nút trong open list
void Update_Node_in_Open_List(PriorityQueue* Open_List, Node* Neightbor_of_Node) {
    for (int i = 0; i < Open_List->Number_of_Elements; i++) {
        if (Open_List->nodes[i]->x == Neightbor_of_Node->x && Open_List->nodes[i]->y == Neightbor_of_Node->y) {
            if (Open_List->nodes[i]->G_Score > Neightbor_of_Node->G_Score) {
                Open_List->nodes[i]->G_Score = Neightbor_of_Node->G_Score;
                Open_List->nodes[i]->F_Score = Neightbor_of_Node->F_Score;
                Open_List->nodes[i]->parent = Neightbor_of_Node->parent;
            }
            return;
        }
    }
    Insert_Node(Open_List, Neightbor_of_Node);
}

Node* Create_New_Node(int x, int y, Node* parent, double G_Score, double H_Score) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return NULL;

    newNode->x = x;
    newNode->y = y;
    newNode->G_Score = G_Score;
    newNode->H_Score = H_Score;
    newNode->F_Score = G_Score + H_Score;
    newNode->parent = parent;
    return newNode;
}

Node* popNode(PriorityQueue* pq) {
    if (isQueueEmpty(pq)) {
        return NULL;
    }

    // Lấy nút đầu tiên (có giá trị F_Score thấp nhất)
    Node* topNode = pq->nodes[0];

    // Di chuyển nút cuối cùng lên vị trí đầu tiên
    pq->nodes[0] = pq->nodes[pq->Number_of_Elements - 1];
    pq->Number_of_Elements--;

    // Bubble down
    int currentIndex = 0;
    while (true) {
        int childIndex = 2 * currentIndex + 1;
        if (childIndex >= pq->Number_of_Elements) {
            break; // Đã đến nút lá
        }

        // Tìm nút con có giá trị F_Score thấp nhất
        if (childIndex + 1 < pq->Number_of_Elements && pq->nodes[childIndex + 1]->F_Score < pq->nodes[childIndex]->F_Score) {
            childIndex++;
        }

        // Nếu nút hiện tại đã nhỏ hơn nút con, dừng lại
        if (pq->nodes[currentIndex]->F_Score <= pq->nodes[childIndex]->F_Score) {
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

    pq->Number_of_Elements = 0;
    pq->Maximum = 10;
    pq->nodes = (Node**)malloc(sizeof(Node*) * pq->Maximum);
    if (!pq->nodes) {
        free(pq);
        return NULL;
    }

    return pq;
}

void freePriorityQueue(PriorityQueue* pq) {
    for (int i = 0; i < pq->Number_of_Elements; i++) {
        freeNode(pq->nodes[i]);
    }
    free(pq->nodes);
    free(pq);
}



bool isQueueEmpty(PriorityQueue* pq) {
    return pq->Number_of_Elements == 0;
}

void freeNode(Node* node) {
    free(node);
}

double Manhattan_Distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}




void resizeQueueIfNeeded(PriorityQueue* pq) {
    if (pq->Number_of_Elements == pq->Maximum) {
        pq->Maximum *= 2;
        pq->nodes = (Node**)realloc(pq->nodes, sizeof(Node*) * pq->Maximum);
    }
}

void Insert_Node(PriorityQueue* pq, Node* node) {
    resizeQueueIfNeeded(pq);

    // Thêm nút mới vào cuối hàng đợi
    pq->nodes[pq->Number_of_Elements] = node;
    pq->Number_of_Elements++;

    // Bubble up
    int currentIndex = pq->Number_of_Elements - 1;
    while (currentIndex > 0) {
        int parentIndex = (currentIndex - 1) / 2;
        if (pq->nodes[currentIndex]->F_Score >= pq->nodes[parentIndex]->F_Score) {
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

double Movement_Cost(char terrain, bool Check_Diagonal) {
    double cost = 0.0;

    // Xác định chi phí dựa trên loại địa hình
    switch (terrain) {
        case '.':
            cost = 1.0; // Road
            break;
        case ',':
            cost = 2.0; // Grass
            break;
        case 'o':
            cost = 3.0; // Rocks
            break;
        case '=':
            cost = 50.0; // Walls
            break;
        default:
            cost = 0.0; // Character or invalid terrain
    }

    // Nếu di chuyển là chéo, chi phí tăng lên 1.5 lần
    if (Check_Diagonal) {
        cost *= 1.5;
    }

    return cost;
}


double Calculate_Cost_of_Path(Node* current, Node* Neightbor_of_Node, char** map) {
    char terrain = map[Neightbor_of_Node->x][Neightbor_of_Node->y];
    bool Check_Diagonal = (current->x != Neightbor_of_Node->x && current->y != Neightbor_of_Node->y);
    double Base_cost = Movement_Cost(terrain, Check_Diagonal);

    // Tránh di chuyển chéo qua các ô chứa nhân vật
    if (Check_Diagonal) {
        if (map[current->x][Neightbor_of_Node->y] == '1' || map[current->x][Neightbor_of_Node->y] == '3' ||
            map[Neightbor_of_Node->x][current->y] == '1' || map[Neightbor_of_Node->x][current->y] == '3') {
            return DBL_MAX;
        }
    }

    if (terrain == '1' || terrain == '3' || terrain == '=') {
        return DBL_MAX;
    }

    return Base_cost;
}





void Find_The_best_Path(char** map, int rows, int cols, Node* start, Node* end) {
    PriorityQueue* Open_List = createPriorityQueue();
    bool** Close_List = (bool**)malloc(sizeof(bool*) * rows);
    for (int i = 0; i < rows; i++) {
        Close_List[i] = (bool*)calloc(cols, sizeof(bool));
    }
    Insert_Node(Open_List, start);
    bool Path_Found = false;

    while (!isQueueEmpty(Open_List)) {
        Node* current = popNode(Open_List);
        int x = current->x;
        int y = current->y;

        if (x == end->x && y == end->y) {
            end->parent = current->parent; // Đảm bảo liên kết đúng đường đi
            Path_Found = true;
            break;
        }

         Close_List[x][y] = true;

        // Xét các nút láng giềng
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue; // Bỏ qua nút hiện tại

                int newX = x + dx;
                int newY = y + dy;

                // Kiểm tra điều kiện hợp lệ của nút mới...
                if (newX < 0 || newX >= rows || newY < 0 || newY >= cols) continue;
                if (map[newX][newY] == '=' || Close_List[newX][newY]) continue; // Bỏ qua tường và nút đã xét
                if (map[newX][newY] == '1' || map[newX][newY] == '3') continue;

                bool Check_Diagonal = (dx != 0 && dy != 0);
                double newG = current->G_Score + Calculate_Cost_of_Path(current, Create_New_Node(newX, newY, NULL, 0, 0), map);
                Node* Neightbor_of_Node = Create_New_Node(newX, newY, current, newG, Manhattan_Distance(newX, newY, end->x, end->y));

                // Thêm nút láng giềng vào danh sách mở nếu chưa có
                if (!Close_List[newX][newY]) {
                    Update_Node_in_Open_List (Open_List, Neightbor_of_Node);
                }
            }
        }
    }
    if (Path_Found) {
        // Đánh dấu đường đi trên bản đồ
        Node* pathNode = end->parent; // Bắt đầu từ nút cha của điểm cuối để bỏ qua điểm cuối
        double totalCost = 0;
        Node* prevNode = pathNode;
        while (pathNode != start) {
            bool Check_Diagonal = (pathNode->x != prevNode->x && pathNode->y != prevNode->y);
            totalCost += Movement_Cost(map[pathNode->x][pathNode->y], Check_Diagonal);
            map[pathNode->x][pathNode->y] = '*'; // Đánh dấu '*' trên đường đi
            prevNode = pathNode;
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
    }
        // Dọn dẹp
    freePriorityQueue(Open_List);
    for (int i = 0; i < rows; i++) {
        free(Close_List[i]);
    }
    free(Close_List);
}

char **Read_Map_From_Input(int *rows, int *cols) {
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
    char **map = Read_Map_From_Input(&rows, &cols);

    // Tìm vị trí của nhân vật điều khiển và mục tiêu
    Node *start, *end;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (map[i][j] == '0') {
                start = Create_New_Node(i, j, NULL, 0, 0);
            } else if (map[i][j] == '2') {
                end = Create_New_Node(i, j, NULL, 0, 0);
            }
        }
    }

    // Gọi hàm Find_The_best_Path
    Find_The_best_Path(map, rows, cols, start, end);

    // Dọn dẹp
    freeMap(map, rows);
    freeNode(start);
    freeNode(end);

    return 0;
}