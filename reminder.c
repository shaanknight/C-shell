#include "globals.h"

int remindme_func(char **command,int *rpid,char **remindpid)
{
	// syntax checking
	if(command[2]==NULL)
	{
		printf("Syntax error\n");
		printf("Original syntax : remindme <duration> <job>\n");
	    return 1;
	}

	// starting a background process for the assigned job
	pid_t pid;
	pid = fork();

	if (!pid)
	{
		sleep(atoi(command[1]));
		exit(EXIT_FAILURE);
	}
	else if(pid < 0)
		perror("Error forking ");
	else
	{
		signal(SIGCHLD,SIG_DFL);
		for(int i=0;i<100;++i)
		{
			if(rpid[i]==0)
			{
				rpid[i] = pid;
				int job_length = 0;
				for(int l=2;command[l]!=NULL;++l)
					job_length = job_length + strlen(command[l]);

				remindpid[i] = malloc((5+2*job_length) * sizeof(char));
				strcpy(remindpid[i],command[2]);
				for(int l=3;command[l]!=NULL;++l)
				{
					strcat(remindpid[i]," ");
					strcat(remindpid[i],command[l]);
				}
				break;
			}
		}
	}
	return 1;
}