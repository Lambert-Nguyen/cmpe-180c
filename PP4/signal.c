/*
 * toggle_debug.c
 *
 * This program runs an infinite loop sleeping for 2 seconds per iteration.
 * - At startup, nothing is printed.
 * - When SIGINT is received, it toggles a debug flag.
 *   * If debug is on, the program prints a message with the current iteration count on each cycle.
 *   * If debug is off, printing is suppressed.
 * - When SIGUSR1 is received, the program terminates gracefully.
 *
 * Compile with: gcc -Wall -o toggle_debug toggle_debug.c
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <signal.h>
 
 /* Use volatile sig_atomic_t for variables accessed in signal handlers */
 volatile sig_atomic_t debug_mode = 0;   // 0 = off, 1 = on
 volatile sig_atomic_t exit_flag = 0;    // Flag to terminate loop
 volatile sig_atomic_t iteration = 0;    // Counts the number of iterations
 
 /* SIGINT handler toggles debug mode */
 void handle_sigint(int sig) {
     /* Toggle the debug flag */
     debug_mode = !debug_mode;
     
     /* Print an immediate message indicating new state */
     if(debug_mode) {
         printf("Debug mode ON. Iteration: %d\n", iteration);
     } else {
         printf("Debug mode OFF.\n");
     }
     fflush(stdout);
 }
 
 /* SIGUSR1 handler sets exit_flag to terminate the loop */
 void handle_sigusr1(int sig) {
     exit_flag = 1;
 }
 
 int main(void) {
     struct sigaction sa_int, sa_usr1;
     
     /* Setup SIGINT handler */
     sa_int.sa_handler = handle_sigint;
     sigemptyset(&sa_int.sa_mask);
     sa_int.sa_flags = 0;   // No flags needed
     if (sigaction(SIGINT, &sa_int, NULL) == -1) {
         perror("sigaction(SIGINT)");
         exit(EXIT_FAILURE);
     }
     
     /* Setup SIGUSR1 handler */
     sa_usr1.sa_handler = handle_sigusr1;
     sigemptyset(&sa_usr1.sa_mask);
     sa_usr1.sa_flags = 0;
     if (sigaction(SIGUSR1, &sa_usr1, NULL) == -1) {
         perror("sigaction(SIGUSR1)");
         exit(EXIT_FAILURE);
     }
     
     /* Infinite loop: sleep for 2 seconds on each iteration */
     while (!exit_flag) {
         sleep(2);
         iteration++;
         if (debug_mode) {
             printf("Iteration: %d\n", iteration);
             fflush(stdout);
         }
     }
     
     /* Exit gracefully */
     return 0;
 }
 