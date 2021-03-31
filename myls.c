#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>
#include "myls.h"

int processToken(char *, struct Options *, char **, int *);
void myls(struct Options *, char **, int numPaths);
char *permissions(char *);

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

  DIR *d;
  struct dirent *dir;

  // default print pwd
  if (numPaths == 0)
  {
    paths[numPaths++] = "./";
  }

  for (int i = 0; i < numPaths; i++)
  {
    d = opendir(paths[i]);
    // directory exists
    if (d)
    {
      while ((dir = readdir(d)))
      {
        if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
        {
          continue;
        }
        if(options->_l) {
            printf("%s ", permissions(dir->d_name));
        }
        if(options->_i) {
            printf("%lu ", dir->d_ino);
        }
        printf("%s\n", dir->d_name);

      }
      closedir(d);
    }
    // directory does not exist
    else if (ENOENT == errno)
    {
      printf("Error: directory does not exist\n");
      exit(1);
    }
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