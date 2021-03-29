#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <dirent.h> 
#include <stdbool.h>
#include <errno.h>

void listDir(bool r_flag, bool i_flag, bool l_flag, char * directory) {
    // these are test outputs to see if flags are being parsed correctly
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
    // this is the case when user specifies no directory
    if(strcmp(directory, "./myls") == 0) {
        d = opendir("./");
        while((dir = readdir(d))) {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    } 
    // this is the case when the user specifies a directory
    else {
        d = opendir(directory);
        // directory exists
        if(d) {
            while((dir = readdir(d))) {
                printf("%s\n", dir->d_name);
            }
            closedir(d);
        }
        // directory does not exist
        else if (ENOENT == errno) {
            printf("Error: directory does not exist\n");
            exit(1);
        }
        
    }
}

int main(int argc,char **argv) 
{ 
    bool r_flag = 0;
    bool i_flag = 0;
    bool l_flag = 0;
    char dir[1024] = "";

    int i;
    int j;
    // parse user input for flags and directories
    for(i = 0; i < argc; i++) {
        for(j = 1; argv[i][j] != '\0'; j++) {
            // any input beginning with '-' is treated as a potential flag
            if(argv[i][0] == '-'){
                if(argv[i][j] == 'R') {
                    r_flag = 1;
                }
                if(argv[i][j] == 'i') {
                    i_flag = 1;
                }
                if(argv[i][j] == 'l') {
                    l_flag = 1;
                }
                // if user enters a flag that is neither R, i, or l, we print error message and exit
                if(argv[i][j] != 'R' && argv[i][j] != 'i' && argv[i][j] != 'l') {
                    printf("Error: invalid flag\n");
                    exit(1);
                }
            } 
            // if input does not begin with a '-' we treat it as a directory that the user is specifying
            else {
                strcpy(dir, argv[i]);
            }
        }
    }
    listDir(r_flag, i_flag, l_flag, dir);
    return 0; 
} 

