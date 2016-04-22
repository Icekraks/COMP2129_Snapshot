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

const char *commands[26] = {
        "BYE",      //0
        "HELP",     //1
        "GET",      //2
        "DEL",      //3
        "PURGE",    //4
        "SET",      //5
        "PUSH",     //6
        "APPEND",   //7
        "PICK",     //8
        "PLUCK",    //9
        "POP",      //10
        "DROP",     //11
        "ROLLBACK", //12
        "CHECKOUT", //13
        "SNAPSHOT", //14
        "MIN",      //15
        "MAX",      //16
        "SUM",      //17
        "LEN",      //18
        "REV",      //19
        "UNIQ",     //20
        "SORT",     //21
        "DIFF",     //22
        "INTER",    //23
        "UNION",    //24
        "LIST"      //25
//        "LIST KEYS", //26
//        "LIST ENTRIES", //27
//        "LIST SNAPSHOTS", //28

};
int snapshot_id = 1;

void static free_values(entry *key) {
    free(key->values);
    key->values = NULL;
}

static entry *find_key(char *key) {
    entry *curr = entry_head;
    entry *found = NULL;

    while (curr != NULL && found == NULL) {
        int result = strcmp(curr->key, key);
        if (result == 0) {
            found = curr;
            return found;
        }
        curr = curr->next;
    }
    return NULL;
}

static entry *populate_snapshot() {
    entry *live_entries = entry_head;
    entry *snap_entry_head = NULL;
    entry *snap_entry_tail = NULL;
    while (live_entries->next != NULL) {
        int length = live_entries->length;
        entry *copy_entry = malloc(sizeof(entry));
        copy_entry->values = malloc(sizeof(int) * length);
        copy_entry->next = NULL;
        copy_entry->prev = NULL;
        strcpy(copy_entry->key, live_entries->key);
        for (int i = 0; i < length; i++) {
            copy_entry->values[i] = live_entries->values[i];
        }
        copy_entry->length = length;

        if (snap_entry_head == NULL) {
            snap_entry_head = copy_entry;
            snap_entry_tail = copy_entry;
        } else {
            snap_entry_tail->next = copy_entry;
            copy_entry->prev = snap_entry_tail;
            snap_entry_tail = copy_entry;
        }

        live_entries = live_entries->next;
    }
    return snap_entry_head;
}

void static free_entries(entry *free_entry) {
    entry *temp = free_entry;
    if (temp == NULL) {
        return; //If head is null, no need to free memory
    }
    if (temp->next == NULL) {
        free_values(temp); //If there is no next, then free the node
        free(temp);
    } else {
        temp = temp->next;
        while (temp->next != NULL) {
            free_values(temp->prev); //Otherwise, free the node and values before it, and move to next node
            free(temp->prev);
            temp = temp->next;
        }
        free_values(temp);
        free(temp->key);
    }
}

void command_bye() {
    printf("bye\n");
    entry *temp = entry_head;
    //TODO: put this section in a seperate method for snapshot easyness
    if (entry_head == NULL) {
        exit(0); //If head is null, no need to free memory
    }
    //TODO: Use Delete Method??
    if (temp->next == NULL) {
        free(temp->values); //If there is no next, then free the node
        free(temp);
    } else {
        temp = temp->next;
        while (temp->next != NULL) {
            free_values(temp->prev); //Otherwise, free the node and values before it, and move to next node
            free(temp->prev);
            temp = temp->next;
        }
        free_values(temp);
        free(temp->key);
    }
    exit(0);
}


void command_help() {
    printf("%s\n", HELP);
}

/*
 * The Method that displays a list of keys, the list of keys is from newest to oldest
 */
void command_listkeys() {
    entry *temp = entry_head;
    if (temp == NULL) {
        printf("no keys\n");
    } else {
        while (temp != NULL) {
            printf("%s\n", temp->key);
            temp = temp->next;
        }
    }
    printf("\n");
}

/*
 * The Method that displays a list of keys and the values with the keys,
 * the list of keys and values is from newest to oldest
 */
