// main.h

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define ALPHA 0.57

// Cấu trúc cho thông tin cấm người chơi
typedef struct Info {
    int server_id;
    long unix_time_of_ban;
    struct Info* next;
} Info;

// Cấu trúc cho PlayerNode
typedef struct PlayerNode {
    char username[50];
    Info* info;
    int height;
    int subtreeSize;
    struct PlayerNode* parent;
    struct PlayerNode* left;
    struct PlayerNode* right;
} PlayerNode;

// Cấu trúc cho AVLNode
typedef struct AVLNode {
    char username[50];           // Tên người chơi
    Info* info;           // Danh sách các lần người chơi bị cấm
    int height;                  // Chiều cao của node (dùng để kiểm tra và duy trì tính cân bằng)
    struct AVLNode* left;   // Node con bên trái
    struct AVLNode* right;  // Node con bên phải
} AVLNode;

#endif // MAIN_H


Info* createInfo(int server_id, long unix_time_of_ban);
PlayerNode* createPlayerNode(char* username, int server_id, long unix_time_of_ban);
void addInfo(PlayerNode* node, int server_id, long unix_time_of_ban);
int treeSize(PlayerNode* root);
int InsertKey(PlayerNode** root, PlayerNode** newNode, char* username, int server_id, long unix_time_of_ban, PlayerNode* parent);
PlayerNode* FindScapegoat(PlayerNode* n, float alpha);
void Flatten_Tree(PlayerNode* root, PlayerNode** head);
void UpdateHeight(PlayerNode* node);
PlayerNode* Build_Height_Balanced_Tree(int size, PlayerNode** head);
void RebuildTree(int size, PlayerNode** root, PlayerNode* scapegoat);
PlayerNode* SearchPlayer(PlayerNode* root, char* username);
bool idExists(int* arr, int n, int target);
PlayerNode* run_scapegoat(const char *file_name);

int Height(AVLNode* node);
int BalanceFactor(AVLNode* node);
AVLNode* RightRotate(AVLNode* y);
AVLNode* LeftRotate(AVLNode* x);
AVLNode* InsertAVL(AVLNode* node, char* username, int server_id, long unix_time_of_ban);
AVLNode* SearchAVL(AVLNode* root, char* username);
AVLNode* run_avl(const char *file_name);


