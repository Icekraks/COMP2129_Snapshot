/**
 * comp2129 - assignment 2
 * <Felix Hu>
 * <fehu4705>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <search.h>

#include "snapshot.h"

entry *entry_head = NULL;
entry *entry_tail = NULL;

snapshot *snapshot_head = NULL;
snapshot *snapshot_tail = NULL;

//
// We recommend that you design your program to be
// modular where each function performs a small task
//
// e.g.
//
// command_bye
// command_help
// ...
// entry_add
// entry_delete
// ...
//

const char *commands[26] = {
        "BYE", //0
        "HELP", //
        "GET", //2
        "DEL", //3
        "PURGE", //4
        "SET", //5
        "PUSH", //6
        "APPEND", //7
        "PICK", //8
        "PLUCK", //9
        "POP", //10
        "DROP", //11
        "ROLLBACK", //12
        "CHECKOUT", //13
        "SNAPSHOT", //14
        "MIN", //15
        "MAX", //16
        "SUM", //17
        "LEN", //18
        "REV", //19
        "UNIQ", //20
        "SORT", //21
        "DIFF", //22
        "INTER", //23
        "UNION", //24
        "LIST" //25
//        "LIST KEYS", //26
//        "LIST ENTRIES", //27
//        "LIST SNAPSHOTS", //28

};

void command_bye() {
    //free all memory....
    printf("bye\n");
}

void command_help() {
    printf("%s\n", HELP);
}

void command_list(int index_two) {
    if (index_two == 0) {
        if (entry_head == NULL) {
            printf("no keys\n");
            return;
        }
        entry *temp = entry_head;
        while (temp->next != NULL) {
            printf("%s\n", temp->key);
            if (temp->next == NULL) {
                return;
            }
            temp = temp->next;
        }

    }
    else if (index_two == 1) {
        if (entry_head == NULL) {
            printf("no entries\n");
            return;
        }
        entry *temp = entry_head;
        while (temp->next != NULL) {
            printf("%s ", temp->key);
            printf("[");
            int size = sizeof(temp->values) / sizeof(temp->values[0]);
            for (int i = 0; i < size; i++) {
                printf("%s ", temp->values[i]);
            }
            printf("]");
            if (temp->next == NULL) {
                return;
            }
            temp = temp->next;
        }
    }
    else if (index_two == 2) {
        if (snapshot_head == NULL) {
            printf("no snapshots\n");
            return;
        }
        snapshot *temp = entry_head;

        while (temp->next != NULL) {
            printf("%s ", temp->id);
            if (temp->next == NULL) {
                return;
            }
            temp = temp->next;
        }
    }
    else {
        return;
    }

}

void command_get(char *cmd) {
    if (entry_head == NULL) {
        printf("no key");
    }
    entry *temp = entry_head;
    while (temp->next != NULL) {
        int compare = strcasecmp(temp->key, cmd[1]);
        if (compare == 0) {
            int size = sizeof(temp->values) / sizeof(temp->values[0]);
            printf("[");
            for (int i = 0; i < size; i++) {
                printf("%d", temp->values[i]);
            }
            printf("]\n");
            return;
        }
        if (temp->next == NULL) {
            printf("no key\n");
            return;
        }
        temp = temp->next;
    }
}

void command_del(char *cmd) {
    entry *current_version = entry_head;
    while(current_version->next != NULL){
        int result = strcmp(current_version->key,cmd[1]);
        if(result==0){
            entry * temp = current_version->prev;
            entry * temp2 = current_version->next;
            free(current_version->values);
            free(current_version->key);
            temp->next = temp2;
            temp2->prev = temp;
        }

        if(current_version->next==NULL) {
            printf("No Key");
            return;
        }
        current_version = current_version->next;
    }
    printf("ok\n");
}

void command_purge() {
    printf("ok\n");
}
//method is only run when there is no entries in entries
entry *set_creation(int cmd_counter, char *cmd) {

    entry *new_entry = malloc(sizeof(new_entry));
    strcpy(new_entry->key, cmd[1]);
    int value_size = (sizeof(new_entry->values));
    new_entry->values = malloc((cmd_counter - 1) * value_size);
    for (int i = 1; i < cmd_counter - 2; i++) {
        strcpy(new_entry->values[i], cmd[i]);
    }
    new_entry->length = (cmd_counter - 2);
    new_entry->next = NULL;
    new_entry->prev = NULL;
    return new_entry;

}
/*
 * Method below and helper method above sets the values of the
 */