void command_listentries() {
    entry *temp = entry_head;
    if (temp == NULL) {
        printf("no entries\n");
        printf("\n");
        return;
    } else {
        while (temp != NULL) {
            printf("%s ", temp->key);
            printf("[");
            for (int i = 0; i < temp->length; i++) {
                printf("%d", temp->values[i]);
                if (i != temp->length - 1) {
                    printf(" ");
                }
            }
            printf("]\n");
            temp = temp->next;
        }
    }
    printf("\n");
}

/*
 * The Method that displays a list of all the available snapshots from newest to oldest.
 */
void command_listsnapshots() {
    if (snapshot_head == NULL) {
        printf("no snapshots\n");
        printf("\n");
        return;
    }
    snapshot *temp = snapshot_head;

    while (temp->next != NULL) {
        printf("%d ", temp->id);
        if (temp->next == NULL) {
            return;
        }
        temp = temp->next;
    }
    printf("\n");
}

/*
 * Method that gets the values for a given key.
 */
void command_get(char **cmd) {
    entry *key = find_key(cmd[1]);
    if (key == NULL) {
        printf("no such key\n\n");
        return;
    }
    int len = key->length;
    int values[len];
    for (int i = 0; i < len; i++) {
        values[i] = key->values[i];
    }
    printf("[");
    for (int i = 0; i < len; i++) {
        printf("%d", values[i]);
        if (i < len - 1) {
            printf(" ");
        }
    }
    printf("]\n\n");

}

/*
 * Method handles the del command when the user wants to delete the key.
 */
void command_del(char **cmd) {
    entry *del_val = find_key(cmd[1]);
    if (del_val == NULL) {
        printf("no such key\n\n");
        return;
    }
    entry *prev = del_val->prev;
    entry *next = del_val->next;
    if (prev == NULL) {
        entry_head = next;
    } else if (next == NULL) {
        entry_tail = prev;
    } else {
        prev->next = next;
        next->prev = prev;
    }
    free_values(del_val);
    free(del_val);
    printf("ok\n\n");
}

void command_purge(char **cmd) {
//    entry * current = find_key(current_snapshot,cmd[1]);
//    if (current == NULL) {
//        printf("no such key\n\n");
//        return;
//    }
//    command_del(current_snapshot,cmd);
    printf("ok\n\n");
}

/*
 * Method handles the set command when the user wants to set values to a key, the key can either
 * pre-exist or be brand new.
 */
void command_set(int cmd_counter, char **cmd) {
    //TODO: MEMORY LEAK
    char *key = cmd[1];
    entry *found = find_key(key);

    if (found == NULL) {
        char *key = cmd[1];
        entry *new_entry = malloc(sizeof(entry));
        new_entry->next = NULL;
        new_entry->prev = NULL;
        strcpy(new_entry->key, key);
        int length = cmd_counter - 2;
        new_entry->values = malloc(sizeof(int) * length);
        for (int i = 0; i < length; i++) {
            new_entry->values[i] = atoi(cmd[i + 2]);
        }
        new_entry->length = length;

        if (entry_head == NULL) {
            entry_head = new_entry;
            entry_tail = new_entry;
        } else {
            entry_tail->next = new_entry;
            new_entry->prev = entry_tail;
            entry_tail = new_entry;
        }
    }
    if (found != NULL) {
        free_values(found);
        int length = cmd_counter - 2;
        found->values = malloc(sizeof(int) * length);
        for (int i = 0; i < length; i++) {
            found->values[i] = atoi(cmd[i + 2]);
        }
    }
    printf("ok\n\n");
}

/*
 * Method handles the user push(adding to the front), the assumption is that there is a entry in the linked list.
 * the values are pushed or added to the start of the values for the entry.
 */
void command_push(int cmd_counter, char **cmd) {
    entry *current_entry = find_key(cmd[1]);
    if (current_entry == NULL) {
        printf("no such key\n\n");
        return;
    }
    int old_len = current_entry->length;
    int new_len = cmd_counter - 2;
    int total_len = new_len + old_len;
    int current_value[old_len];
    int holding_array[new_len];

    for (int i = 0; i < old_len; i++) {
        current_value[i] = current_entry->values[i];
    }
    printf("\n");
    for (int i = 0; i < new_len; i++) {
        holding_array[i] = atoi(cmd[i + 2]);
    }
    int backwards[new_len];
    int reverse_index = new_len - 1;
    for (int i = 0; i < new_len; i++) {
        backwards[i] = holding_array[reverse_index - i];
    }
    free_values(current_entry);
    current_entry->values = malloc(sizeof(int) * total_len);
    for (int j = 0; j < old_len; j++) {
        current_entry->values[j] = backwards[j];
    }
    for (int z = 0; z < new_len; z++) {
        current_entry->values[old_len + z] = holding_array[z];
    }

    current_entry->length = total_len;

    printf("ok\n\n");
}

