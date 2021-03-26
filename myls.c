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

    listDir(r_flag, i_flag, l_flag);
    return 0; 
} 

