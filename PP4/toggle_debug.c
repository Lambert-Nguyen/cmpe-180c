#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

volatile sig_atomic_t debug_mode = 0;
volatile sig_atomic_t running = 1;

void handle_signal(int sig) {
    if (sig == SIGINT) {
        debug_mode = !debug_mode; 
    } else if (sig == SIGUSR1) {
        running = 0;
    }
}

int main(void){
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sa.sa_flags = 0;


    if (sigemptyset(&sa.sa_mask) == -1){
        fprintf(stderr, "Error initializing signal mask: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Set up SIGINT handler
    if (sigaction(SIGINT, &sa, NULL) == -1){
        fprintf(stderr, "Error setting up SIGINT handler: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Set up SIGUSR1 handler
    if (sigaction(SIGUSR1, &sa, NULL) == -1){
        fprintf(stderr, "Error setting up SIGUSR1 handler: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int iteration = 0;
    while (running) {
        sleep(2); 
        iteration++;

        if(debug_mode){fprintf(stdout, "Iteration %d\n", iteration);}
    }

    fprintf(stdout, "Program terminated gracefully.\n");
    return 0;
}