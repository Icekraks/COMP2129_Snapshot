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
const char *commands[] = {
        "BYE", //0
        "HELP", //1
        "LIST KEYS", //2
        "LIST ENTRIES", //3
        "LIST SNAPSHOTS", //4
        "GET", //5
        "DEL", //6
        "PURGE", //7
        "SET", //8
        "PUSH", //9
        "APPEND", //10
        "PICK", //11
        "PLUCK", //12
        "POP", //13
        "DROP", //14
        "ROLLBACK", //15
        "CHECKOUT", //16
        "SNAPSHOT", //17
        "MIN", //18
        "MAX", //19
        "SUM", //20
        "LEN", //21
        "REV", //22
        "UNIQ", //23
        "SORT", //24
        "DIFF", //25
        "INTER", //26
        "UNION" //27

};

void command_bye() {
    printf("bye\n");
}

void command_help() {
    printf("%s\n", HELP);
}

void command_list(snapshot *current_version, int cmdcounter, char *cmd, int index_two) {

}

void command_get() {

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
    printf("ok");
}

void command_diff() {

}

void command_inter() {

}

void command_union() {

}

void command_compare(snapshot *current_version, int cmdcounter, char *cmd) {
    int i = 0;
    int index_one = 0;
    bool isFound = false;
    while (i < sizeof(commands)) {

        if (strcasecmp(cmd[0], commands[i]) == true) {
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
    if (index_one==2||index_one==3||index_one==4) {
        char *second_cmd[3] = {"KEYS", "ENTRIES", "SNAPSHOT"};
        isFound = false;
        i = 0;
        int index_two = 0;
        while (i < sizeof(second_cmd)) {
            if (strcasecmp(cmd[1], second_cmd[i]) == true) {
                isFound = true;
                index_two = i;
                break;
            }
            i++;
        }
        if (isFound == false) {
            return;
            //incorrect command
        }
    }

    if(index_one==0){
        free(current_version);
        command_bye();
    } else if(index_one==1){
        command_help();
    } else if(index_one==2){

    } else if(index_one==3){

    } else if(index_one==4){

    } else if(index_one==5){

    } else if(index_one==6){

    } else if(index_one==7){

    } else if(index_one==8){

    } else if(index_one==9){

    } else if(index_one==10){

    } else if(index_one==11){

    } else if(index_one==12){

    } else if(index_one==13){

    } else if(index_one==14){

    } else if(index_one==15){

    } else if(index_one==16){

    } else if(index_one==17){

    } else if(index_one==18){

    } else if(index_one==19){

    } else if(index_one==20){

    } else if(index_one==21){

    } else if(index_one==22){

    } else if(index_one==23){

    } else if(index_one==24){

    } else if(index_one==25){

    } else if(index_one==26){

    } else if(index_one==27){

    } else if(index_one==28){

    } else {
        return;
    }
}

int main(void) {

    char line[MAX_LINE];
    entry *current_version = malloc(sizeof(entry));
    current_version->next = NULL;
    current_version->prev = NULL;

    while (true) {
        printf("> ");
        fgets(line, sizeof(line), stdin);
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            command_bye();
            return 0;
        }
        strtok(line, "\n");

        char *cmd[MAX_COMMAND];
        int i = 0;
        cmd[i] = strtok(line, " ");

        while (cmd[i] != NULL) {
            i++;
            cmd[i] = strtok(line, " ");
        }
        int cmdcounter = i;
        command_compare(current_version, cmdcounter, *cmd);

    }
    free(current_version);
    return 0;
}
