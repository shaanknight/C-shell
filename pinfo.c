#include "globals.h"

int pinfo(char **command,char* root)
{
	char path[1024],pr_status,ex_path[1024];
	int pid,v_mem;

	// check if pid is given for which we have to pinfo
	if(command[1] == NULL)
		strcpy(path,"/proc/self");
	else
	{
		// converting string to integer
		int check_pid = atoi(command[1]);
		/*The kill() system call can be used to send any signal to any process group or process.
   		  If sig is 0, then no signal is sent, but error checking is still performed.
   		  On success zero is returned.  On error, -1 is returned.
   		 */
		if(kill(check_pid,0) == -1)
		{
			perror("Invalid PID specified");
			return 1;
		}
		strcpy(path,"/proc/");
		strcat(path,command[1]);
	}

	// ----*** pid and status ***----
	strcat(path,"/stat");

	//opening the file /proc/____/stat and checking for error

	/*The fopen() function opens the file whose pathname is the string pointed to by filename, and associates a stream with it.
	  Upon successful completion, fopen() returns a pointer to the object controlling the stream. 
	  Otherwise, a null pointer is returned, and errno is set to indicate the error. 
	*/
	// path string - printf("%s\n",path);
	FILE *stat = fopen(path, "r");
	if(stat == NULL)
	{
		perror("error in pinfo");
		return 1;
	}

	//printing the pid
	fscanf(stat,"%d",&pid);
	printf("pid -- %d\n", pid);

	//printing the process status
	char pp[100];
	fscanf(stat,"%s",pp);
	fscanf(stat," %c",&pr_status);
	printf("process status -- %c\n", pr_status);

	fclose(stat);

	// ----*** virtual memory size ****----
	// opening /proc/____/statm
	strcat(path,"m");
	// path string - printf("%s\n",path);
	FILE* statm = fopen(path,"r");
	if(stat == NULL)
	{
		perror("error in pinfo");
		return 1;
	}

	//printing virtual memomry size 
	fscanf(statm, "%d", &v_mem);
	printf("-  %d {Virtual Memory}\n",v_mem);

	fclose(statm);

	// ----*** executable path ***---

	// removing from the string statm and concatenating exe
	int i,j;
	for(i=0;path[i]!='\0';++i);
	for(j=0;j<=4;++j)
		path[i-1-j]='\0';

	strcat(path,"exe");
	// path string - printf("%s\n",path);
	int len = readlink(path,ex_path,sizeof(ex_path));
	/* if readlink is unsuccessful it returns -1 to the variable supposed to hold the buffer size
	   on a success
	*/
	if(len<0)
	{
		perror("error reading the path link");
		return 1;
	}

	// string returned by readlink doesn't have a null terminator
	ex_path[len] = '\0';
	char *exp = NULL;
	// checking relativity of path
	exp = showpath(root,ex_path);
	printf("-  Executable Path --- %s\n",exp);
	return 1;

}