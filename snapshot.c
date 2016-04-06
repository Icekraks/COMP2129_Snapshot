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

entry* entry_head = NULL;
entry* entry_tail = NULL;

snapshot* snapshot_head = NULL;
snapshot* snapshot_tail = NULL;

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

void command_bye () {
    printf("bye\n");
}

void command_help() {
    printf("%s\n", HELP);
}

int main(void) {

	char line[MAX_LINE];

	while (true) {
    	printf("> ");

    	if (fgets(line, MAX_LINE, stdin) == NULL) {
    	    command_bye();
    	    return 0; 		
    	}
    	
	    //
	    // TODO
	    //
  	}

	return 0;
}