/*
 * Method handles the user appending(adding to the end), the assumption is that there is a entry in the linked list.
 * the values are appended or added to the end of the values for the entry.
 */
void command_append(int cmd_counter, char **cmd) {
    entry *current_entry = find_key(cmd[1]);
    if (current_entry == NULL) {
        printf("no such key\n\n");
        return;
    }
    int old_len = current_entry->length;
    int new_len = cmd_counter - 2;
    int total_len = new_len + old_len;
    int current_value[old_len];
    int holding_array[new_len];

    for (int i = 0; i < old_len; i++) {
        current_value[i] = current_entry->values[i];
    }
    current_entry->values = realloc(current_entry->values, sizeof(int) * total_len);
    for (int j = 0; j < old_len; j++) {
        current_entry->values[j] = current_value[j];
    }
    printf("\n");
    for (int x = 0; x < new_len; x++) {
        holding_array[x] = atoi(cmd[x + 2]);
    }
    for (int z = 0; z < new_len; z++) {
        current_entry->values[old_len + z] = holding_array[z];
    }
    current_entry->length = total_len;

    printf("ok\n\n");
}

/*
 * This method handles the picking of a specific value and displays it.
 */
void command_pick(char **cmd) {
    entry *current_entry = find_key(cmd[1]);
    if (current_entry == NULL) {
        printf("no such key\n\n");
        return;
    }
    if (current_entry->length == 0) {
        printf("nil\n\n");
        return;
    }
    int index = atoi(cmd[2]) - 1;
    if (index < 0 || index >= current_entry->length) {
        printf("index out of range\n\n");
        return;
    }
    int value = current_entry->values[index];
    printf("%d\n\n", value);
}

/*
 * Method handles Pluck command, which find the index in the array of values, deletes the value
 * and displays it.
 */
void command_pluck(char **cmd) {
    entry *current_entry = find_key(cmd[1]);
    if (current_entry == NULL) {
        printf("no such key\n\n");
        return;
    }
    if (current_entry->length == 0) {
        printf("nil\n\n");
        return;
    }
    int index = atoi(cmd[2]) - 1;
    if (index < 0 || index >= current_entry->length) {
        printf("index out of range\n\n");
        return;
    }
    int value = current_entry->values[index];
    printf("%d\n", value);
    int holding_array[current_entry->length];
    for (int i = 0; i < current_entry->length; i++) {
        holding_array[i] = current_entry->values[i];
    }

    current_entry->length = current_entry->length - 1;
    free_values(current_entry);
    current_entry->values = malloc(sizeof(int) * current_entry->length);
    for (int z = 0; z < index; z++) {
        current_entry->values[z] = holding_array[z];
    }
    for (int z = index; z < current_entry->length; z++) {
        current_entry->values[z] = holding_array[z + 1];
    }

    printf("\n");
}

/*
 * Method handles Pop command, which gets the first index in the array of values, deletes the value
 * and displays it.
 */
void command_pop(char **cmd) {
    entry *current_entry = find_key(cmd[1]);
    if (current_entry == NULL) {
        printf("no such key\n\n");
        return;
    }
    if (current_entry->length == 0) {
        printf("nil\n\n");
        return;
    }
    int index = 0;
    if (index < 0 || index >= current_entry->length) {
        printf("index out of range\n\n");
        return;
    }
    int value = current_entry->values[index];
    printf("%d\n", value);
    int holding_array[current_entry->length];
    for (int i = 0; i < current_entry->length; i++) {
        holding_array[i] = current_entry->values[i];
    }

    current_entry->length = current_entry->length - 1;
    free_values(current_entry);
    current_entry->values = malloc(sizeof(int) * current_entry->length);
    for (int z = 0; z < index; z++) {
        current_entry->values[z] = holding_array[z];
    }
    for (int z = index; z < current_entry->length; z++) {
        current_entry->values[z] = holding_array[z + 1];
    }

    printf("\n");
}

