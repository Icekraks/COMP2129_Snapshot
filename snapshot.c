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

void static free_values(entry *key) {
    free(key->values);
    key->values = NULL;
}

static entry *find_key(snapshot *current_snapshot, char *key) {
    entry *curr = current_snapshot->entries;
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

static entry *make_or_find_key(snapshot *current_snapshot, char *key) {
    entry *curr = current_snapshot->entries;
    entry *found = NULL;

    while (curr != NULL && found == NULL) {
        int result = strcmp(curr->key, key);
        if (result == 0) {
            found = curr;
            return found;
        }
        curr = curr->next;
    }
    if (found == NULL) {
        found = malloc(sizeof(entry));
        strcpy(found->key, key);
        found->prev = NULL;
        found->next = NULL;

        entry *temp = current_snapshot->entries;
        found->next = temp;
        if (temp != NULL) {
            temp->prev = found;
        }
        current_snapshot->entries = found;
    }
    return found;
}

void command_bye(snapshot *current_snapshot) {
//    entry *current_snap = current_snapshot->entries;
//    current_snap->values;

    printf("bye\n");
}

void command_help() {
    printf("%s\n", HELP);
}

void command_listkeys(snapshot *current_snapshot) {
    entry *temp = current_snapshot->entries;
    if (temp == NULL) {
        printf("no keys\n");
    } else {
        while (temp != NULL) {
            printf("%s\n", temp->key);
            temp = temp->next;
        }
    }
}

void command_listentries(snapshot *current_snapshot) {
    if (entry_head == NULL) {
        printf("no entries\n");
        return;
    }
    entry *temp = current_snapshot->entries;
    while (temp->next != NULL) {
        printf("%s ", temp->key);
        printf("[");
        int size = sizeof(temp->values) / sizeof(temp->values[0]);
        for (int i = 0; i < size; i++) {
            printf("%d ", temp->values[i]);
        }
        printf("]");
        if (temp->next == NULL) {
            return;
        }
        temp = temp->next;
    }
}

void command_listsnapshots(snapshot *current_snapshot) {
    if (snapshot_head == NULL) {
        printf("no snapshots\n");
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
}

void command_get(snapshot *current_snapshot, char **cmd) {
    entry *key = find_key(current_snapshot, cmd[1]);
    if (key == NULL) {
        printf("no key\n");
        return;
    }
    int *values = key->values;
    printf("[");
    for (int i = 0; i < key->length; i++) {
        printf("%d", values[i]);
        if (i < key->length - 1) {
            printf(" ");
        }
    }
    printf("]\n");

}

void command_del(snapshot *current_snapshot, char **cmd) {
    entry *key = find_key(current_snapshot, cmd[1]);
    if (key == NULL) {
        printf("no such key");
        return;
    }
    free_values(key);
    entry *prev = key->prev;
    entry *next = key->next;
    if (next != NULL) {
        next->prev = prev;
    }
    if (prev != NULL) {
        prev->next = next;
    }

    if (current_snapshot->entries == key) {
        current_snapshot->entries = key->next;
    }
    free(key);
    printf("ok\n");
}

void command_purge() {
    printf("ok\n");
}

/*
 * Method handles the set command when the user wants to set values to a key, the key can either
 * pre-exist or be brand new.
 */
void command_set(snapshot *current_snapshot, int cmd_counter, char **cmd) {

    entry *new_entry = make_or_find_key(current_snapshot, cmd[1]);
    free_values(new_entry);
    new_entry->values = (int *) malloc((cmd_counter - 2) * sizeof(int));
    for (int i = 2; i < cmd_counter; i++) {
        new_entry->values[i - 2] = atoi(cmd[i]);
    }
    new_entry->length = cmd_counter - 2;

    printf("ok\n");
}


void command_push(snapshot *current_snapshot, int cmd_counter, char **cmd) {
    entry *current_entry = find_key(current_snapshot, cmd[1]);
    if (current_entry == NULL) {
        printf("no such key\n");
        return;
    }
    int old_len = current_entry->length;
    int current_value[current_entry->length];
    for (int i = 0; i < current_entry->length; i++) {
        current_value[i] = current_entry->values[i];
    }
    int new_len = cmd_counter - 2;
    current_entry->values = realloc(current_entry->values, sizeof(int) * (new_len + old_len));

    int holding_array[new_len];
    for (int x = 0; x < new_len; x++) {
        holding_array[(new_len-1)-x] = atoi(cmd[x + 2]);
    }
    for (int z = 0; z < (old_len + new_len); z++) {
        current_entry->values[z] = holding_array[z];
    }
    for (int z = 0; z < (old_len + new_len); z++) {
        current_entry->values[old_len + z] = current_value[z];
    }


    current_entry->length = (new_len + old_len);

    printf("ok\n");
}

/*
 * Method handles the user appending(adding to the end), the assumption is that there is a entry in the linked list.
 * the values are appended or added to the end of the values for the entry.
 */

void command_append(snapshot *current_snapshot, int cmd_counter, char **cmd) {
    entry *current_entry = find_key(current_snapshot, cmd[1]);
    if (current_entry == NULL) {
        printf("no such key\n");
        return;
    }
    int old_len = current_entry->length;
    int current_value[current_entry->length];
    for (int i = 0; i < current_entry->length; i++) {
        current_value[i] = current_entry->values[i];
    }
    int new_len = cmd_counter - 2;
    current_entry->values = realloc(current_entry->values, sizeof(int) * (new_len + old_len));
    for (int j = 0; j < old_len; j++) {
        current_entry->values[j] = current_value[j];
        printf("%d", current_entry->values[j]);
    }
    int holding_array[new_len];
    for (int x = 0; x < new_len; x++) {
        holding_array[x] = atoi(cmd[x + 2]);
    }

    for (int z = 0; z < (old_len + new_len); z++) {
        current_entry->values[old_len + z] = holding_array[z];
    }
    current_entry->length = (new_len + old_len);

    printf("ok\n");
}

/*
 * This method handles the picking of a specific value and displays it.
 */
void command_pick(snapshot *current_snapshot, char **cmd) {
    entry *current_entry = find_key(current_snapshot, cmd[1]);
    if (current_entry == NULL) {
        printf("no such key\n");
        return;
    }
    int index = atoi(cmd[2]) - 1;
    if (index < 0 || index >= current_entry->length) {
        printf("index out of range\n");
        return;
    }
    int value = current_entry->values[index];
    printf("%d\n", value);
}

void command_pluck(snapshot *current_snapshot, char **cmd) {
    entry *current_entry = find_key(current_snapshot, cmd[1]);
    if (current_entry == NULL) {
        printf("no such key\n");
        return;
    }
    int index = atoi(cmd[2]) - 1;
    if (index < 0 || index >= current_entry->length) {
        printf("index out of range\n");
        return;
    }
    int value = current_entry->values[index];
    printf("%d\n", value);
    int holding_array[current_entry->length];
    for (int i = 0; i < current_entry->length; i++) {
        holding_array[i] = current_entry->values[i];
    }


    for (int i = index; i < current_entry->length - 1; i++) {
        current_entry->values[i] = current_entry->values[i + 1];
    }
    current_entry->length = current_entry->length - 1;
}

void command_pop(snapshot * current_snapshot, char **cmd) {
    entry *current_entry = find_key(current_snapshot, cmd[1]);
    if (current_entry == NULL) {
        printf("no such key\n");
        return;
    }
    int index = 0;
    if (index < 0 || index >= current_entry->length) {
        printf("index out of range\n");
        return;
    }
    int value = current_entry->values[index];
    printf("%d\n", value);
    int holding_array[current_entry->length];
    for (int i = 0; i < current_entry->length; i++) {
        holding_array[i] = current_entry->values[i];
    }


    for (int i = index; i < current_entry->length - 1; i++) {
        current_entry->values[i] = current_entry->values[i + 1];
    }
    current_entry->length = current_entry->length - 1;
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
    printf("ok\n");
}

void command_uniq() {
    printf("ok\n");
}

void command_sort() {
    //use qSort()
    printf("ok\n");
}

void command_diff() {

}

void command_inter() {

}

void command_union() {

}

void command_compare(snapshot *current_snapshot, int cmd_counter, char **cmd) {
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
    int size2 = sizeof(cmd) / sizeof(cmd[0]);
    /*
     * All these else if statements handles all the commands on large scale comparing what the index is to the
     * predefined indexes of the command array.
     */
    if (index_one == 0) {
        command_bye(current_snapshot);
        exit(0);
    } else if (index_one == 1) {
        command_help();
    } else if (index_one == 2) {//TODO: done
        command_get(current_snapshot, cmd);
    } else if (index_one == 3) {//TODO: done
        command_del(current_snapshot, cmd);
    } else if (index_one == 4) {

    } else if (index_one == 5) {//TODO: done
        command_set(current_snapshot, cmd_counter, cmd);
    } else if (index_one == 6) {//TODO: done
        command_push(current_snapshot, cmd_counter, cmd);
    } else if (index_one == 7) {//TODO: done
        command_append(current_snapshot, cmd_counter, cmd);
    } else if (index_one == 8) {//TODO: done
        if (size2 < 3) {
            command_pick(current_snapshot, cmd);
        }
    } else if (index_one == 9) {//TODO: done
        if (size2 < 3) {
            command_pluck(current_snapshot, cmd);
        }
    } else if (index_one == 10) {//TODO: done
        if(size2 < 2){
            command_pop(current_snapshot, cmd);
        }
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
        if (index_two == 0) command_listkeys(current_snapshot);
        if (index_two == 1) command_listentries(current_snapshot);
        if (index_two == 2) command_listsnapshots(current_snapshot);
        else return;
    } else {
        return;
    }
}

int main(void) {
    /*
     * Main function that handles the user input by breaking the string up into its components.
     */
    char line[MAX_LINE];
    snapshot *current_snap = malloc(sizeof(snapshot));
    current_snap->entries = NULL;
    current_snap->next = NULL;
    current_snap->prev = NULL;

    while (true) {
        printf("> ");
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            command_bye(current_snap);
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
        command_compare(current_snap, cmd_counter, cmd);
    }
    free(current_snap);
    return EXIT_SUCCESS;
}
