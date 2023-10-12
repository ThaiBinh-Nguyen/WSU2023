#include "main.h"

bool idExists(int* arr, int n, int target) {
    for (int i = 0; i < n; ++i) {
        if (arr[i] == target) {
            return true;
        }
    }
    return false;
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

    } else if (strcmp(argv[1], "avl") == 0) {
        AVLNode* root = run_avl(argv[2]);

        if (!root) {
            printf("Failed to build the AVL tree.\n");
            return 1;
        }

        char username[50];
        while (scanf("%49s", username) != EOF) {
            AVLNode* player = SearchAVL(root, username);
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

                free(unique_servers);
            } else {
                printf("%s is not currently banned from any servers.\n", username);
            }
        }
    } else {
        printf("Invalid program name. Use 'scapegoat' or 'btree'.\n");
        return 1;
    }

    return 0;
}
