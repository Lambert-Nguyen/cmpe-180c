#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();  
    printf("Process ID: %d, Parent ID: %d, pid=%d (After first fork)\n", getpid(), getppid(), pid);

    if (fork()) {  
        printf("Process ID: %d, Parent ID: %d (Inside if after second fork)\n", getpid(), getppid());
        fork();  
        printf("Process ID: %d, Parent ID: %d (After third fork)\n", getpid(), getppid());
    }

    if (pid) {  
        fork();
        printf("Process ID: %d, Parent ID: %d (After fourth fork, pid=%d)\n", getpid(), getppid(), pid);
    }

    sleep(1);
    return 0;
}