#include <stdio.h>

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
                break;
            }
        }
    }

    return 0;
}