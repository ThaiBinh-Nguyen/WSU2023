#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define ALPHA 0.57

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
    newNode->subtreeSize = 1; // Khởi tạo với kích thước là 1
    newNode->left = newNode->right = newNode->parent = NULL;
    return newNode;
}

// Hàm thêm Info vào danh sách liên kết của một PlayerNode
void addInfo(PlayerNode* node, int server_id, long unix_time_of_ban) {
    Info* newInfo = createInfo(server_id, unix_time_of_ban);
    newInfo->next = node->info;
    node->info = newInfo;
}
int treeSize(PlayerNode* root) {
    if (root == NULL) {
        return 0;
    }
    return root->subtreeSize;
}

// Hàm chèn key và trả về độ cao của node
int InsertKey(PlayerNode** root, PlayerNode** newNode, char* username, int server_id, long unix_time_of_ban, PlayerNode* parent) {
    if (*root == NULL) {
        *root = createPlayerNode(username, server_id, unix_time_of_ban);
        (*root)->parent = parent; 
        *newNode = *root; // Cập nhật newNode
        return 1; // Độ cao của node mới là 1
    }
    
    int cmp = strcmp(username, (*root)->username);
    if (cmp == 0) {
        addInfo(*root, server_id, unix_time_of_ban);
        *newNode = *root; // Cập nhật newNode
        return (*root)->height; // Trả về độ cao hiện tại của node
    } else if (cmp < 0) {
        int leftHeight = InsertKey(&(*root)->left, newNode, username, server_id, unix_time_of_ban, *root);
        (*root)->height = 1 + leftHeight;
        (*root)->subtreeSize = 1 + treeSize((*root)->left) + treeSize((*root)->right);  // Cập nhật kích thước
    } else {
        int rightHeight = InsertKey(&(*root)->right, newNode, username, server_id, unix_time_of_ban, *root);
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
void Flatten_Tree(PlayerNode* root, PlayerNode** head) {
    if (root == NULL) return;
    
    Flatten_Tree(root->right, head);
    
    root->right = *head;
    if (*head != NULL) {
        (*head)->parent = root;
    }
    *head = root;
    
    Flatten_Tree(root->left, head);
}

// Hàm cập nhật chiều cao của nút
void UpdateHeight(PlayerNode* node) {
    if (node == NULL) return;
    int leftHeight = (node->left != NULL) ? node->left->height : 0;
    int rightHeight = (node->right != NULL) ? node->right->height : 0;
    node->height = 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);
}

PlayerNode* Build_Height_Balanced_Tree(int size, PlayerNode** head) {
    if (size == 0) return NULL;

    int leftSize = (size - 1) / 2;
    int rightSize = size - 1 - leftSize;

    PlayerNode* leftChild = Build_Height_Balanced_Tree(leftSize, head);

    PlayerNode* root = *head;
    *head = (*head)->right;

    root->left = leftChild;
    if (leftChild != NULL) {
        leftChild->parent = root;
    }

    root->right = Build_Height_Balanced_Tree(rightSize, head);
    if (root->right != NULL) {
        root->right->parent = root;
    }

    UpdateHeight(root); // Cập nhật chiều cao
    root->subtreeSize = size; // Cập nhật kích thước

    return root;
}

void RebuildTree(int size, PlayerNode** root, PlayerNode* scapegoat) {
    PlayerNode* head = NULL;
    Flatten_Tree(scapegoat, &head);

    PlayerNode* newRoot = Build_Height_Balanced_Tree(size, &head);

    // Cập nhật cây mới vào vị trí của scapegoat trong cây gốc
    if (scapegoat->parent != NULL) {
        if (scapegoat->parent->left == scapegoat) {
            scapegoat->parent->left = newRoot;
        } else {
            scapegoat->parent->right = newRoot;
        }
        newRoot->parent = scapegoat->parent;
    } else {
        *root = newRoot;  // Cập nhật nút gốc nếu cần
        newRoot->parent = NULL;
    }
}

PlayerNode* SearchPlayer(PlayerNode* root, char* username) {
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

bool idExists(int* arr, int n, int target) {
    for (int i = 0; i < n; ++i) {
        if (arr[i] == target) {
            return true;
        }
    }
    return false;
}

PlayerNode* run_scapegoat(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Could not open file: %s\n", file_name);
        return NULL;  // Thêm dòng này để trả về NULL khi có lỗi
    }
    
    PlayerNode* root = NULL;  // Root của cây
    int h_alpha = 0;  // Độ cao tối đa được phép theo T.h_alpha trong pseudocode
    
    char username[50];
    int server_id;
    long unix_time_of_ban;
    PlayerNode* newNode = NULL;
    while (fscanf(file, "%s %d %ld\n", username, &server_id, &unix_time_of_ban) != EOF) {
        int height = InsertKey(&root, &newNode, username, server_id, unix_time_of_ban, NULL);
        
        // Cập nhật h_alpha
        h_alpha = (height > h_alpha) ? height : h_alpha;

        if (height == -1) {
            continue;  // không thêm được
        } else if (height > h_alpha) {
            if (newNode != NULL) {
                PlayerNode* scapegoat = FindScapegoat(newNode, ALPHA);
                if (scapegoat != NULL) {
                    int size = scapegoat->subtreeSize;
                    RebuildTree(size, &root, scapegoat);
                }
            }
        }
    }

    fclose(file);
    
    return root;  // Trả về root của cây
}


int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <program_name> <file_name>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "scapegoat") == 0) {
        PlayerNode* root = run_scapegoat(argv[2]);
        char username[50];
        while (scanf("%49s", username) != EOF) {
            PlayerNode* player = SearchPlayer(root, username);
            if (player != NULL) {
                Info* info = player->info;
                int server_count = 0;
                int unique_server_count = 0;
                long most_recent_time = 0;

                int* unique_servers = malloc(100 * sizeof(int)); // Giả sử có tối đa 100 server khác nhau

                // Đếm số server và tìm thời gian bị cấm gần đây nhất
                while (info != NULL) {
                    
                    if (!idExists(unique_servers, unique_server_count, info->server_id)) {
                        unique_servers[unique_server_count++] = info->server_id;
                        server_count++;
                    }
                    if (info->unix_time_of_ban > most_recent_time) {
                        most_recent_time = info->unix_time_of_ban;
                    }
                    info = info->next;
                }
                printf("%s was banned from %d servers. most recently on: %ld\n", username, server_count, most_recent_time);
            } else {
                printf("%s is not currently banned from any servers.\n", username);
            }
        }

    } else if (strcmp(argv[1], "btree") == 0) {
        // Code cho btree
    } else {
        printf("Invalid program name. Use 'scapegoat' or 'btree'.\n");
        return 1;
    }

    return 0;
}
