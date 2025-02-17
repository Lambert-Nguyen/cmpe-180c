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
                num_count = (strlen(argv[i]) > 2) ? extract_value(argv[i]) : ((i + 1 < argc) && (argv[i+1][0] != '-')) ? atoi(argv[++i]) : -1;
                printf("num_count = %d\n", num_count);
                if (num_count == -1) {fprintf(stderr, "Error: Missing VALUE for -n\n"); return 1;}
                break;
            case 'r':
                if(flag_r) {fprintf(stderr, "Error: Duplicate -r flag detected!\n"); return 1;}
                flag_r = true;
                max_num = (strlen(argv[i]) > 2) ? extract_value(argv[i]) : (i + 1 < argc && argv[i + 1][0] != '-') ? atoi(argv[++i]) : -1;
                if (max_num == -1) {fprintf(stderr, "Error: Missing VALUE for -r\n"); return 1;}
                break;
            case 'p':
                if(flag_p) {fprintf(stderr, "Error: Duplicate -p flag detected!\n"); return 1;}
                flag_p = true;
                max_powerball = (strlen(argv[i]) > 2) ? extract_value(argv[i]) : (i + 1 < argc && argv[i + 1][0] != '-') ? atoi(argv[++i]) : -1;
                if (max_powerball == -1) {fprintf(stderr, "Error: Missing VALUE for -p\n"); return 1;}
                break;
            case 'N':
                if(flag_N) {fprintf(stderr, "Error: Duplicate -N flag detected!\n"); return 1;}
                flag_N = true;
                num_sets = (strlen(argv[i]) > 2) ? extract_value(argv[i]) : (i + 1 < argc && argv[i + 1][0] != '-') ? atoi(argv[++i]) : -1;
                if (num_sets == -1) {fprintf(stderr, "Error: Missing VALUE for -N\n"); return 1;}
                break;
                        
            default:
            fprintf(stderr,"Error: Unrecognized flag: %s\n",argv[i]);
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