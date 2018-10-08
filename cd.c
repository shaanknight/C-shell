#include "globals.h"

int cd_func(char **command,char *root)
{
	char cwd[1024],hd[1024];

	strcpy(hd,root);
	getcwd(cwd,sizeof(cwd));

	int result = 0;

	/*chdir() changes the current working directory of the calling process
      to the directory specified in path.
	  On success, zero is returned.  On error, -1 is returned, and errno is
      set appropriately.
      perror prints a description for a system error code or for a storage engine 
      (table handler) error code.*/

	if(!command[1] || (strcmp(command[1],"~")==0) || (strcmp(command[1], "~/")==0))
		result = chdir(hd);
	else
	{
		if(chdir(command[1])<0)
			perror("Error");
	}
	return 1;
}
