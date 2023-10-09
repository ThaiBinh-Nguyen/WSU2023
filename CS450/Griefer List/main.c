#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Info {
    int server_id;
    long unix_time_of_ban;
    struct Info* next;
} Info;

typedef struct PlayerNode {
    char username[50];
    Info* info;
    int height;
    int subtreeSize;
    struct PlayerNode* parent;
    struct PlayerNode* left;
    struct PlayerNode* right;
} PlayerNode;

// Hàm để tạo node Info mới
Info* createInfo(int server_id, long unix_time_of_ban) {
    Info* newInfo = (Info*) malloc(sizeof(Info));
    newInfo->server_id = server_id;
    newInfo->unix_time_of_ban = unix_time_of_ban;
    newInfo->next = NULL;
    return newInfo;
}

// Hàm để tạo node PlayerNode mới
PlayerNode* createPlayerNode(char* username, int server_id, long unix_time_of_ban) {
    PlayerNode* newNode = (PlayerNode*) malloc(sizeof(PlayerNode));
    strcpy(newNode->username, username);
    newNode->info = createInfo(server_id, unix_time_of_ban);
    newNode->height = 1; // Khởi tạo với độ cao là 1
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Hàm thêm Info vào danh sách liên kết của một PlayerNode
void addInfo(PlayerNode* node, int server_id, long unix_time_of_ban) {
    Info* newInfo = createInfo(server_id, unix_time_of_ban);
    newInfo->next = node->info;
    node->info = newInfo;
}

// Hàm chèn key và trả về độ cao của node
int InsertKey(PlayerNode** root, char* username, int server_id, long unix_time_of_ban, PlayerNode* parent)
 {
    if (*root == NULL) {
        *root = createPlayerNode(username, server_id, unix_time_of_ban);
        (*root)->parent = parent; 
        return 1; // Độ cao của node mới là 1
    }
    
    int cmp = strcmp(username, (*root)->username);
    if (cmp == 0) {
        addInfo(*root, server_id, unix_time_of_ban);
        return (*root)->height; // Trả về độ cao hiện tại của node
    } else if (cmp < 0) {
        int leftHeight = InsertKey(&(*root)->left, username, server_id, unix_time_of_ban);
        (*root)->height = 1 + leftHeight;
        (*root)->subtreeSize = 1 + treeSize((*root)->left) + treeSize((*root)->right);  // Cập nhật kích thước
    } else {
        int rightHeight = InsertKey(&(*root)->right, username, server_id, unix_time_of_ban);
        (*root)->height = 1 + rightHeight;
        (*root)->subtreeSize = 1 + treeSize((*root)->left) + treeSize((*root)->right);  // Cập nhật kích thước
    }
    
    return (*root)->height; // Trả về độ cao mới của root
}

// Hàm giả định để tìm scapegoat
PlayerNode* FindScapegoat(PlayerNode* n, float alpha) {
    int totalSize = 1;

    while (n->parent != NULL) {
        PlayerNode* parent = n->parent;
        int siblingSize = parent->subtreeSize - n->subtreeSize - 1;
        totalSize += 1 + siblingSize;

        if (n->height > floor(log(totalSize) / log(1 / alpha))) {
            return parent;
        }

        n = parent;
    }

    return NULL;
}


// Hàm giả định để rebuild tree
void RebuildTree(int size, PlayerNode* scapegoat) {
    // Logic của bạn để rebuild tree
}

void run_scapegoat(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Could not open file: %s\n", file_name);
        return;
    }
    
    PlayerNode* root = NULL;  // Root của cây
    int h_alpha = 0;  // Độ cao tối đa được phép theo T.h_alpha trong pseudocode
    
    char username[50];
    int server_id;
    long unix_time_of_ban;
    while (fscanf(file, "%s %d %ld\n", username, &server_id, &unix_time_of_ban) != EOF) {
        int height = InsertKey(&root, username, server_id, unix_time_of_ban, NULL);
        
        if (height == -1) {
            continue;  // không thêm được
        } else if (height > h_alpha) {
            PlayerNode* scapegoat = FindScapegoat(root, username);
            if (scapegoat != NULL) {
                // Logic để tính size của cây con có gốc là scapegoat
                int size = 0;
                RebuildTree(size, scapegoat);
            }
        }
    }

    fclose(file);
    printf("Scapegoat program finished.\n");
}


// Function to handle the btree program
void run_btree(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Could not open file: %s\n", file_name);
        return;
    }
    // Your code to read and process the file goes here
    fclose(file);
    printf("Btree program finished.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <program_name> <file_name>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "scapegoat") == 0) {
        run_scapegoat(argv[2]);
    } else if (strcmp(argv[1], "btree") == 0) {
        run_btree(argv[2]);
    } else {
        printf("Invalid program name. Use 'scapegoat' or 'btree'.\n");
        return 1;
    }

    return 0;
}
