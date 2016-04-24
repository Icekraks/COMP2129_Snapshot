#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(){
	int values[14] = {1,2,3,4,3,4,5,6,6,7,6,6,6,6};
	int len = 14;
	int new_len = 14;
	int num_duplicates = 1;
	int update_values[len];
	for(int i = 0;i<len;i++){
		printf("%d ",values[i]);
	}
	printf("\n");
	update_values[0] = values[0];
	for (int j = 1; j < len; j++) {
		if (values[j] == update_values[num_duplicates-1]) {
			new_len--;
			continue;
		}
		else {
			update_values[num_duplicates] = values[j];
			num_duplicates++;
		}
	}


	//Print Statement
	for(int i = 0;i<new_len;i++){
		printf("%d ",update_values[i]);
	}

}
