#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#define MSG_SIZE 100

int main(void){
    int pipe1[2]; // Parent-to-child communication
    int pipe2[2]; // Child-to-parent communication
    pid_t pid;
    char buffer[MSG_SIZE];

    /* Create the first pipe (parent -> child) */
    if (pipe(pipe1) == -1) {
        fprintf(stderr, "Error: pipe1 creation failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Create the second pipe (child -> parent) */
    if (pipe(pipe2) == -1) {
        fprintf(stderr, "Error: pipe2 creation failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Fork to create child process */
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Error: fork failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        /***** Parent Process *****/
        /* 
         * Close unused ends:
         * For pipe1, the parent only writes, so close the read end.
         * For pipe2, the parent only reads, so close the write end.
         */
        close(pipe1[0]); 
        close(pipe2[1]);

        /* Prepare a message with parent's PID */
        snprintf(buffer, MSG_SIZE, "I am your daddy! and my name is %d\n", getpid());

        /* Write the message into pipe1 for the child */
        if (write(pipe1[1], buffer, strlen(buffer) + 1) == -1) {
            fprintf(stderr, "Error: write to pipe1 failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        /* Optionally, the parent could block while waiting for the child’s message */
        if (read(pipe2[0], buffer, MSG_SIZE) == -1) {
            fprintf(stderr, "Error: read from pipe2 failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        /* Print the message received from the child to stdout */
        printf("Parent received: %s\n", buffer);

        /* Close the remaining pipe ends */
        close(pipe1[1]);
        close(pipe2[0]);

        /* Wait for child to exit to avoid creating a zombie process */
        wait(NULL);
    } else {
        /***** Child Process *****/
        /* 
         * Close unused ends:
         * For pipe1, the child reads the message so it closes the write end.
         * For pipe2, the child writes its message so it closes the read end.
         */
        close(pipe1[1]);
        close(pipe2[0]);

        /* Read the message sent by the parent from pipe1 */
        if (read(pipe1[0], buffer, MSG_SIZE) == -1) {
            fprintf(stderr, "Error: read from pipe1 failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        /* Print the message to stdout */
        printf("Child received: %s\n", buffer);

        /* Prepare the message with child’s PID */
        snprintf(buffer, MSG_SIZE, "Daddy, my name is %d", getpid());

        /* Write the message into pipe2 for the parent */
        if (write(pipe2[1], buffer, strlen(buffer) + 1) == -1) {
            fprintf(stderr, "Error: write to pipe2 failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        /* Close the remaining pipe ends */
        close(pipe1[0]);
        close(pipe2[1]);

        exit(EXIT_SUCCESS);
    }

    return 0;
}
