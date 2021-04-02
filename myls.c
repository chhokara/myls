#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>
#include "myls.h"

// helpful post: https://stackoverflow.com/questions/29401653/printing-all-files-and-folders-in-a-path-in-linux-recursively-in-c?noredirect=1&lq=1
int processToken(char *, struct Options *, char **, int *);
void myls(struct Options *, char **, int numPaths);
char *permissions(char *);
void mylsExecute(char * dirname, struct Options *);
void mylsDir(char *, struct Options *);
void handleDisplay(char *, struct Options *);
void printFileID(char *);
void printLongListing(char *);

void myls(struct Options *options, char **paths, int numPaths)
{
  // these are test outputs to see if flags are being parsed correctly
  if (options->_R)
  {
    printf("R flag was entered\n");
  }
  if (options->_i)
  {
    printf("i flag was entered\n");
  }
  if (options->_l)
  {
    printf("l flag was entered\n");
  }


  // default print pwd
  if (numPaths == 0)
  {
    paths[numPaths++] = "./";
  }

  for (int i = 0; i < numPaths; i++)
  {
    mylsExecute(paths[i], options);
  }
}

int main(int argc, char **argv)
{
  struct Options *options = malloc(sizeof(struct Options));
  char **paths = (char **)malloc((argc - 1) * sizeof(char *));
  int numPaths = 0;

  for (int i = 1; i < argc; i++)
  {
    processToken(argv[i], options, paths, &numPaths);
  }

  myls(options, paths, numPaths);
  return 0;
}

int processToken(char *token, struct Options *options, char **paths, int *numPaths)
{
  if (token[0] == '-')
  {
    for (int i = 1; token[i] != '\0'; i++)
    {
      if (token[i] == 'R')
      {
        options->_R = true;
      }
      if (token[i] == 'i')
      {
        options->_i = true;
      }
      if (token[i] == 'l')
      {
        options->_l = true;
      }
    }
  }
  else
  {
    paths[*numPaths] = malloc(strlen(token) + 3);
    strcpy(paths[*numPaths], token);
    *numPaths += 1;
  }
  return 0;
}

char *permissions(char *file)
{
  struct stat st;
  char *modeval = malloc(sizeof(char) * 9 + 5);
  if (stat(file, &st) == 0)
  {
    mode_t perm = st.st_mode;
    modeval[0] = (perm & S_IRUSR) ? 'r' : '-';
    modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
    modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
    modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
    modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
    modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
    modeval[6] = (perm & S_IROTH) ? 'r' : '-';
    modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
    modeval[8] = (perm & S_IXOTH) ? 'x' : '-';
    modeval[9] = '\0';
    return modeval;
  }
  else
  {
    return strerror(errno);
  }
}

void mylsExecute(char * path, struct Options *options) {

  if (access(path, F_OK) != 0) {
    printf("Error: file or directory does not exist\n");
    exit(EXIT_FAILURE);
  }

  // Check if directory
  DIR *d;
  d = opendir(path);

  if (d) {
    mylsDir(path, options);
  } else {
    handleDisplay(path, options);
  }
}

void mylsDir(char *path, struct Options *options) {
  char * subdir;
  struct dirent ** namelist;
  int n;
  int i = -1;

  n = scandir(path, &namelist, 0, alphasort);
  if(n < 0) {
    perror("scandir");
  } else {
    while(++i < n) {
      if(namelist[i]->d_name[0] != '.') {
        if(options->_i) {
          printf("%lu ", namelist[i]->d_ino);
        }
        if(options->_l) {
          printf("%s ", permissions(namelist[i]->d_name));
        }
        printf("%s\n", namelist[i]->d_name);
      }

      if(namelist[i]->d_type == DT_DIR && strcmp(namelist[i]->d_name, ".") && strcmp(namelist[i]->d_name, "..") && namelist[i]->d_name[0] != '.') {
        subdir = malloc(strlen(path) + strlen(namelist[i]->d_name) + 2);

        strcpy(subdir, path);
        strcat(subdir, "/");
        strcat(subdir, namelist[i]->d_name);

        mylsExecute(subdir, options);

        free(subdir);
      }
    }
  }
}

void handleDisplay(char *path, struct Options *options) {
  if (options->_i) {
    printFileID(path);
  }
  if (options->_l) {
    printLongListing(path);
  }
  printf(" %s\n", basename(path));
}

void printFileID(char *path) {
  struct stat st;
  if (stat(path, &st) == 0)
  {
    printf("%lu ", st.st_ino);
  }
  else
  {
    perror("printFileID");
  }
}

void printLongListing(char *path) {
  printf("%s", permissions(path));
  // printf("%s\n", dir->d_name);
}