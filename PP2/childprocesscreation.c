#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;
    pid = fork();

    if(pid == -1){
        perror("Failed to fork()\n");
    }
    else if (pid == 0){
        printf("Hello from child process pid = %d\n", pid);
    }
    else{
        printf("Hello from parent process pid = %d\n", pid);

    }
    return 0;
}