void command_set(int cmd_counter, char **cmd) {
    //If there is no entries in the linked list.
    if (entry_head == NULL) {
        entry_head = set_creation(cmd_counter, *cmd);
        printf("ok\n");
        return;
    }
    int result = (strcasecmp(entry_head->key,cmd[1]));
    //If there is one entry in the linked list.
    if (entry_head != NULL && entry_head->next == NULL && result == 0) {
        entry *new_entry = malloc(sizeof(new_entry));
        strcpy(new_entry->key, cmd[1]);
        int value_size = (sizeof(new_entry->values));
        new_entry->values = malloc((cmd_counter - 1) * value_size);
        for (int i = 1; i < cmd_counter - 2; i++) {
            strcpy(new_entry->values[i], cmd[i]);
        }
        new_entry->length = (cmd_counter - 2);
        new_entry->next = NULL;
        new_entry->prev = entry_head;
        entry_head->next = new_entry;
        printf("ok\n");
        return;
    }
    //If there is multiple entries in the linked list.
    if (entry_head != NULL && entry_head->next != NULL) {
        entry *temp = entry_head;
        while (temp->next != NULL) {
            //If the key already exists and the values are being updated.
            int result = strcasecmp((temp->key), cmd[1]);
            if (result == 0) {
                int i = 0;
                while (temp->values != NULL) {
                    free(temp->values);
                    i++;
                }
                i = 1;
                while (cmd != NULL) {
                    int value_size = (sizeof(temp->values));
                    temp->values = malloc((cmd_counter - 1) * value_size);
                    for (int i = 1; i < cmd_counter - 2; i++) {
                        temp->values[i] = atoi(cmd[i]);
                    }
                }
                return;
            }
            if (temp->next == NULL) {
                break;
            }
            temp = temp->next;
        }
        //If the key doesnt exist but there is more than two entries.
        entry *new_entry = malloc(sizeof(new_entry));
        strcpy(new_entry->key, cmd[1]);
        int value_size = (sizeof(new_entry->values));
        new_entry->values = malloc((cmd_counter - 1) * value_size);
        for (int i = 1; i < cmd_counter - 2; i++) {
            strcpy(new_entry->values[i], cmd[i]);
        }
        new_entry->length = (cmd_counter - 2);
        new_entry->next = NULL;
        new_entry->prev = temp;
        temp->next = new_entry;
        entry_tail = new_entry;
        printf("ok\n");
        return;
    }

}

/*
 * Method that handles the push where the values are added to the front of the value list.
 */
void command_push(int cmd_counter, char *cmd) {
    entry *temp = entry_head;
    while (temp->next != NULL) {
        //If the key already exists and the values are being updated.
        int result = strcasecmp((temp->key), cmd[1]);
        if (result == 0) {
            int old_index = sizeof(temp->values)/ sizeof(temp->values[1]);
            int new_size = sizeof(temp->values) + (sizeof(cmd[1]) * (cmd_counter - 1));
            int temporary_reorganise[(old_index+1)];
            for(int i = 1;i<(old_index+!1);i++){
                int temporary_reorganise[i] = temp->values[i];
            }

            temp->values = realloc(temp->values, new_size);
            int new_index = sizeof(temp->values)/ sizeof(temp->values[1]);
            for (int i = (old_index+1); i < (new_index+1); i++) {
                temp->values[i] = atoi(cmd[i]);
            }
            int i = 1;
            int j = (sizeof(temporary_reorganise))/ (sizeof(temporary_reorganise[1]));
            while(temporary_reorganise!=NULL){
                temp->values[old_index+i] = temporary_reorganise[j];
                if(j = 1){
                    break;
                }
                j--;
                i++;
            }
            printf("ok");
            return;
        }
        if(temp->next==NULL){
            printf("key doesnt exist");
            return;
        }
        temp = temp->next;

    }

    printf("ok");
}

/*
 * Method handles the user appending, the assumption is that there is a entry in the linked list.
 * the values are appended or added to the end of the values for the entry.
 */

