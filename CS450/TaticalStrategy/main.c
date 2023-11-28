/*
Name: Thai Binh Nguyen
Project 5 - Tactical Strategy
Instructor: Dr. Williams
Date: 11-20-2023

*/

/* 
    I used A* algorithm. But my work does not work correctly. I tried to fix it but I did not figure it out yet. 
    I think my problem may be the A* algorithm.
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>



#define DIAGONAL_COST_MULTIPLIER 1.5


typedef struct Node {
    int x, y; // Node's position
    double G_Score; // Cost from the start node
    double H_Score; // Heuristic (estimated cost to the destination)
    double F_Score; // Total cost (G_Score + H_Score)
    struct Node* parent; // Parent node for backtracking the path
} Node;

typedef struct {
    Node** nodes; // Array of nodes
    int Number_of_Elements;     // Current number of elements
    int Maximum; // Maximum capacity
} PriorityQueue;


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




// Function to check and update a node in the open list
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

    // Get the first node (with the lowest F_Score value)
    Node* topNode = pq->nodes[0];

    // Move the last node to the first position
    pq->nodes[0] = pq->nodes[pq->Number_of_Elements - 1];
    pq->Number_of_Elements--;

    // Bubble down
    int currentIndex = 0;
    while (true) {
        int childIndex = 2 * currentIndex + 1;
        if (childIndex >= pq->Number_of_Elements) {
            break;
        }

        // Find the child node with the lowest F_Score value
        if (childIndex + 1 < pq->Number_of_Elements && pq->nodes[childIndex + 1]->F_Score < pq->nodes[childIndex]->F_Score) {
            childIndex++;
        }

        // If the current node is smaller than the child node, stop
        if (pq->nodes[currentIndex]->F_Score <= pq->nodes[childIndex]->F_Score) {
            break;
        }

        // Swap the current node with its child
        Node* temp = pq->nodes[currentIndex];
        pq->nodes[currentIndex] = pq->nodes[childIndex];
        pq->nodes[childIndex] = temp;

        // Update the index for the next iteration
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

    // Add new node to the end of the queue
    pq->nodes[pq->Number_of_Elements] = node;
    pq->Number_of_Elements++;

    // Bubble up
    int currentIndex = pq->Number_of_Elements - 1;
    while (currentIndex > 0) {
        int parentIndex = (currentIndex - 1) / 2;
        if (pq->nodes[currentIndex]->F_Score >= pq->nodes[parentIndex]->F_Score) {
            break;
        }

        // Swap the current node with its parent
        Node* temp = pq->nodes[currentIndex];
        pq->nodes[currentIndex] = pq->nodes[parentIndex];
        pq->nodes[parentIndex] = temp;

        // Update the index for the next iteration
        currentIndex = parentIndex;
    }
}

double Movement_Cost(char terrain, bool Check_Diagonal) {
    double cost = 0.0;

    // Determine costs based on terrain type
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

    // If the move is diagonal, the cost increases by 1.5 times
    if (Check_Diagonal) {
        cost *= 1.5;
    }

    return cost;
}


double Calculate_Cost_of_Path(Node* current, Node* Neightbor_of_Node, char** map) {
    char terrain = map[Neightbor_of_Node->x][Neightbor_of_Node->y];
    bool Check_Diagonal = (current->x != Neightbor_of_Node->x && current->y != Neightbor_of_Node->y);
    double Base_cost = Movement_Cost(terrain, Check_Diagonal);

    // Avoid moving diagonally across cells containing characters
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
            end->parent = current->parent;
            Path_Found = true;
            break;
        }

         Close_List[x][y] = true;

        // Consider neighbor nodes
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;

                int newX = x + dx;
                int newY = y + dy;

                // Check the validity of the new node.
                if (newX < 0 || newX >= rows || newY < 0 || newY >= cols) continue;
                if (map[newX][newY] == '=' || Close_List[newX][newY]) continue;
                if (map[newX][newY] == '1' || map[newX][newY] == '3') continue;

                bool Check_Diagonal = (dx != 0 && dy != 0);
                double newG = current->G_Score + Calculate_Cost_of_Path(current, Create_New_Node(newX, newY, NULL, 0, 0), map);
                Node* Neightbor_of_Node = Create_New_Node(newX, newY, current, newG, Manhattan_Distance(newX, newY, end->x, end->y));

              // Add neighbor node to open list if not already there
                if (!Close_List[newX][newY]) {
                    Update_Node_in_Open_List (Open_List, Neightbor_of_Node);
                }
            }
        }
    }
    if (Path_Found) {
        // Mark the route on the map
        Node* pathNode = end->parent;
        double totalCost = 0;
        Node* prevNode = pathNode;
        while (pathNode != start) {
            bool Check_Diagonal = (pathNode->x != prevNode->x && pathNode->y != prevNode->y);
            totalCost += Movement_Cost(map[pathNode->x][pathNode->y], Check_Diagonal);
            map[pathNode->x][pathNode->y] = '*';
            prevNode = pathNode;
            pathNode = pathNode->parent;
        }
        map[start->x][start->y] = '0';

        // Print the resulting map to the screen
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                printf("%c ", map[i][j]);
            }
            printf("\n");
        }
        printf("Total cost: %.1f\n", totalCost);
    }
    
    freePriorityQueue(Open_List);
    for (int i = 0; i < rows; i++) {
        free(Close_List[i]);
    }
    free(Close_List);
}

char **Read_Map_From_Input(int *rows, int *cols) {
    int rowSize = 1, colSize = 0;
    char **map = malloc(sizeof(char*));
    if (!map) return NULL;
    map[0] = NULL;
    char ch;
    int currentCol = 0;

    while ((ch = getchar()) != EOF) {
        if (ch != '\n') {
            if (ch != ' ') {
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
            if (currentCol == 0) {
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
    *cols = colSize;
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

    // Find the location of the controller and target
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

    Find_The_best_Path(map, rows, cols, start, end);

    freeMap(map, rows);
    freeNode(start);
    freeNode(end);

    return 0;
}