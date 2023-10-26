
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define ALPHA 0.57


typedef struct Information_of_Player {
    int ID_of_Server;
    long UNIX_TIME_OF_BAN;
    struct Information_of_Player* next;
} Information_of_Player;


typedef struct ScapeGoatNode {
    char username[50];
    Information_of_Player* information;
    int height;
    int subtreeSize;
    struct ScapeGoatNode* parent;
    struct ScapeGoatNode* left;
    struct ScapeGoatNode* right;
} ScapeGoatNode;


typedef struct AVLNode {
    char username[50];           
    Information_of_Player* information;           
    int height;                 
    struct AVLNode* left;   
    struct AVLNode* right;  
} AVLNode;

#endif // MAIN_H


Information_of_Player* Create_NewNode_NewInfo(int ID_of_Server, long UNIX_TIME_OF_BAN);
ScapeGoatNode* Create_NewNode_For_NewPlayer(char* username, int ID_of_Server, long UNIX_TIME_OF_BAN);
void Add_Info_Of_User(ScapeGoatNode* node, int ID_of_Server, long UNIX_TIME_OF_BAN);
int Size_of_Tree(ScapeGoatNode* root);
int InsertKey(ScapeGoatNode** root, ScapeGoatNode** newNode, char* username, int ID_of_Server, long UNIX_TIME_OF_BAN, ScapeGoatNode* parent);
ScapeGoatNode* FindScapegoat(ScapeGoatNode* n, float alpha);
void Flatten_Tree(ScapeGoatNode* root, ScapeGoatNode** head);
void Update_Height_Of_Tree(ScapeGoatNode* node);
ScapeGoatNode* Build_Height_Balanced_Tree(int size, ScapeGoatNode** head);
void Rebuild_Tree(int size, ScapeGoatNode** root, ScapeGoatNode* scapegoat);
ScapeGoatNode* Search_Player_ScapeGoat(ScapeGoatNode* root, char* username);
bool If_idServer_ExistOrNot(int* arr, int n, int target);
ScapeGoatNode* Call_ScapeGoat(const char *file_name);

int Height(AVLNode* node);
int BalanceFactor(AVLNode* node);
AVLNode* RightRotate(AVLNode* y);
AVLNode* LeftRotate(AVLNode* x);
AVLNode* InsertAVL(AVLNode* node, char* username, int ID_of_Server, long UNIX_TIME_OF_BAN);
AVLNode* Search_Player_AVL(AVLNode* root, char* username);
AVLNode* Call_AVL(const char *file_name);


