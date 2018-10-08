#include "globals.h"

int action(char **command,char *root,int flag,int *ar,char **namepid,int *rpid,char **remindpid)
{
  	if(!strcmp(command[0],"quit"))
	     	return EXIT_SUCCESS;
    
    // ***- "redirection" -***
    int in_f=0, out_f=0, ap_f=0;
    char inp_file[64], out_file[64], ap_file[64];
    int fd;
    
    for(int i=0;command[i]!='\0';i++)
    {
        if(!(strcmp(command[i],"<")))
        {        
            command[i]=NULL;
            strcpy(inp_file,command[i+1]);
            in_f=1;           
        }
        else if(!(strcmp(command[i],">")))
        {      
            command[i]=NULL;
            strcpy(out_file,command[i+1]);
            out_f=1;
        }         
        else if(!(strcmp(command[i],">>")))
        {      
            command[i]=NULL;
            strcpy(ap_file,command[i+1]);
            ap_f=1;
        }  
    }

    /*
        dup() : The dup() system call creates a copy of a file descriptor.
                It uses the lowest-numbered unused descriptor for the new descriptor.

        dup2() : The dup2() system call is similar to dup() but the basic difference between them is that 
                 instead of using the lowest-numbered unused file descriptor, 
                 it uses the descriptor number specified by the user.

                 As in dup2(), in place of newfd any file descriptor can be put.
                 This will lead all the printf() statements to be written in the file
                  referred by the old file descriptor.
    
    */

    if(in_f)
    { 
        fd = open(inp_file, O_RDONLY, 0);
        if(fd<0)
            perror("Error opening input file");
        if(dup2(fd,0) < 0) 
            perror("Error - duping the input");
        close(fd);
    }

    if(out_f)
    {
        fd = open(out_file, O_WRONLY | O_TRUNC | O_CREAT ,0644);
        if(fd<0) 
            perror("Error opening output file");
        if(dup2(fd,1)<0) 
            perror("Error - duping the output");
        close(fd);
    }

    if(ap_f)
    {
        fd = open(ap_file, O_WRONLY | O_APPEND | O_CREAT,0644);
        if(fd<0)
            perror("Error opening the to be appended file");
        if(dup2(fd,1)<0) 
            perror("Error - duping while appending");
        close(fd);
    }
    //printf("%s\n",command[0]);
	if(!strcmp(command[0],"cd"))
	    return cd_func(command,root);
	else if(!strcmp(command[0],"echo"))
	    return echo_func(command);
	else if(!strcmp(command[0],"pwd"))
	    return pwd_func();
	else if(!strcmp(command[0],"ls"))
	{
	    if(command[1]==NULL)
		    return ls_func(0);
		else if(command[2] == NULL && !strcmp(command[1],"-a"))
	        return ls_func(1);
	    else if(command[2] == NULL && !strcmp(command[1],"-l"))
	        return lsl_func(0);
	    else if(command[2] == NULL && !strcmp(command[1],"-la"))
		    return lsl_func(1);
		else if(command[2] == NULL && !strcmp(command[1],"-al"))
			return lsl_func(1);
		else if(command[2] != NULL && !strcmp(command[1],"-l") && !strcmp(command[2],"-a"))
			return lsl_func(1);
		else if(command[2] != NULL && !strcmp(command[2],"-l") && !strcmp(command[1],"-a"))
			return lsl_func(1);
		else
		{
			perror("ls cannot access certain flags : No such file or directory exits\n");
			return 1;
		}
	}
	else if(!strcmp(command[0],"pinfo"))
		return pinfo(command,root);
	else if(!strcmp(command[0],"clock"))
		return clock_func(command);
	else if(!strcmp(command[0],"remindme"))
		return remindme_func(command,rpid,remindpid);
    else if(!strcmp(command[0],"setenv"))
        return setenv_func(command);
    else if(!strcmp(command[0],"unsetenv"))
        return unsetenv_func(command);
    else if(!strcmp(command[0],"jobs"))
        return jobs_func(ar,namepid);
    else if(!strcmp(command[0],"kjob"))
        return kjobs(command,ar);
    else if(!strcmp(command[0],"bg"))
        return bg_func(command,ar);
    else if(!strcmp(command[0],"overkill"))
	return overkill_func(ar); 
    else if(!strcmp(command[0],"fg"))
        return fg_func(command,ar);
	else if(command[0]!=NULL)
		return proc_start(command,flag,ar,namepid,root);
	else
		return 1;
}
