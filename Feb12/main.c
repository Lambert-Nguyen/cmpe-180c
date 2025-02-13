
#include <stdlib.h> /* need for atoi() stdc library funct. */
#include <stdio.h> /* need for printf() stdc library*/

#include <sys/stat.h>
#include <errno.h>
#include <string.h>

int factorial(int n) {
    int f=1;
    for (int i = 1; i <= n; i++) {
        f = f * i;
    }
    return f;
}

int main(int argc, char *argv[]) {
    printf("argc = %d\n", argc);
    printf("argv[0] - %s\n", argv[0]);
    if (argc < 2) {
        fprintf(stderr, "Argument count should be 2 or larger\n");
        return 1;
    }
    int n = atoi(argv[1]);
    fprintf(stdout,"The factorial(%d) is %d\n",n, factorial(n));
    struct stat s;
    if(stat("ERROR.txt", &s)!= 0) {
        fprintf(stderr, "System call failed! errno=%d, %s \n", errno, strerror(errno));
    }
    
    return 0;
}
