#include "globals.h"

int proc_start(char **command,int bg_flag,int* ar,char **namepid,char *root)
{
	// The pid_t data type represents process IDs. 
	pid_t pid,wpid;

	/*System call fork() is used to create processes.
	  It takes no arguments and returns a process ID. 
	  The purpose of fork() is to create a new process, which becomes the child process of the caller. 
	  After a new child process is created, both processes will execute the next instruction following the fork() system call. 
	  Therefore, we have to distinguish the parent from the child. This can be done by testing the returned value of fork():

	  If fork() returns a negative value, the creation of a child process was unsuccessful.
	  fork() returns a zero to the newly created child process.
	  fork() returns a positive value, the process ID of the child process, to the parent. The returned process ID is of type pid_t defined in sys/types.h. Normally, the process ID is an integer. Moreover, a process can use function getpid() to retrieve the process ID assigned to this process.
	*/
	pid = fork();

	if (!pid)
	{
		/*The execvp() family of commands can be used to execute an
		  application from a process. The system call execvp()
		  replaces the executing process by a new process image
		  which executes the application specified as its parameter.
		  Arguments can also be specified.
		*/
		if(execvp(command[0], command))
			perror("sksh");
		exit(EXIT_FAILURE);
		return 1;
	}
	else if(pid < 0)
		perror("Error forking");
	else
	{
		if(bg_flag)
		{
			signal(SIGCHLD,SIG_DFL);
			for(int i=0;i<100;++i)
			{
				if(ar[i]==0)
				{
					ar[i] = pid;
					namepid[i] = malloc((5+strlen(command[0])) * sizeof(char));
					strcpy(namepid[i],command[0]);
					printf("%s with %d started\n",command[0],pid);
					return 1;
				}
			}
		}
		else
		{
			int status;
			if(bg_flag == 0)
				foreground_pid = pid;
			do
			{
				/*The waitpid() system call suspends execution of the calling process 
				  until a child specified by pid argument has changed state. 
				  By default, waitpid() waits only for terminated children, but this behavior is modifiable via the options argument.
				  
				  WUNTRACED The status of any child processes specified by pid that are stopped, and whose status has not yet been reported 
				  since they stopped, shall also be reported to the requesting process.
				  
				  If the child process for which status was returned by the wait or waitpid function exited normally, 
				  the WIFEXITED macro evaluates to TRUE.

				  WIFSIGNALED Returns True if status was returned for a child that terminated abnormally, by receipt of a signal
 				  that it didn’t catch
				*/
				wpid = waitpid(pid, &status, WUNTRACED); 
				if(ctrz==1)
				{
					for(int i=0;i<100;++i)
					{
						if(ar[i]==0)
						{
							ar[i] = pid;
							namepid[i] = malloc((5+strlen(command[0])) * sizeof(char));
							strcpy(namepid[i],command[0]);
							printf("%s with %d started\n",command[0],pid);
							break;
						}
					}
					ctrz=0;
					fg_flag = 0;
					return 1;	
				}
			} while(!WIFEXITED(status) && !WIFSIGNALED(status));
		}	
		return 1;	
	}
}
