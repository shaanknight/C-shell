#include "globals.h"

int echo_func(char **command)
{
	for(int i = 1; command[i] ; ++i)
		printf("%s ",command[i]);
	printf("\n");
	return 1;
}