void command_drop() {

}

void command_rollback() {
    

    printf("ok");
}

void command_checkout() {

}

void command_snapshot() {
    snapshot *new_snap = malloc(sizeof(snapshot));
    new_snap->next = NULL;
    new_snap->prev = NULL;
    new_snap->id = snapshot_id;
    new_snap->entries = populate_snapshot();

    if (snapshot_head == NULL) {
        snapshot_head = new_snap;
        snapshot_tail = new_snap;
    } else {
        snapshot_tail->next = new_snap;
        new_snap->prev = snapshot_tail;
        snapshot_tail = new_snap;
    }
    printf("saved as snapshot %d\n\n", snapshot_id);
    snapshot_id++;
}

/*
 * Gets the Min value within the key.
 */
void command_min(char **cmd) {
    entry *current_entry = find_key(cmd[1]);
    if (current_entry == NULL) {
        printf("no such key\n\n");
        return;
    }
    int min = current_entry->values[0];
    for (int i = 0; i < current_entry->length; i++) {
        if (current_entry->values[i] < min) {
            min = current_entry->values[i];
        }
    }
    printf("%d\n\n", min);
}

/*
 * Gets the Max value within the key.
 */
void command_max(char **cmd) {
    entry *current_entry = find_key(cmd[1]);
    if (current_entry == NULL) {
        printf("no such key\n\n");
        return;
    }
    int max = current_entry->values[0];
    for (int i = 0; i < current_entry->length; i++) {
        if (current_entry->values[i] > max) {
            max = current_entry->values[i];
        }
    }
    printf("%d\n\n", max);
}

/*
 * Gets the sum of all the value within the key.
 */
void command_sum(char **cmd) {
    entry *current_entry = find_key(cmd[1]);
    if (current_entry == NULL) {
        printf("no such key\n\n");
        return;
    }
    int sum = 0;
    for (int i = 0; i < current_entry->length; i++) {
        sum += current_entry->values[i];
    }
    printf("%d\n\n", sum);
}

/*
 * Gets the length value within the key.
 */
void command_len(char **cmd) {
    entry *current_entry = find_key(cmd[1]);
    if (current_entry == NULL) {
        printf("no such key\n\n");
        return;
    }
    int length = current_entry->length;
    printf("%d\n\n", length);
}

/*
 * Method Reverses the values within the key. Still Work in Progress
 */
void command_rev(char **cmd) {
    entry *current_entry = find_key(cmd[1]);
    if (current_entry == NULL) {
        printf("no such key\n\n");
        return;
    }
    int length = current_entry->length;
    int forwards[length];
    for (int i = 0; i < length; i++) {
        forwards[i] = current_entry->values[i];
    }
    int backwards[length];
    int reverse_index = length - 1;
    for (int i = 0; i < length; i++) {
        backwards[i] = forwards[reverse_index - i];
    }
    for (int i = 0; i < length; i++) {
        current_entry->values[i] = backwards[i];
    }
    printf("ok\n\n");
}

void command_uniq(snapshot *current_snapshot, char **cmd) {
//    entry *current_entry = find_key(current_snapshot, cmd[1]);
//    if (current_entry == NULL) {
//        printf("no such key\n\n");
//        return;
//    }
//    int length = current_entry->length;
//    int *holding_array;
//    for (int i = 0; i < length; i++) {
//        holding_array[i] = current_entry->values[i];
//    }
//    int repeat = 0;
//    int j = 0;
//    while (j < length) {
//        int i = j;
//        for (; i < length; i++) {
//            if (holding_array[i] == holding_array[i]) {
//                repeat++;
//            }
//            if (holding_array[i] != holding_array[i]) {
//                break;
//            }
//        }
//        if (repeat > 0) {
//            for (int z = 0; z < (j + 1); z++) {
//                holding_array[z] = holding_array[z];
//            }
//            for (int z = (j + 1); z < (length - repeat); z++) {
//                holding_array[z] = holding_array[z + i];
//            }
//        }
//        repeat = 0;
//        j++;
//    }

    printf("ok\n\n");
}

