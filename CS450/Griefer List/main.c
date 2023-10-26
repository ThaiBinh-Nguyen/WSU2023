#include "main.h"

//Checking if ID of Server exist or not in Tree
bool If_idServer_ExistOrNot(int* array, int n, int id_target) {
    for (int i = 0; i < n; ++i) {
        if (array[i] == id_target) {
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


    //SCAPEGOAT PROGRAM

    if (strcmp(argv[1], "scapegoat") == 0) {
        ScapeGoatNode* root = Call_ScapeGoat(argv[2]);
        char username[50];
        while (scanf("%49s", username) != EOF) {
            //Search Player exists or not first. If not => create new Node
            //If Player exists => Add new information (ServerID, UNIX_TIME_OF_BAN)
            ScapeGoatNode* Player = Search_Player_ScapeGoat(root, username);
            if (Player != NULL) {
                Information_of_Player* information = Player->information;
                int Number_of_Server = 0;
                int Unique_number_of_Server = 0;
                long MostRecent_Time = 0;

                int* Unique_Server = malloc(100 * sizeof(int)); 

                
                while (information != NULL) {
                    //Count the number of SERVERID
                    if (!If_idServer_ExistOrNot(Unique_Server, Unique_number_of_Server, information->ID_of_Server)) {
                        Unique_Server[Unique_number_of_Server++] = information->ID_of_Server;
                        Number_of_Server++;
                    }
                    //RETURN the most recent banned time 
                    if (information->UNIX_TIME_OF_BAN > MostRecent_Time) {
                        MostRecent_Time = information->UNIX_TIME_OF_BAN;
                    }
                    information = information->next;
                }
                printf("%s was banned from %d servers. most recently on: %ld\n", username, Number_of_Server, MostRecent_Time);
            } else {
                printf("%s is not currently banned from any servers.\n", username);
            }
        }

    // AVL PROGRAM

    } else if (strcmp(argv[1], "avl") == 0) {
        AVLNode* root = Call_AVL(argv[2]);

        if (!root) {
            printf("Failed to build the AVL tree.\n");
            return 1;
        }

        char username[50];
        while (scanf("%49s", username) != EOF) {
            //Search Player exists or not first. If not => create new Node
            //If Player exists => Add new information (ServerID, UNIX_TIME_OF_BAN)
            AVLNode* Player = Search_Player_AVL(root, username);
            if (Player != NULL) {
                Information_of_Player* information = Player->information;
                int Number_of_Server = 0;
                int Unique_number_of_Server = 0;
                long MostRecent_Time = 0;

                int* Unique_Server = malloc(100 * sizeof(int));

                
                while (information != NULL) {
                    if (!If_idServer_ExistOrNot(Unique_Server, Unique_number_of_Server, information->ID_of_Server)) {
                        Unique_Server[Unique_number_of_Server++] = information->ID_of_Server;
                        Number_of_Server++;
                    }
                    if (information->UNIX_TIME_OF_BAN > MostRecent_Time) {
                        MostRecent_Time = information->UNIX_TIME_OF_BAN;
                    }
                    information = information->next;
                }
                printf("%s was banned from %d servers. most recently on: %ld\n", username, Number_of_Server, MostRecent_Time);

                free(Unique_Server);
            } else {
                printf("%s is not currently banned from any servers.\n", username);
            }
        }
    } else {
        printf("Invalid program name. Use 'scapegoat' or 'avl'.\n");
        return 1;
    }

    return 0;
}
