#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){

    int r=0;

    pid_t pid = fork();

    if(pid){

         waitpid(pid, &r, 0);

    } else {

         exit(-100);

    }

    printf("r=%d\n", r);

} 