#include "main.h"

int Height(AVLNode* node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

int BalanceFactor(AVLNode* node) {
    if (node == NULL) {
        return 0;
    }
    return Height(node->left) - Height(node->right);
}

AVLNode* RightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T3 = x->right;

    // Thực hiện quay
    x->right = y;
    y->left = T3;

    // Cập nhật chiều cao cho y
    if (Height(y->left) > Height(y->right)) {
        y->height = 1 + Height(y->left);
    } else {
        y->height = 1 + Height(y->right);
    }

    // Cập nhật chiều cao cho x
    if (Height(x->left) > Height(x->right)) {
        x->height = 1 + Height(x->left);
    } else {
        x->height = 1 + Height(x->right);
    }

    return x;
}

AVLNode* LeftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    // Thực hiện quay
    y->left = x;
    x->right = T2;

    // Cập nhật chiều cao cho x
    if (Height(x->left) > Height(x->right)) {
        x->height = 1 + Height(x->left);
    } else {
        x->height = 1 + Height(x->right);
    }

    // Cập nhật chiều cao cho y
    if (Height(y->left) > Height(y->right)) {
        y->height = 1 + Height(y->left);
    } else {
        y->height = 1 + Height(y->right);
    }

    return y;
}


AVLNode* InsertAVL(AVLNode* node, char* username, int server_id, long unix_time_of_ban) {
    // 1. Chèn bình thường như BST
    if (node == NULL) {
        AVLNode* newNode = (AVLNode*)malloc(sizeof(AVLNode));
        strncpy(newNode->username, username, 50);
        newNode->info = (Info*)malloc(sizeof(Info));
        newNode->info->server_id = server_id;
        newNode->info->unix_time_of_ban = unix_time_of_ban;
        newNode->info->next = NULL;
        newNode->height = 1;  // Node mới có chiều cao = 1
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    if (strcmp(username, node->username) < 0) {
        node->left = InsertAVL(node->left, username, server_id, unix_time_of_ban);
    } else if (strcmp(username, node->username) > 0) {
        node->right = InsertAVL(node->right, username, server_id, unix_time_of_ban);
    } else {
        // Trường hợp username đã tồn tại, thêm thông tin mới vào danh sách
        Info* newInfo = (Info*)malloc(sizeof(Info));
        newInfo->server_id = server_id;
        newInfo->unix_time_of_ban = unix_time_of_ban;
        newInfo->next = node->info;
        node->info = newInfo;
        return node;
    }

    // 2. Cập nhật chiều cao của node cha sau khi chèn
    node->height = 1 + max(Height(node->left), Height(node->right));

    // 3. Lấy yếu tố cân bằng để kiểm tra liệu node này có trở nên mất cân bằng không
    int balance = BalanceFactor(node);

    // 4. Nếu mất cân bằng, có 4 trường hợp:

    // Trường hợp Left Left
    if (balance > 1 && strcmp(username, node->left->username) < 0) {
        return RightRotate(node);
    }

    // Trường hợp Right Right
    if (balance < -1 && strcmp(username, node->right->username) > 0) {
        return LeftRotate(node);
    }

    // Trường hợp Left Right
    if (balance > 1 && strcmp(username, node->left->username) > 0) {
        node->left = LeftRotate(node->left);
        return RightRotate(node);
    }

    // Trường hợp Right Left
    if (balance < -1 && strcmp(username, node->right->username) < 0) {
        node->right = RightRotate(node->right);
        return LeftRotate(node);
    }

    return node;
}

AVLNode* SearchAVL(AVLNode* root, char* username) {
    while (root != NULL) {
        int cmp = strcmp(username, root->username);

        // Nếu username trùng khớp
        if (cmp == 0) {
            return root;
        }

        // Nếu username nhỏ hơn root->username
        if (cmp < 0) {
            root = root->left;
        } 
        // Nếu username lớn hơn root->username
        else {
            root = root->right;
        }
    }

    // Nếu không tìm thấy
    return NULL;
}


AVLNode* run_avl(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Could not open file: %s\n", file_name);
        return NULL;
    }

    AVLNode* root = NULL;
    char username[50];
    int server_id;
    long unix_time_of_ban;

    while (fscanf(file, "%s %d %ld\n", username, &server_id, &unix_time_of_ban) != EOF) {
        root = InsertAVL(root, username, server_id, unix_time_of_ban);
    }

    fclose(file);
    return root;
}