#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <dirent.h> 

void listDir() {
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
    listDir();
    return 0; 
} 