int comparison(const void *larger, const void *smaller) {
    return (*(int *) larger - *(int *) smaller);
}

void command_sort(char **cmd) {
    entry *current_entry = find_key(cmd[1]);
    if (current_entry == NULL) {
        printf("no such key\n\n");
        return;
    }
    qsort(current_entry->values, current_entry->length, sizeof(current_entry->values[0]), comparison);
    printf("ok\n\n");
}

void command_diff() {

}

void command_inter() {

}

void command_union() {

}

/*
 * Method that handles comparison of the command and to figure out what the user is inputing
 */
void command_compare(int cmd_counter, char **cmd) {
    int i = 0;
    int index_one;
    int index_two;
    bool isFound = false;
    bool isFound2 = false;
    int size = (sizeof(commands) / sizeof(commands[0]));
    /*
     * Both while loops handles the index allocation for later comparison.
     */
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
        char *second_cmd[3] = {"KEYS", "ENTRIES", "SNAPSHOTS"};
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
    int size2 = sizeof(cmd) / sizeof(cmd[0]);

    /*
     * All these else if statements handles all the commands on large scale comparing what the index is to the
     * predefined indexes of the command array.
     */
    if (index_one == 0) {
        command_bye();
    } else if (index_one == 1 && cmd[0] != NULL) {//done
        command_help();
    } else if (index_one == 2 && cmd[1] != NULL && size2 < 2) {//done
        command_get(cmd);
    } else if (index_one == 3 && cmd[1] != NULL && size2 < 2) {//done
        command_del(cmd);
    } else if (index_one == 4) {

    } else if (index_one == 5 && cmd[1] != NULL) {//done
        command_set(cmd_counter, cmd);
    } else if (index_one == 6 && cmd[1] != NULL) {//done
        command_push(cmd_counter, cmd);
    } else if (index_one == 7 && cmd[1] != NULL) {//done
        command_append(cmd_counter, cmd);
    } else if (index_one == 8 && cmd[1] != NULL && size2 < 3) {//done
        command_pick(cmd);
    } else if (index_one == 9 && cmd[1] != NULL && size2 < 3) {//done
        command_pluck(cmd);
    } else if (index_one == 10 && cmd[1] != NULL && size2 < 2) {//done
        command_pop(cmd);
    } else if (index_one == 11) {

    } else if (index_one == 12) {

    } else if (index_one == 13) {

    } else if (index_one == 14) {
        command_snapshot();
    } else if (index_one == 15 && cmd[1] != NULL && size2 < 2) {//done
        command_min(cmd);
    } else if (index_one == 16 && cmd[1] != NULL && size2 < 2) {//done
        command_max(cmd);
    } else if (index_one == 17 && cmd[1] != NULL && size2 < 2) {//done
        command_sum(cmd);
    } else if (index_one == 18 && cmd[1] != NULL && size2 < 2) {//done
        command_len(cmd);
    } else if (index_one == 19 && cmd[1] != NULL && size2 < 2) {//done
        command_rev(cmd);
    } else if (index_one == 20 && cmd[1] != NULL && size2 < 2) {
//        command_uniq(current_snapshot, cmd);
    } else if (index_one == 21 && cmd[1] != NULL && size2 < 2) {//done
        command_sort(cmd);
    } else if (index_one == 22) {

    } else if (index_one == 23) {

    } else if (index_one == 24) {

    } else if (index_one == 25 && (index_two == 0 || index_two == 1 || index_two == 2)) {
        if (index_two == 0) command_listkeys();//done
        if (index_two == 1) command_listentries();//done
        if (index_two == 2) command_listsnapshots();
        else return;
    } else {
        return;
    }
}

/*
 * Main function that handles the user input by breaking the string up into its components.
 */
int main(void) {
    char line[MAX_LINE];
    while (true) {
        printf("> ");
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            command_bye();
            return 0;
        }

        char *cmd[MAX_COMMAND];
        int i = 0;
        char *new_line = strtok(line, "\n");
        cmd[i] = strtok(new_line, " ");

        while (cmd[i] != NULL) {
            i++;
            cmd[i] = strtok(NULL, " ");

        }
        int cmd_counter = i;
        command_compare(cmd_counter, cmd);
    }
    return EXIT_SUCCESS;
}