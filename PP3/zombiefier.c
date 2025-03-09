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
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void sigcont_handler(int signo) {
    continue_execution = 1;
}

int main(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "-n") != 0) {
        fprintf(stderr, "Usage: %s -n <num_zombies>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    int n = atoi(argv[2]);
    if (n <= 0) {
        fprintf(stderr, "Please provide a positive integer for the number of zombies.\n");
        return EXIT_FAILURE;
    }

}
