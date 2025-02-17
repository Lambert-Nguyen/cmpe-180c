#include <stdio.h>   
#include <stdlib.h>  
#include <stdbool.h>
#include <time.h>    /*For time() */ 
#include <string.h>  /*For strcmp() */ 
#include <ctype.h>   /*For isdigit() (*/ 

int extract_value(char *arg){
    while (*arg && !isdigit(*arg)) arg++;  
    return (*arg) ? atoi(arg) : -1; 
}

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
    bool flag_n = false, flag_r = false, flag_p = false, flag_N = false;

    for (int i = 1; i < argc; i++){
        if (argv[i][0] == '-'){
            switch (argv[i][1])
            {
            case 'n':
                if(flag_n) {fprintf(stderr, "Error: Duplicate -n flag detected!\n"); return 1;}
                flag_n = true;
                printf("case 'n'\n");
                break;
            case 'r':
                if(flag_r) {fprintf(stderr, "Error: Duplicate -r flag detected!\n"); return 1;}
                flag_r = true;
                printf("case 'r'\n");
                break;
            case 'p':
                if(flag_p) {fprintf(stderr, "Error: Duplicate -p flag detected!\n"); return 1;}
                flag_p = true;
                printf("case 'p'\n");
                break;
            case 'N':
                if(flag_N) {fprintf(stderr, "Error: Duplicate -N flag detected!\n"); return 1;}
                flag_N = true;
                printf("case N\n");
                break;
            
            
            default:
            fprintf(stderr,"Error: Unrecognized Argument: %s\n",argv[i]);
                return 1;
            }
        }
        else {
            fprintf(stderr,"Error: Unexpected Argument: %s\n",argv[i]);
            return 1;
        }
    }

    if(num_count <= 0 || max_num <= 0 || num_sets <= 0){
        fprintf(stderr, "Error: Invalid Input. -n, -r or -N value are not properly set.\n");
        return 1;

    }

    if(max_num < num_count){
        fprintf(stderr, "Error: The numbers to be generated (-n %d) cant not be larger than the maximum number rage (-r %d)\n", num_count, max_num);
        return 1;
    }

    fprintf(stdout,"Parsed values: -n %d, -r %d, -p %d, -N %d\n", num_count, max_num, max_powerball, num_sets);

    return 0;
}