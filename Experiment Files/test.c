#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(){
	char *commands[3] = {
"KEYS", "ENTRIES", "SNAPSHOT"

	};
	int numbers[5] = {1,2,3,4,5};
	int test_array[3]={1, 2, 3};
	int number = sizeof(test_array);
	int number_div = number/sizeof(test_array[0]);
	printf("%d %d\n",number,number_div);
	int size = sizeof(commands);
	int z = sizeof(commands);
	int f = sizeof(commands[0]);
	int size2 = z/f;
	printf("%d %d\n",size,size2);
	char *test ="keys";
	int x,y;
  x = strcmp(commands[0],test);
	y = strcasecmp(commands[0],test);
	for(int i = 0;i<5;i++){
				printf("%d\n",numbers[i]);
		}
	int backwards[5];
	for(int i = 0;i<5;i++){
        backwards[i]=numbers[4-i];
    }
		for(int i = 0;i<5;i++){
	        printf("%d\n",backwards[i]);
	    }

	printf("%d %d",x,y);
}
