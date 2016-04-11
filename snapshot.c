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
        "LIST KEYS", //3
        "LIST ENTRIES", //4
        "LIST SNAPSHOTS", //5
        "GET", //6
        "DEL", //7
        "PURGE", //8
        "SET", //9
        "PUSH", //10
        "APPEND", //11
        "PICK", //12
        "PLUCK", //13
        "POP", //14
        "DROP", //15
        "ROLLBACK", //16
        "CHECKOUT", //17
        "SNAPSHOT", //18
        "MIN", //19
        "MAX", //20
        "SUM", //21
        "LEN", //22
        "REV", //23
        "UNIQ", //24
        "SORT", //25
        "DIFF", //26
        "INTER", //27
        "UNION" //28

};

void command_bye() {
    printf("bye\n");
}

void command_help() {
    printf("%s\n", HELP);
}

//int command_list(snapshot * current_version,int cmdcounter,char *cmd) {
//    if(){
//        printf("no keys");
//    }
//    if(){
//        printf("no entries");
//    }
//    if(snapshot_head==NULL){
//        printf("no snapshots");
//    }
//}

void command_get() {
    
}

void command_del() {

}

void command_purge() {

}

void command_set() {

    printf("ok");
}

void command_push(){

}
void command_append(){

}
void command_pick(){

}
void command_pluck(){

}
void command_pop(){

}
void command_drop(){

}
void command_rollback(){

}
void command_checkout(){

}
void command_snapshot(){

}
void command_min(){

}
void command_max(){

}
void command_sum(){

}
void command_len(){

}
void command_rev(){

}
void command_uniq(){

}
void command_sort(){

}
void command_diff(){

}
void command_inter(){

}
void command_union(){

}
static void command_compare(snapshot * current_version,int cmdcounter,char *cmd){
    while(i<) {

        if (strcasecmp(cmd[0], commands[i]))
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
        cmd[i]=strtok(line," ");

        while(cmd[i]!=NULL){
            i++;
            cmd[i]=strtok(line," ");
        }
        int cmdcounter = i;
        command_compare(current_version, cmdcounter, cmd);

    }
    free(current_version);
    return 0;
}
