#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>

int fg_flag;
int foreground_pid;
int ctrz;

int proc_start(char **command,int bg_flag,int* ar,char **namepid,char *root);
int cd_func(char **command,char *root);
int echo_func(char **command);
int ls_func(int flag);
int lsl_func(int flag);
int pwd_func();
char* showpath(char* root,char* cwd);
void promtPrint(char* root);
char *read_line();
char **splitline(char* line);
char **splitcommand(char* each_line);
int pinfo(char **command,char* root);
int clock_func(char **command);
int remindme_func(char **command,int *rpid,char **remindpid);
int action(char **command,char *root,int flag,int *ar,char **namepid,int *rpid,char **remindpid);
int setenv_func(char **command);
int unsetenv_func(char **command);
int jobs_func(int* ar,char **namepid);
int kjobs(char **command, int *ar);
int bg_func(char **command,int *ar);
int fg_func(char **command,int *ar);
int overkill_func(int *ar);
