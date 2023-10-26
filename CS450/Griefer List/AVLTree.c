#include "main.h"

// Calculates the height of a given AVL node.
int Height(AVLNode* node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

// Calculates the balance factor of a given AVL node.
int BalanceFactor(AVLNode* node) {
    if (node == NULL) {
        return 0;
    }
    return Height(node->right) - Height(node->left);
}

// Performs a right rotation on the subtree rooted at y.
AVLNode* RightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T3 = x->right;

    x->right = y;
    y->left = T3;

    if (Height(y->left) > Height(y->right)) {
        y->height = 1 + Height(y->left);
    } else {
        y->height = 1 + Height(y->right);
    }

    if (Height(x->left) > Height(x->right)) {
        x->height = 1 + Height(x->left);
    } else {
        x->height = 1 + Height(x->right);
    }

    return x;
}

// Performs a left rotation on the subtree rooted at x.
AVLNode* LeftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    if (Height(x->left) > Height(x->right)) {
        x->height = 1 + Height(x->left);
    } else {
        x->height = 1 + Height(x->right);
    }

    if (Height(y->left) > Height(y->right)) {
        y->height = 1 + Height(y->left);
    } else {
        y->height = 1 + Height(y->right);
    }

    return y;
}

// Inserts a new node into the AVL tree and rebalances it.
AVLNode* InsertAVL(AVLNode* node, char* username, int ID_of_Server, long UNIX_TIME_OF_BAN) {
    if (node == NULL) {
        AVLNode* newNode = (AVLNode*)malloc(sizeof(AVLNode));
        strncpy(newNode->username, username, 16);
        newNode->information = (Information_of_Player*)malloc(sizeof(Information_of_Player));
        newNode->information->ID_of_Server = ID_of_Server;
        newNode->information->UNIX_TIME_OF_BAN = UNIX_TIME_OF_BAN;
        newNode->information->next = NULL;
        newNode->height = 1;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    if (strcmp(username, node->username) < 0) {
        node->left = InsertAVL(node->left, username, ID_of_Server, UNIX_TIME_OF_BAN);
    } else if (strcmp(username, node->username) > 0) {
        node->right = InsertAVL(node->right, username, ID_of_Server, UNIX_TIME_OF_BAN);
    } else {
        Information_of_Player* newInfo = (Information_of_Player*)malloc(sizeof(Information_of_Player));
        newInfo->ID_of_Server = ID_of_Server;
        newInfo->UNIX_TIME_OF_BAN = UNIX_TIME_OF_BAN;
        newInfo->next = node->information;
        node->information = newInfo;
        return node;
    }

    //Uodate the height of tree after inserting
    if (Height(node->left) > Height(node->right)) {
        node->height = 1 + Height(node->left);
    } else {
        node->height = 1 + Height(node->right);
    }


    //Checking if the node causes imbalance for tree
    int balance = BalanceFactor(node);
    if (balance > 1 && strcmp(username, node->right->username) > 0) {
        return LeftRotate(node);
    }
    if (balance < -1 && strcmp(username, node->left->username) < 0) {
        return RightRotate(node);
    }
    if (balance > 1 && strcmp(username, node->right->username) < 0) {
        node->right = RightRotate(node->right);
        return LeftRotate(node);
    }
    if (balance < -1 && strcmp(username, node->left->username) > 0) {
        node->left = LeftRotate(node->left);
        return RightRotate(node);
    }
    return node;
}

// Searches for a player in the AVL tree.
AVLNode* Search_Player_AVL(AVLNode* root, char* username) {
    while (root != NULL) {
        int cmp = strcmp(username, root->username);
        if (cmp == 0) {
            return root;
        }
        if (cmp < 0) {
            root = root->left;
        } 
        else {
            root = root->right;
        }
    }
    return NULL;
}

// Reads player information from a file and constructs an AVL tree.
AVLNode* Call_AVL(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Could not open file: %s\n", file_name);
        return NULL;
    }

    AVLNode* root = NULL;
    char username[16];
    int ID_of_Server;
    long UNIX_TIME_OF_BAN;

    while (fscanf(file, "%s %d %ld\n", username, &ID_of_Server, &UNIX_TIME_OF_BAN) != EOF) {
        root = InsertAVL(root, username, ID_of_Server, UNIX_TIME_OF_BAN);
    }

    fclose(file);
    return root;
}