#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <dirent.h> 
#include <stdbool.h>

void listDir(bool r_flag, bool i_flag, bool l_flag) {
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
    if(argv[1]){ 
        for(i = 0; i < strlen(argv[1]); i++) {
            if(argv[1][i] == 'R'){
                r_flag = 1;
                printf("R flag was entered\n");
            } else if(argv[1][i] == 'i') {
                i_flag = 1;
                printf("i flag was entered\n");
            } else if(argv[1][i] == 'l') {
                l_flag = 1;
                printf("l flag was entered\n");
            }
        }
    }
    
    listDir(r_flag, i_flag, l_flag);
    return 0; 
} 

