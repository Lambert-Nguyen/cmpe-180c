#include <stdio.h>

int main(int argc, char *argv[]){
    printf("argc = %d\n", argc);
    printf("argv[0] - %s\n", argv[0]);
    if (argc < 4) {
        fprintf(stderr, "Argument count should be 4 or larger\n");
        return 1;
    }

    return 0;
}