#include <stdio.h>   
#include <stdlib.h>  
#include <stdbool.h>
#include <time.h>    /*For time() */ 
#include <string.h>  /*For strcmp() */ 
#include <ctype.h>   /*For isdigit() (*/ 

int extract_value(const char *arg){
    while (*arg && !isdigit(*arg)) arg++;  
    return (*arg) ? atoi(arg) : -1; 
}

/*Checks if arg is (flag+number) in one arg like "-r49"*/
bool is_combined_flag_number(const char *arg, char *flagOut, int *valueOut) {
    if (arg[0] != '-' || strlen(arg) < 3) {return false;}
    char f = arg[1]; 
    if (f != 'r' && f != 'n' && f != 'p' && f != 'N') {return false;}
    int val = extract_value(arg + 2);
    if (val <= 0) {fprintf(stderr,"ERROR: negative value input is not allowed!\n"); return false;}
    *flagOut = f;
    *valueOut = val;
    return true;
}

/*Checks if arg is (number+flag) in one arg "49-r"*/
bool is_combined_number_flag(const char *arg, char *flagOut, int *valueOut) {
    // find dash
    const char *dashPos = strchr(arg, '-');
    if (!dashPos) {return false; }
    if (dashPos == arg || *(dashPos + 1) == '\0') {return false;}

    
    char f = *(dashPos + 1);
    if (f != 'r' && f != 'n' && f != 'p' && f != 'N') {return false;}

    int lenFront = dashPos - arg;
    if (lenFront <= 0) {return false;}
    char front[50];
    strncpy(front, arg, lenFront);
    front[lenFront] = '\0';
    int val = atoi(front);
    if (val <= 0) {fprintf(stderr,"ERROR: negative value input is not allowed!\n"); return false;}
    *flagOut = f;
    *valueOut = val;
    return true;
}

int is_duplicate(int *arr, int size, int num) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == num) {return 1;}
    }
    return 0;
}

void generate_lottery_numbers(int num_count, int max_num, int max_powerball, int sets){
    int numbers[num_count];  
    int powerball;
    
    for(int s = 0; s < sets; s++) {

        for(int i = 0; i < num_count; i++){
            int num;
            do {num = (rand() % max_num) + 1;} 
            while (is_duplicate(numbers, i, num));
            numbers[i] = num;
        }
        for(int i = 0; i < num_count - 1; i++){
            for (int j = i + 1; j < num_count; j++) {
                if(numbers[i] > numbers[j]){
                    int temp = numbers[i];
                    numbers[i] = numbers[j];
                    numbers[j] = temp;
                }
            }
        }
        for(int i = 0; i < num_count; i++){
            fprintf(stdout,"%d ", numbers[i]);
        }

        if (max_powerball > 0) {
            powerball = (rand() % max_powerball) + 1;
            fprintf(stdout,"%d", powerball);
        }
        fprintf(stdout,"\n");
    }
}