void command_append(int cmd_counter, char *cmd) {
    entry *temp = entry_head;
    while (temp->next != NULL) {
        //If the key already exists and the values are being updated.
        int result = strcasecmp((temp->key), cmd[1]);
        if (result == 0) {
            int old_index = sizeof(temp->values)/ sizeof(temp->values[1]);
            int new_size = sizeof(temp->values) + (sizeof(cmd[1]) * (cmd_counter - 1));
            temp->values = realloc(temp->values, new_size);
            int new_index = sizeof(temp->values)/ sizeof(temp->values[1]);
            for (int i = (old_index+1); i < (new_index+1); i++) {
                temp->values[i] = atoi(cmd[i]);
            }
            printf("ok");
            return;
        }
        if(temp->next==NULL){
            printf("key doesnt exist");
            return;
        }
        temp = temp->next;

    }
}

void command_pick() {

}

void command_pluck() {

}

void command_pop() {

}

void command_drop() {

}

void command_rollback() {
    printf("ok");
}

void command_checkout() {

}

void command_snapshot() {

}

void command_min() {

}

void command_max() {

}

void command_sum() {

}

void command_len() {

}

void command_rev() {
    printf("ok");
}

void command_uniq() {
    printf("ok");
}

void command_sort() {
    //use qSort()
    printf("ok");
}

void command_diff() {

}

void command_inter() {

}

void command_union() {

}

void command_compare(int cmd_counter, char **cmd) {
    int i = 0;
    int index_one;
    int index_two;
    bool isFound = false;
    bool isFound2 = false;
    int size = (sizeof(commands) / sizeof(commands[0]));
    while (i < size) {
        int result = strcasecmp(cmd[0], commands[i]);
        if (result == 0) {
            isFound = true;
            index_one = i;
            break;
        }
        i++;
    }
    if (isFound == false) {
        return;
        //incorrect command
    }

    if (cmd[1] != NULL && index_one == 25) {
        int j = 0;
        char *second_cmd[3] = {"KEYS", "ENTRIES", "SNAPSHOT"};
        size = (sizeof(second_cmd) / sizeof(second_cmd[0]));
        while (j < size) {
            int result = strcasecmp(cmd[1], second_cmd[j]);
            if (result == 0) {
                isFound2 = true;
                index_two = j;
                break;
            }
            j++;
        }
        if (isFound2 == false) {
            return;
            //incorrect command
        }
    }

    if (index_one == 0) {
        command_bye();
        exit(0);
    } else if (index_one == 1) {
        command_help();
    } else if (index_one == 2) {
        command_get(*cmd);
    } else if (index_one == 3) {
        command_del(*cmd);
    } else if (index_one == 4) {

    } else if (index_one == 5) {
        command_set(cmd_counter, *cmd);
    } else if (index_one == 6) {

    } else if (index_one == 7) {
        command_append(cmd_counter, *cmd);
    } else if (index_one == 8) {

    } else if (index_one == 9) {

    } else if (index_one == 10) {

    } else if (index_one == 11) {

    } else if (index_one == 12) {

    } else if (index_one == 13) {

    } else if (index_one == 14) {

    } else if (index_one == 15) {

    } else if (index_one == 16) {

    } else if (index_one == 17) {

    } else if (index_one == 18) {

    } else if (index_one == 19) {

    } else if (index_one == 20) {

    } else if (index_one == 21) {

    } else if (index_one == 22) {

    } else if (index_one == 23) {

    } else if (index_one == 24) {

    } else if (index_one == 25 && (index_two == 0 || index_two == 1 || index_two == 2)) {
        command_list(index_two);
    } else {
        return;
    }
}

int main(void) {
    /*
     * Main function that handles the user input by breaking the string up into its components.
     */
    char line[MAX_LINE];


    while (true) {
        printf("> ");
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            command_bye();
            return 0;
        }

        char *cmd[MAX_COMMAND];
        int i = 0;
        strtok(line, "\n");
        cmd[i] = strtok(line, " ");

        while (cmd[i] != NULL) {
            i++;
            cmd[i] = strtok(NULL, " ");
        }
        int cmd_counter = i;
        command_compare(cmd_counter, cmd);
    }
    return 0;
}
