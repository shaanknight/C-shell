#include "globals.h"

int setenv_func(char **command)
{

	/*The setenv() function shall update or add a variable in the environment of the calling process.
	 The envname argument points to a string containing the name of an environment variable to be added or altered.
	 If the environment variable named by envname already exists and the value of overwrite is non-zero,
	 the function shall return success and the environment shall be updated.
	 If the environment variable named by envname already exists and the value of overwrite is zero,
	 the function shall return success and the environment shall remain unchanged.
	*/

	int l;
	for(l = 0; command[l] ; ++l);
	if(l<=1 || l>3)
	{
		printf("Proper no. of arguements not provided\n");
		return 1;
	}
	if(l==2)
		setenv(command[1] , " " , 1);
	else
		setenv(command[1] , command[2] , 1);
	return 1;
}

int unsetenv_func(char **command)
{
	int l;
	for(l = 0; command[l] ; ++l);
	if(l<=1)
	{
		printf("Proper no. of arguements not provided\n");
		return 1;
	}
	unsetenv(command[1]);
	return 1;
}

int jobs_func(int* ar,char **namepid)
{
	int i,pid;
	int h=0;
	char path[102];
	char pr_status;
	char buffer[16],pp[100];

	for(i = 0; ar[i] != 0 ; ++i)
	{
		if(ar[i] == -1)
			continue;
		strcpy(path,"/proc/");
		++h;
		
		for(int j = 0; j<10 ; ++j)
			buffer[j] = '\0';

		snprintf(buffer, 10, "%d", ar[i]);
		strcat(path,buffer);
		strcat(path,"/stat");
		FILE *stat = fopen(path, "r");
		fscanf(stat,"%d",&pid);
		fscanf(stat,"%s",pp);
		fscanf(stat," %c",&pr_status);
		printf("[%d] %c %s %d\n", h, pr_status,namepid[i],ar[i]);
		fclose(stat);

		for(int j = 0; j<100 ; ++j)
			path[j] = '\0';
	}
	return 1;
}

int kjobs(char **command, int *ar)
{
	int l,h=0;  
	for(l = 0; command[l] ; ++l);
	if(l<=2 || l>3)
	{
		printf("Proper no. of arguements not provided\n");
		return 1;
	}
	int i;
	int id = 0, sigval = 0;
	for(i = 0; command[1][i] != '\0' ; ++i)
		id = id*10 + (command[1][i]-'0');
	for(i = 0; command[2][i] != '\0' ; ++i)
		sigval = sigval*10 + (command[2][i]-'0');
	for(i = 0; ar[i]!=0 ; ++i)
	{
		if(ar[i] == -1)
			continue;
		++h;
		if(h == id)
			break;
	}
	kill(ar[i] , sigval);
	return 1;

}

int overkill_func(int *ar)
{
	for(int i=0;i<100;++i)
	{
		if(ar[i]==0 || ar[i]==-1)
			continue;
		else
			kill(ar[i],SIGKILL);
	}
	return 1;
}

int bg_func(char **command,int *ar)
{
	int l,h=0,i;
	for(l = 0; command[l] ; ++l);
	if(l<=1 || l>3)
	{
		printf("Proper no. of arguements not provided\n");
		return 1;
	}
	int check = atoi(command[1]);
	for(i=0;i<100 && ar[i]!=0;++i)
	{
		if(ar[i]==-1)
			continue;
		++h;
		if(h==check)
		{
			printf("%ld continued\n",ar[i]);
			kill(ar[i],SIGCONT);
			return 1;
		}
	}
}

int fg_func(char **command,int *ar)
{
	int l,h=0,i,status;
	for(l = 0; command[l] ; ++l);
	if(l<=1 || l>3)
	{
		printf("Proper no. of arguements not provided\n");
		return 1;
	}
	int check = atoi(command[1]);
	for(i=0;i<100 && ar[i]!=0;++i)
	{
		if(ar[i]==-1)
			continue;
		++h;
		if(h==check)
		{
			kill(ar[i],SIGCONT);
			do{
				waitpid(ar[i], &status, WUNTRACED);
				if(ctrz==1)
				{
					ctrz=0;
					return 1;
				}
			}while(!WIFEXITED(status) && !WIFSIGNALED(status));
			ar[i]=-1;
			return 1;
		}
	}
	printf("background process does not exit\n");
	return 1;
}


