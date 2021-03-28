#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <dirent.h> 
#include <stdbool.h>

void listDir(bool r_flag, bool i_flag, bool l_flag) {
    if(r_flag) {
        printf("R flag was entered\n");
    } 
    if(i_flag){
        printf("i flag was entered\n");
    } 
    if(l_flag){
        printf("l flag was entered\n");
    }
    DIR *d;
    struct dirent * dir;
    d = opendir("./");
    while((dir = readdir(d))) {
        printf("%s\n", dir->d_name);
    }
    closedir(d);
    
}

int main(int argc,char **argv) 
{ 
    bool r_flag = 0;
    bool i_flag = 0;
    bool l_flag = 0;

    int i;
    if(argv[1] && argv[1][0] == '-'){ 
        for(i = 1; i < strlen(argv[1]); i++) {
            if(argv[1][i] == 'R'){
                r_flag = 1;
            } 
            if(argv[1][i] == 'i') {
                i_flag = 1;
            } 
            if(argv[1][i] == 'l') {
                l_flag = 1;
            }
            if(argv[1][i] != 'R' && argv[1][i] != 'i' && argv[1][i] != 'l') {
                printf("Error: invalid flag entered\n");
                exit(1);
            }
        }
    } 
    if(argv[2] && argv[2][0] == '-') {
        for(i = 1; i < strlen(argv[2]); i++) {
            if(argv[2][i] == 'R'){
                r_flag = 1;
            } 
            if(argv[2][i] == 'i') {
                i_flag = 1;
            } 
            if(argv[2][i] == 'l') {
                l_flag = 1;
            }
            if(argv[2][i] != 'R' && argv[2][i] != 'i' && argv[2][i] != 'l') {
                printf("Error: invalid flag entered\n");
                exit(1);
            }
        }
    } 
    if(argv[3] && argv[3][0] == '-') {
        for(i = 1; i < strlen(argv[3]); i++) {
            if(argv[3][i] == 'R'){
                r_flag = 1;
            } 
            if(argv[3][i] == 'i') {
                i_flag = 1;
            } 
            if(argv[3][i] == 'l') {
                l_flag = 1;
            }
            if(argv[3][i] != 'R' && argv[3][i] != 'i' && argv[3][i] != 'l') {
                printf("Error: invalid flag entered\n");
                exit(1);
            }
        }
    }
    
    listDir(r_flag, i_flag, l_flag);
    return 0; 
} 

