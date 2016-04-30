gcc -std=c11 -g -Werror -Wall snapshot.c -o snapshot
valgrind --leak-check=full --show-leak-kinds=all ./snapshot < basic-snapshot.in
