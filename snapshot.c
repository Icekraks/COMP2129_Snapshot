/**
 * comp2129 - assignment 2
 * <your name>
 * <your unikey>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

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
    printf("bye\n");
}

void command_help() {
    printf("%s\n", HELP);
}

void command_list(snapshot *current_version, int cmd_counter, char *cmd, int index_two) {
    if (index_two == 1) {

    }
}

void command_get(snapshot *current_version, int cmd_counter, char *cmd) {

}

void command_del() {
    printf("ok");
}

void command_purge() {
    printf("ok");
}

void command_set() {

    printf("ok");
}

void command_push() {

    printf("ok");
}

void command_append() {
    printf("ok");
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

void command_compare(snapshot *current_version, int cmd_counter, char **cmd) {
    int i = 0;
    int index_one = 0;
    int index_two = 0;
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
        char *second_cmd[3] = {"KEYS", "ENTRIES", "SNAPSHOT"};
        while (i < (sizeof(second_cmd) / sizeof(commands[0]))) {
            int result = strcasecmp(cmd[0], commands[i]);
            if (result == 0) {
                isFound2 = true;
                index_two = i;
                break;
            }
            i++;
        }
        if (isFound2 == false) {
            return;
            //incorrect command
        }

    }

    if (index_one == 0) {
        free(current_version);
        command_bye();
        exit(0);
    } else if (index_one == 1) {
        command_help();
    } else if (index_one == 2) {
        command_list(current_version, cmd_counter, *cmd, index_two);
    } else if (index_one == 3) {

    } else if (index_one == 4) {

    } else if (index_one == 5) {

    } else if (index_one == 6) {

    } else if (index_one == 7) {

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

    } else if (index_one == 25) {

    } else {
        return;
    }
}

int main(void) {
    /*
     * Main function that handles the user input by breaking the string up into its components.
     */
    char line[MAX_LINE];
    snapshot *current_version = malloc(sizeof(snapshot));
    current_version->entries = NULL;
    current_version->next = NULL;
    current_version->prev = NULL;

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
        command_compare(current_version, cmd_counter, cmd);
    }
    free(current_version);
    return 0;
}
