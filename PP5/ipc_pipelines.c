#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MSG_SIZE 100

int main(void) {
    int pipe1[2]; // for parent -> child communication
    int pipe2[2]; // for child -> parent communication

    // Create the two pipes.
    if (pipe(pipe1) == -1) {
        perror("pipe1");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipe2) == -1) {
        perror("pipe2");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {  // Child process
        // Close unused ends.
        close(pipe1[1]); // Close write end of pipe1 (parent->child)
        close(pipe2[0]); // Close read end of pipe2 (child->parent)

        char buffer[MSG_SIZE];

        // Read message from parent.
        ssize_t bytesRead = read(pipe1[0], buffer, MSG_SIZE - 1);
        if (bytesRead < 0) {
            perror("child read");
            exit(EXIT_FAILURE);
        }
        buffer[bytesRead] = '\0';
        // Print parent's message to stdout.
        printf("Child received: %s", buffer);

        // Prepare reply: "Daddy, my name is <child pid>"
        char reply[MSG_SIZE];
        snprintf(reply, MSG_SIZE, "Daddy, my name is %d\n", getpid());

        // Write reply to parent.
        if (write(pipe2[1], reply, strlen(reply)) < 0) {
            perror("child write");
            exit(EXIT_FAILURE);
        }

        // Close the remaining file descriptors.
        close(pipe1[0]);
        close(pipe2[1]);

        exit(EXIT_SUCCESS);
    }
    else {  // Parent process
        // Close unused ends.
        close(pipe1[0]); // Close read end of pipe1 (parent->child)
        close(pipe2[1]); // Close write end of pipe2 (child->parent)

        // Prepare message: "I am your daddy! and my name is <parent pid>\n"
        char message[MSG_SIZE];
        snprintf(message, MSG_SIZE, "I am your daddy! and my name is %d\n", getpid());

        // Write the message to the child.
        if (write(pipe1[1], message, strlen(message)) < 0) {
            perror("parent write");
            exit(EXIT_FAILURE);
        }

        // Close write end after sending.
        close(pipe1[1]);

        // Block on reading child's reply.
        char reply[MSG_SIZE];
        ssize_t bytesRead = read(pipe2[0], reply, MSG_SIZE - 1);
        if (bytesRead < 0) {
            perror("parent read");
            exit(EXIT_FAILURE);
        }
        reply[bytesRead] = '\0';
        // Print child's reply to stdout.
        printf("Parent received: %s", reply);

        // Close read end.
        close(pipe2[0]);

        // Wait for child to exit to avoid zombies.
        wait(NULL);
    }
    return 0;
}
