#include "main.h"

// Creates a new node to store player information.
Information_of_Player* Create_NewNode_NewInfo(int ID_of_Server, long UNIX_TIME_OF_BAN) {
    Information_of_Player* newInfo = (Information_of_Player*) malloc(sizeof(Information_of_Player));
    newInfo->ID_of_Server = ID_of_Server;
    newInfo->UNIX_TIME_OF_BAN = UNIX_TIME_OF_BAN;
    newInfo->next = NULL;
    return newInfo;
}

// Creates a new ScapeGoatNode for a new player.
ScapeGoatNode* Create_NewNode_For_NewPlayer(char* username, int ID_of_Server, long UNIX_TIME_OF_BAN) {
    ScapeGoatNode* newNode = (ScapeGoatNode*) malloc(sizeof(ScapeGoatNode));
    strcpy(newNode->username, username);
    newNode->information = Create_NewNode_NewInfo(ID_of_Server, UNIX_TIME_OF_BAN);
    newNode->height = 1; 
    newNode->subtreeSize = 1; 
    newNode->left = newNode->right = newNode->parent = NULL;
    return newNode;
}

// Adds new ban information to a given ScapeGoatNode.
void Add_Info_Of_User(ScapeGoatNode* node, int ID_of_Server, long UNIX_TIME_OF_BAN) {
    Information_of_Player* newInfo = Create_NewNode_NewInfo(ID_of_Server, UNIX_TIME_OF_BAN);
    newInfo->next = node->information;
    node->information = newInfo;
}

// Calculates the size of the tree rooted at 'root'.
int Size_of_Tree(ScapeGoatNode* root) {
    if (root == NULL) {
        return 0;
    }
    return root->subtreeSize;
}

// Inserts a new player into the ScapeGoatTree and rebalances if necessary.
int InsertKey(ScapeGoatNode** root, ScapeGoatNode** newNode, char* username, int ID_of_Server, long UNIX_TIME_OF_BAN, ScapeGoatNode* parent) {
    if (*root == NULL) {
        *root = Create_NewNode_For_NewPlayer(username, ID_of_Server, UNIX_TIME_OF_BAN);
        (*root)->parent = parent; 
        *newNode = *root; 
        return 1; 
    }
    
    int cmp = strcmp(username, (*root)->username);
    if (cmp == 0) {
        Add_Info_Of_User(*root, ID_of_Server, UNIX_TIME_OF_BAN);
        *newNode = *root; 
        return (*root)->height;
    } else if (cmp < 0) {
        int leftHeight = InsertKey(&(*root)->left, newNode, username, ID_of_Server, UNIX_TIME_OF_BAN, *root);
        (*root)->height = 1 + leftHeight;
        (*root)->subtreeSize = 1 + Size_of_Tree((*root)->left) + Size_of_Tree((*root)->right);  
    } else {
        int rightHeight = InsertKey(&(*root)->right, newNode, username, ID_of_Server, UNIX_TIME_OF_BAN, *root);
        (*root)->height = 1 + rightHeight;
        (*root)->subtreeSize = 1 + Size_of_Tree((*root)->left) + Size_of_Tree((*root)->right);  
    }
    
    return (*root)->height; 
}

// Finds the scapegoat node if any.
ScapeGoatNode* FindScapegoat(ScapeGoatNode* n, float alpha) {
    int totalSize = 1;
    
    while (n->parent != NULL) {
        ScapeGoatNode* parent = n->parent;
        int siblingSize = parent->subtreeSize - n->subtreeSize - 1;
        totalSize += 1 + siblingSize;

        if (n->height > floor(log(totalSize) / log(1 / alpha))) {
            return parent;
        }

        n = parent;
    }

    return NULL;
}

// Flattens the tree rooted at 'root' into a linked list.
void Flatten_Tree(ScapeGoatNode* root, ScapeGoatNode** head) {
    if (root == NULL) return;
    
    Flatten_Tree(root->right, head);
    
    root->right = *head;
    if (*head != NULL) {
        (*head)->parent = root;
    }
    *head = root;
    
    Flatten_Tree(root->left, head);
}

// Updates the height of a given node based on its children.
void Update_Height_Of_Tree(ScapeGoatNode* node) {
    if (node == NULL) return;
    int leftHeight = (node->left != NULL) ? node->left->height : 0;
    int rightHeight = (node->right != NULL) ? node->right->height : 0;
    node->height = 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);
}

// Builds a height-balanced tree from a linked list of nodes.
ScapeGoatNode* Build_Height_Balanced_Tree(int size, ScapeGoatNode** head) {
    if (size == 0) return NULL;

    int leftSize = (size - 1) / 2;
    int rightSize = size - 1 - leftSize;

    ScapeGoatNode* leftChild = Build_Height_Balanced_Tree(leftSize, head);

    ScapeGoatNode* root = *head;
    *head = (*head)->right;

    root->left = leftChild;
    if (leftChild != NULL) {
        leftChild->parent = root;
    }

    root->right = Build_Height_Balanced_Tree(rightSize, head);
    if (root->right != NULL) {
        root->right->parent = root;
    }

    Update_Height_Of_Tree(root); 
    root->subtreeSize = size; 

    return root;
}

// Rebuilds the tree to maintain the balance property.
void Rebuild_Tree(int size, ScapeGoatNode** root, ScapeGoatNode* scapegoat) {
    ScapeGoatNode* head = NULL;
    Flatten_Tree(scapegoat, &head);

    ScapeGoatNode* newRoot = Build_Height_Balanced_Tree(size, &head);

    if (scapegoat->parent != NULL) {
        if (scapegoat->parent->left == scapegoat) {
            scapegoat->parent->left = newRoot;
        } else {
            scapegoat->parent->right = newRoot;
        }
        newRoot->parent = scapegoat->parent;
    } else {
        *root = newRoot;  
        newRoot->parent = NULL;
    }
}

// Searches for a player in the ScapeGoatTree.
ScapeGoatNode* Search_Player_ScapeGoat(ScapeGoatNode* root, char* username) {
    while (root != NULL) {
        int cmp = strcmp(username, root->username);
        if (cmp == 0) {
            return root;
        } else if (cmp < 0) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return NULL;
}

// Reads player information from a file and constructs a ScapeGoatTree.
ScapeGoatNode* Call_ScapeGoat(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Could not open file: %s\n", file_name);
        return NULL;
    }
    
    ScapeGoatNode* root = NULL;
    int h_alpha = 0; 
    
    char username[16];
    int ID_of_Server;
    long UNIX_TIME_OF_BAN;
    ScapeGoatNode* newNode = NULL;
    while (fscanf(file, "%s %d %ld\n", username, &ID_of_Server, &UNIX_TIME_OF_BAN) != EOF) {
        int height = InsertKey(&root, &newNode, username, ID_of_Server, UNIX_TIME_OF_BAN, NULL);
        
        h_alpha = (height > h_alpha) ? height : h_alpha;

        if (height == -1) {
            continue;
        } else if (height > h_alpha) {
            if (newNode != NULL) {
                ScapeGoatNode* scapegoat = FindScapegoat(newNode, ALPHA);
                if (scapegoat != NULL) {
                    int size = scapegoat->subtreeSize;
                    Rebuild_Tree(size, &root, scapegoat);
                }
            }
        }
    }

    fclose(file);
    
    return root;
}