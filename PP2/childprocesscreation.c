/*Write a “C” program that creates a child process that prints out its pid and new line and then calls pause() system call.
Once it returns from the pause system call the child program must exit with exit code 5.
The parent program must wait for the child process and print out the pid of the child process and the exit status with the following format: 
“childpid=%d,exitstatus=%d\n”. Do not print anything else to stdout.
Hint: make sure that the child handles SIGINT signal. Make sure that the exit code of the child process is 5 and not any other number.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

void sigint_handler(int sig) {
    // fprintf(stdout, "DO SOMETHING\n");

}

int main() {
    pid_t pid;
    int status;
    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Error: fork() failed - %s\n", strerror(errno));
        exit(1);
    } 
    
    else if (pid == 0) {
        fprintf(stdout, "%d\n", getpid());
        if (signal(SIGINT, sigint_handler) == SIG_ERR){
            fprintf(stderr, "Error: signal() failed in child process- %s\n", strerror(errno));
            exit(1);
        }
        pause();
        exit(5);
    } 
    else {
        if (signal(SIGINT, SIG_IGN) == SIG_ERR){
            fprintf(stderr, "Error: signal() failed in parent - %s\n", strerror(errno));
            exit(1);
        }

        if (wait(&status) == -1){
            fprintf(stderr, "Error: wait() failed - %s\n", strerror(errno));
            exit(1);
        }

        // Restore default SIGINT behavior
        signal(SIGINT, SIG_DFL);
        if (WIFEXITED(status)) {
            fprintf(stdout, "childpid=%d,exitstatus=%d\n", pid, WEXITSTATUS(status));
        }
    }

    return 0;
}

