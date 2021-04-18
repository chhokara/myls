#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <libgen.h>
#include "myls.h"
#include <pwd.h>
#include <grp.h>
#include "infodemo.c"

int processToken(char *, struct Options *, char **, int *);
void myls(struct Options *, char **, int numPaths);
char *permissions(char *);
void mylsExecute(char * dirname, struct Options *);
void mylsDir(char *, struct Options *);
void handleDisplay(char *, struct Options *);
void printFileID(char *);
void printLongListing(char *);
void list(char *, struct Options *);

void myls(struct Options *options, char **paths, int numPaths)
{
  if (numPaths == 0)
  {
    paths[numPaths++] = "./";
  }
  
  for (int i = 0; i < numPaths; i++)
  {
    if (numPaths > 1 && !options->_R) {
      printf("%s:\n", paths[i]);
    }
    mylsExecute(paths[i], options);
    printf("\n");
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
      if(token[i] != 'R' && token[i] != 'i' && token[i] != 'l') {
        printf("Error: Invalid Flag\n");
        exit(1);
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
    modeval[0] = (perm & S_IFDIR) ? 'd' : '-';
    modeval[1] = (perm & S_IRUSR) ? 'r' : '-';
    modeval[2] = (perm & S_IWUSR) ? 'w' : '-';
    modeval[3] = (perm & S_IXUSR) ? 'x' : '-';
    modeval[4] = (perm & S_IRGRP) ? 'r' : '-';
    modeval[5] = (perm & S_IWGRP) ? 'w' : '-';
    modeval[6] = (perm & S_IXGRP) ? 'x' : '-';
    modeval[7] = (perm & S_IROTH) ? 'r' : '-';
    modeval[8] = (perm & S_IWOTH) ? 'w' : '-';
    modeval[9] = (perm & S_IXOTH) ? 'x' : '-';
    modeval[10] = '\0';
    return modeval;
  }
  else
  {
    return strerror(errno);
  }
}

void mylsExecute(char * inputPath, struct Options *options) {

  char *path = malloc(strlen(inputPath) + 1);
  strcpy(path, inputPath);
  if (path[strlen(inputPath) - 1] != '/') {
    path[strlen(inputPath)] = '/';
  }

  if (access(path, F_OK) != 0) {
    printf("myls: cannot access '%s': No such file or directory\n", inputPath);
    free(path);
    return;
  }

  DIR *d;
  d = opendir(path);

  if (d) {
    mylsDir(path, options);
  } else {
    handleDisplay(path, options);
  }
  free(path);
}

#define MAX_SUBDIRS 1024

void mylsDir(char *path, struct Options *options) {
  char * subdir = "";
  struct dirent ** namelist;
  int n;
  int i = -1;
  char **subDirectories = (char **)malloc((MAX_SUBDIRS) * sizeof(char *));;
  int numSubDirs = 0;

  if (options->_R) {
    printf("\n");
    for (int i = 0; i < strlen(path) - 1; i++){
      printf("%c", path[i]);
    }
    printf(":\n");
  }
  n = scandir(path, &namelist, 0, alphasort);
  if(n < 0) {
    perror("scandir");
  } else {
      while(++i < n) {
        if(!strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name, "..") || namelist[i]->d_name[0] == '.') {
          continue;
        }
        char *fullItemPath = malloc(strlen(path) + strlen(namelist[i]->d_name) + 5);
        strcpy(fullItemPath, path);
        strcat(fullItemPath, namelist[i]->d_name);
        handleDisplay(fullItemPath, options);
        
        if(namelist[i]->d_type == DT_DIR && options->_R) {
          subdir = malloc(strlen(path) + strlen(namelist[i]->d_name) + 2);

          strcpy(subdir, path);
          strcat(subdir, namelist[i]->d_name);
          strcat(subdir, "/");

          subDirectories[numSubDirs] = malloc(strlen(subdir) + 3);
          strcpy(subDirectories[numSubDirs], subdir);
          numSubDirs += 1;

          free(subdir);
        }
      }

      for (int i = 0; i < numSubDirs; i++) {
        mylsExecute(subDirectories[i], options);
      }
    }
}

void handleDisplay(char *path, struct Options *options) {
  if (options->_i) {
    printFileID(path);
  }
  if (options->_l) {
    printLongListing(path);
  } else {
    printf("%s\n", basename(path));
  }
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

void printLinks(char* path) {
  struct stat st;
  if (stat(path, &st) == 0)
  {
    printf("%2lu", st.st_nlink);
  }
  else
  {
    perror("printLinks");
  }
}

void printGroup(char* path)
{
  struct stat st;
  if (stat(path, &st) == 0)
  {
    getAndPrintGroup(st.st_gid);
  }
  else
  {
    perror("printGroup");
  }
}

void printUser(char* path)
{
  struct stat st;
  if (stat(path, &st) == 0)
  {
    getAndPrintUserName(st.st_uid);
  }
  else
  {
    perror("printUser");
  }
}

unsigned int getFileSize(char* path)
{
  struct stat st;
  unsigned int fileSize = 0;
  if (stat(path, &st) == 0)
  {
    fileSize = st.st_size;
  }
  else
  {
    perror("getFileSIze");
  }
  return fileSize;
}

void getAndPrintLastModificationDate(char* path)
{
  struct stat st;
  
  if (stat(path, &st) == 0)
  {
    struct timespec lastModTime = st.st_mtim;
    char *dayBuf = malloc(3);
    char monthBuf[4];
    char yearAndTimeBuf[20];
    struct tm time;
    localtime_r(&lastModTime.tv_sec, &time);
    strftime(monthBuf, 4, "%b", &time);
    strftime(yearAndTimeBuf, 20, "%Y %H:%M", &time);
    strftime(dayBuf, 3, "%d", &time);
    if (dayBuf[0] == '0') {
      dayBuf++;
    }
    printf(" %s %2s %s ", monthBuf, dayBuf, yearAndTimeBuf);

  }
  else
  {
    perror("getAndPrintLastModificationDate");
  }
}

void printLongListing(char *path) {
  printf("%s ", permissions(path));
  printLinks(path);
  printUser(path);
  printGroup(path);
  printf("%7u", getFileSize(path));
  getAndPrintLastModificationDate(path);
  struct stat st;
  if (lstat(path, &st) == 0) {
    if (S_ISLNK(st.st_mode) == 1) {
      printf("%s -> ", basename(path));
      char buf[1024];
      ssize_t len;
      if ((len = readlink(path, buf, sizeof(buf)-1)) != -1) {
        buf[len] = '\0';
      }
      printf("%s\n", buf);
    } else {
      printf("%s\n", basename(path));
    }
  }
  else
  {
    perror("symlink");
  }
}