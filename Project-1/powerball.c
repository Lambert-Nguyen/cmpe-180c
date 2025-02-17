#include <stdio.h>
#include <stdlib.h>
#include 

int main(int argc, char *argv[]){
    printf("argc = %d\n", argc);
    printf("argv[0] - %s\n", argv[0]);
    if (argc < 5) {
        fprintf(stderr, "Usage: %s -n NumbersToGenerate -r MaxNumber [-p MaxPowerBallNumber] -N NumberSetsToGenerate\n", argv[0]);
        return 1;
    }

    int num_count = -1;
    int max_num = -1;
    int max_powerball =0;
    int num_sets = -1;

    for (int i = 0; i < argc; i++){
        if (argv[i][0] == '-'){
            switch (argv[i][1])
            {
            case 'n':
                printf("case 'n'\n");
                break;
            case 'r':
                printf("case 'r'\n");
                break;
            case 'p':
                printf("case 'p'\n");
                break;
            case 'N':
                printf("case N\n");
                break;
            
            
            default:
                printf("case default\n");
                return 1;
            }
        }
        else {
            fprintf(stderr,"Error: Unexpected Argument: %s\n",argv[i]);
            return 1;
        }
    }

    if(num_count <= 0 || max_num =< 0 || num_sets <= 0){
        fprintf(stderr, "Error: Invalid Input. -n, -r or -N value are not properly set.\n");
        return 1;

    }

    if(max_num < num_count){
        fprintf(stderr, "Error: The numbers to be generated (-n %d) cant not be larger than the maximum number rage (-r %d)\n", num_count, max_num);
        return 1;
    }

    return 0;
}