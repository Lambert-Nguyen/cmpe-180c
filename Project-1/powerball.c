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
    if (argc != 7 && argc != 9) {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  3 pairs: %s -r MaxNum -n NumGen -N NumSets\n", argv[0]);
        fprintf(stderr, "  4 pairs: %s -r MaxNum -n NumGen -p PowerBall -N NumSets\n", argv[0]);
        fprintf(stderr, "OR number/flag style (but not mixed)\n");
        return 1;
    }

    int num_count = -1;
    int max_num = -1;
    int max_powerball =0;
    int num_sets = -1;
    bool flag_n = false, flag_r = false, flag_p = false, flag_N = false;

    /*mode = 0 => (flag,number), mode = 1 => (number,flag) format*/
    int mode = -1;

    for (int i = 1; i < argc; i+=2){
        char *arg1= argv[i];
        char *arg2= argv[i+1];

        bool is_arg1_flag = (arg1[0] == '-');
        bool is_arg2_flag = (arg2[0] == '-');

        if(mode == -1){
            if (is_arg1_flag && !is_arg2_flag){
                mode = 0; /*mode 0 (flag,number)*/
            } else if (!is_arg1_flag && is_arg2_flag){
                mode = 1; /* mode 1 (number,flag)*/
            } else {fprintf(stderr,"Error: Invalid input pair %s %s. Must be either (flag,number) or (number,flag) format\n", arg1, arg2); return 1;}
        }

        if(mode == 0){
            if(!is_arg1_flag || is_arg2_flag){
                fprintf(stderr, "Error: Mixed format detected in pair '%s' '%s'. All pairs must match the first pair's style.\n", arg1, arg2);
                return 1;
            }
            char flag = arg1[1];
            int value = atoi(arg2);

            switch (flag){
                case 'r':
                    if (flag_r) { fprintf(stderr, "Error: Duplicate -r flag.\n"); return 1;}
                    flag_r = true;
                    max_num = value;
                    break;
                case 'n':
                    if (flag_n) { fprintf(stderr, "Error: Duplicate -n flag.\n"); return 1;}
                    flag_n = true;
                    num_count = value;
                    break;
                case 'p':
                    if (flag_p) { fprintf(stderr, "Error: Duplicate -p flag.\n"); return 1;}
                    flag_p = true;
                    max_powerball = value;
                    break;
                case 'N':
                    if (flag_N) { fprintf(stderr, "Error: Duplicate -N flag.\n"); return 1;}
                    flag_N = true;
                    num_sets = value;
                    break;
                default:
                    fprintf(stderr, "Error: Unrecognized flag '%s'.\n", arg1);
                    return 1;
            }
        } else {
            /*mode == 1*/
            if (is_arg1_flag || !is_arg2_flag) {
                fprintf(stderr, "Error: Mixed format detected in pair '%s' '%s'. All pairs must match the first pair's style.\n", arg1, arg2);
                return 1;
            }
            /*( "49", "-r" )*/
            int value = atoi(arg1);
            char flag = arg2[1];

            switch (flag) {
                case 'r':
                    if (flag_r) {fprintf(stderr, "Error: Duplicate -r flag.\n"); return 1;}
                    flag_r = true;
                    max_num = value;
                    break;
                case 'n':
                    if (flag_n) {fprintf(stderr, "Error: Duplicate -n flag.\n"); return 1;}
                    flag_n = true;
                    num_count = value;
                    break;
                case 'p':
                    if (flag_p) {fprintf(stderr, "Error: Duplicate -p flag.\n"); return 1;}
                    flag_p = true;
                    max_powerball = value;
                    break;
                case 'N':
                    if (flag_N) {fprintf(stderr, "Error: Duplicate -N flag.\n"); return 1;}
                    flag_N = true;
                    num_sets = value;
                    break;
                default:
                    fprintf(stderr, "Error: Unrecognized flag '%s'.\n", arg2);
                    return 1;
            }
        }
    }





    //     if (argv[i][0] == '-'){
    //         switch (argv[i][1])
    //         {
    //         case 'n':
    //             if(flag_n) {fprintf(stderr, "Error: Duplicate -n flag detected!\n"); return 1;}
    //             flag_n = true;
    //             num_count = (strlen(argv[i]) > 2) ? extract_value(argv[i]) : ((i + 1 < argc) && (argv[i+1][0] != '-')) ? atoi(argv[++i]) : -1;
    //             printf("num_count = %d\n", num_count);
    //             if (num_count == -1) {fprintf(stderr, "Error: Missing VALUE for -n\n"); return 1;}
    //             break;
    //         case 'r':
    //             if(flag_r) {fprintf(stderr, "Error: Duplicate -r flag detected!\n"); return 1;}
    //             flag_r = true;
    //             max_num = (strlen(argv[i]) > 2) ? extract_value(argv[i]) : (i + 1 < argc && argv[i+1][0] != '-') ? atoi(argv[++i]) : -1;
    //             if (max_num == -1) {fprintf(stderr, "Error: Missing VALUE for -r\n"); return 1;}
    //             break;
    //         case 'p':
    //             if(flag_p) {fprintf(stderr, "Error: Duplicate -p flag detected!\n"); return 1;}
    //             flag_p = true;
    //             max_powerball = (strlen(argv[i]) > 2) ? extract_value(argv[i]) : (i + 1 < argc && argv[i+1][0] != '-') ? atoi(argv[++i]) : -1;
    //             if (max_powerball == -1) {fprintf(stderr, "Error: Missing VALUE for -p\n"); return 1;}
    //             break;
    //         case 'N':
    //             if(flag_N) {fprintf(stderr, "Error: Duplicate -N flag detected!\n"); return 1;}
    //             flag_N = true;
    //             num_sets = (strlen(argv[i]) > 2) ? extract_value(argv[i]) : (i + 1 < argc && argv[i+1][0] != '-') ? atoi(argv[++i]) : -1;
    //             if (num_sets == -1) {fprintf(stderr, "Error: Missing VALUE for -N\n"); return 1;}
    //             break;
                        
    //         default:
    //         fprintf(stderr,"Error: Unrecognized flag: %s\n",argv[i]);
    //             return 1;
    //         }
    //     }
    //     else {
    //         fprintf(stderr,"Error: Unexpected Argument: %s\n",argv[i]);
    //         return 1;
    //     }
    // }

    if (!flag_r || !flag_n || !flag_N) {
        fprintf(stderr, "Error: Missing required flags: -r, -n, or -N.\n");
        return 1;
    }
    if (max_num <= 0 || num_count <= 0 || num_sets <= 0) {
        fprintf(stderr, "Error: Invalid or missing values for -r, -n, or -N.\n");
        return 1;
    }
    if (num_count > max_num) {
        fprintf(stderr, "Error: The numbers to be generated (-n %d) cannot exceed the maximum (-r %d).\n", num_count, max_num);
        return 1;
    }

    // If we get here, everything is good
    printf("Parsed values:\n");
    printf("  -r (max_num)       = %d\n", max_num);
    printf("  -n (num_count)     = %d\n", num_count);
    printf("  -p (max_powerball) = %d\n", max_powerball);
    printf("  -N (num_sets)      = %d\n", num_sets);

    fprintf(stdout,"Parsed values: -n %d, -r %d, -p %d, -N %d\n", num_count, max_num, max_powerball, num_sets);

    return 0;
}