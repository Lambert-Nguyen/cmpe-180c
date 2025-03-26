#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

// Global variables to track the iteration and debug state
volatile sig_atomic_t debug_mode = 0;
volatile sig_atomic_t running = 1;

void handle_signal(int sig) {
    if (sig == SIGINT) {
        debug_mode = !debug_mode; // Toggle debug mode
    } else if (sig == SIGUSR1) {
        running = 0; // Exit the loop and terminate
    }
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sa.sa_flags = 0; // Use default flags
    sigemptyset(&sa.sa_mask);

    // Set up signal handlers
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);

    int iteration = 0;

    while (running) {
        sleep(2); // Sleep for 2 seconds
        iteration++;

        if (debug_mode) {
            printf("Iteration %d\n", iteration);
        }
    }

    printf("Program terminated gracefully.\n");
    return 0;
}