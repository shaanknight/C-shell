#include "globals.h"

int pwd_func()
{
	int buffer_size = 1024;
	char *pwd = (char *)malloc(buffer_size*sizeof(char));
	getcwd(pwd,1024);
	printf("%s\n",pwd);
	free(pwd);
	return 1;
}