int main(int argc, char *argv[]){
    // printf("argc = %d\n", argc);
    // printf("argv[0] - %s\n", argv[0]);

    if (argc < 2) {
        fprintf(stderr, "Usage: -n NumberToGenerate -r MaxNumber -N NumberSetsToGenerate with optional -p MaxPowerBallNumber.\n");
        fprintf(stderr, "Format should be in pairs (flag, number): \n");
        fprintf(stderr, "  3 pairs: %s -r MaxNum -n NumGen -N NumSets\n", argv[0]);
        fprintf(stderr, "  4 pairs: %s -r MaxNum -n NumGen -p PowerBall -N NumSets\n", argv[0]);
        fprintf(stderr, "OR (number, flag) format style (but not mixed)\n");
        fprintf(stderr, "  3 pairs: %s MaxNum -r NumGen -n NumSets -N \n", argv[0]);
        fprintf(stderr, "  4 pairs: %s MaxNum -r NumGen -n PowerBall -p NumSets -N \n", argv[0]);
        return 1;
    }

    int num_count = -1;
    int max_num = -1;
    int max_powerball =0;
    int num_sets = -1;
    bool flag_n = false, flag_r = false, flag_p = false, flag_N = false;

    /*mode = 0 => (flag,number), mode = 1 => (number,flag) format*/
    int mode = -1;

    int i = 1;
    int pairs_count = 0; 

    while (i < argc) {

        const char *arg = argv[i];

        char f = '\0'; 
        int val = -1;

        /*1. check for (flag,number) in one arg*/ 
        if (is_combined_flag_number(arg, &f, &val)) {
            if (mode == -1) {
                mode = 0; /* (flag, number) style*/
            } else if (mode != 0) {
                fprintf(stderr, "Error: Mixed format. First pair was (number,flag), but now found (flag,number) combined.\n");
                return 1;
            }

            switch (f) {
                case 'r':
                    if (flag_r) { fprintf(stderr, "Error: Duplicate -r.\n"); return 1; }
                    flag_r = true; max_num = val;
                    break;
                case 'n':
                    if (flag_n) { fprintf(stderr, "Error: Duplicate -n.\n"); return 1; }
                    flag_n = true; num_count = val;
                    break;
                case 'p':
                    if (flag_p) { fprintf(stderr, "Error: Duplicate -p.\n"); return 1; }
                    flag_p = true; max_powerball = val;
                    break;
                case 'N':
                    if (flag_N) { fprintf(stderr, "Error: Duplicate -N.\n"); return 1; }
                    flag_N = true; num_sets = val;
                    break;
                default:
                    fprintf(stderr, "Error: Unrecognized flag in '%s'.\n", arg);
                    return 1;
            }
            i++;
            pairs_count++;
        }
        /* 2. check for (number+flag) in one arg */
        else if (is_combined_number_flag(arg, &f, &val)) {
            if (mode == -1) {
                mode = 1; /*(number,flag) format*/ 
            } else if (mode != 1) {
                fprintf(stderr, "Error: Mixed format. First pair was (flag,number), now found (number,flag) in the next arg.\n");
                return 1;
            }
            switch (f) {
                case 'r':
                    if (flag_r) {fprintf(stderr, "Error: Duplicate -r.\n"); return 1;}
                    flag_r = true; max_num = val;
                    break;
                case 'n':
                    if (flag_n) {fprintf(stderr, "Error: Duplicate -n.\n"); return 1;}
                    flag_n = true; num_count = val;
                    break;
                case 'p':
                    if (flag_p) {fprintf(stderr, "Error: Duplicate -p.\n"); return 1;}
                    flag_p = true; max_powerball = val;
                    break;
                case 'N':
                    if (flag_N) {fprintf(stderr, "Error: Duplicate -N.\n"); return 1;}
                    flag_N = true; num_sets = val;
                    break;
                default:
                    fprintf(stderr, "Error: Unrecognized flag in '%s'.\n", arg);
                    return 1;
            }
            i++;
            pairs_count++;
        }
        else {
            /*not combined arg, normal case*/
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: '%s' has no partner.\n", arg);
                return 1;
            }
            const char *arg2 = argv[i+1];

            bool arg1_is_flag = (arg[0] == '-');
            bool arg2_is_flag = (arg2[0] == '-');

            if (mode == -1) {
                if (arg1_is_flag && !arg2_is_flag) {
                    mode = 0; /*(flag, number)*/ 
                } else if (!arg1_is_flag && arg2_is_flag) {
                    mode = 1; /*(number, flag)*/ 
                } else {
                    fprintf(stderr, "Error: Invalid first pair '%s' '%s'.\n", arg, arg2);
                    return 1;
                }
            }

            if (mode == 0) {
                /*mode 0 -> (flag,number) format*/
                if (!arg1_is_flag || arg2_is_flag) {
                    fprintf(stderr,"Error: Mixed format in pair '%s' '%s'.\n", arg, arg2);
                    return 1;
                }
                char ff = arg[1]; 
                int vv = atoi(arg2);
                switch (ff) {
                    case 'r':
                        if (flag_r) { fprintf(stderr,"Duplicate -r!\n"); return 1; }
                        flag_r = true; max_num = vv; break;
                    case 'n':
                        if (flag_n) { fprintf(stderr,"Duplicate -n!\n"); return 1; }
                        flag_n = true; num_count = vv; break;
                    case 'p':
                        if (flag_p) { fprintf(stderr,"Duplicate -p!\n"); return 1; }
                        flag_p = true; max_powerball = vv; break;
                    case 'N':
                        if (flag_N) { fprintf(stderr,"Duplicate -N!\n"); return 1; }
                        flag_N = true; num_sets = vv; break;
                    default:
                        fprintf(stderr,"Error: Unrecognized flag '%s'.\n", arg);
                        return 1;
                }
                pairs_count++;
                i += 2;
            }
            else {
                /* mode==1 => (number, flag)*/
                if (arg1_is_flag || !arg2_is_flag){
                    fprintf(stderr,"Error: Mixed format in pair '%s' '%s'.\n", arg, arg2);
                    return 1;
                }
                int vv = atoi(arg);
                char ff = arg2[1];
                switch (ff) {
                    case 'r':
                        if (flag_r) { fprintf(stderr,"Duplicate -r!\n"); return 1;}
                        flag_r = true; max_num = vv; break;
                    case 'n':
                        if (flag_n) { fprintf(stderr,"Duplicate -n!\n"); return 1;}
                        flag_n = true; num_count = vv; break;
                    case 'p':
                        if (flag_p) { fprintf(stderr,"Duplicate -p!\n"); return 1;}
                        flag_p = true; max_powerball = vv; break;
                    case 'N':
                        if (flag_N) { fprintf(stderr,"Duplicate -N!\n"); return 1;}
                        flag_N = true; num_sets = vv; break;
                    default:
                        fprintf(stderr,"Error: Unrecognized flag '%s'.\n", arg2);
                        return 1;
                }
                pairs_count++;
                i += 2;
            }
        }
    } 

    if (!flag_r || !flag_n || !flag_N) {
        fprintf(stderr,"Error: Missing required flags: -r, -n, or -N.\n");
        return 1;
    }
    if (max_num <= 0 || num_count <= 0 || num_sets <= 0) {
        fprintf(stderr,"Error: Invalid or missing values for -r, -n, or -N.\n");
        return 1;
    }
    if (num_count > max_num) {
        fprintf(stderr,"Error: The numbers to be generated (-n %d) cannot exceed the maximum (-r %d).\n", num_count, max_num);
        return 1;
    }

    // printf("Parsed values:\n");
    // printf("  -r (max_num)       = %d\n", max_num);
    // printf("  -n (num_count)     = %d\n", num_count);
    // printf("  -p (max_powerball) = %d\n", max_powerball);
    // printf("  -N (num_sets)      = %d\n", num_sets);

    // fprintf(stdout,"Parsed values: -n %d, -r %d, -p %d, -N %d\n", num_count, max_num, max_powerball, num_sets);


    srand(time(NULL));

    generate_lottery_numbers(num_count, max_num, max_powerball, num_sets);

    return 0;
}