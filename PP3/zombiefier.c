/*Write a “C” program that creates a number of zombies (1 through n) processes as specified in the command line arguments to the program: zombiefier –n 10 creates 10 zombies. 
Make sure the program cleans up the zombies upon completion. 
Zombies must remain in the system until a signal (SIGCONT) is sent to zombifier. 
BEWARE of solutions posted on the web that may not fulfill these requirements, if you copy such version you most likely will get zero points. 
Your program must be 100% reliable and create the zombies requested on every run, if it does not, you will receive no points.
Programs that are copies of another will all get zero grade. THIS IS AN INDIVIDUAL ASSIGNMENT as any future homework you may get. 
REMINDER: late homework == 0 grade. Online submissions only. Programs that do not compile will get zero credit, programs that would not run will get zero credit.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

volatile sig_atomic_t continue_execution = 0;

void sigcont_handler(int signo) {
    continue_execution = 1;
}

int main(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "-n") != 0) {
        fprintf(stderr, "Usage: %s -n <num_zombies>\n", argv[0]);
        return 1;
    }
    
    int n = atoi(argv[2]);
    if (n <= 0){
        fprintf(stderr, "ERROR: Please provide a positive integer for the number of zombies.\n");
        return 1;
    }
    pid_t zombie_pids[n]; 

    signal(SIGCONT, sigcont_handler);

    for (int i = 0; i < n; i++){
        pid_t pid = fork();
        if (pid<0){
            fprintf(stderr, "Error: fork() failed - %s\n", strerror(errno));
            return 1;
        }
        if (pid == 0){
            exit(0);  // Child process exits immediately, becoming a zombie
        } else {
            zombie_pids[i] = pid;
        }
    }
    
    fprintf(stdout,"%d zombie processes created. Send SIGCONT to PID %d to clean up.\n", n, getpid());
    
    while(!continue_execution){
        pause(); // Wait for SIGCONT
    }

    fprintf(stdout,"Received SIGCONT, cleaning up zombies...\n");
    for(int i = 0; i<n; i++){
        waitpid(zombie_pids[i], NULL, 0);
    }
    
    fprintf(stdout,"All zombies cleaned up. Exiting.\n");
    return 0;
}
