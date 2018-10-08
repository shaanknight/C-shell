#include "globals.h"

void sigintHandler(int sig_num) 
{
    if(fg_flag==1)
    {
        if(kill(foreground_pid,9)==0)
            fg_flag = 0;
        return;
    }
    else
    {
	   signal(SIGINT,sigintHandler);
	   fflush(stdout);
    }
}

void sigstpHandler(int sig_num) 
{
    if(fg_flag==1)
    {
        if(kill(foreground_pid,SIGTSTP)==0)
            ctrz = 1;
        return;
    }
    else
    {
       signal(SIGSTOP,sigstpHandler);
       fflush(stdout);
    }
}

char* showpath(char* root,char* cwd)
{
	int lp,lh,i,min;

	lp = strlen(cwd);
	lh = strlen(root);

	if(lp<lh)
		min = lp;
	else
		min = lh;

	for(i = 0; i<min; i++)
	{
		if(cwd[i] != root[i])
			break;
	}

	if(i == lh)
	{
		cwd[0]='~';
		for(i = 1; i<=lp-lh; i++)
			cwd[i] = cwd[i+lh-1];
		cwd[i] = '\0';

		return cwd;
	}
	else
		return cwd;
}

void promtPrint(char* root)
{

	char temp[1024];
	char *user=NULL, *pwd=NULL;

	user = getenv("USER");

	/*Determines the path name of the working directory and stores it in buffer.
	  If successful, getcwd() returns a pointer to the buffer.
	  If unsuccessful, getcwd() returns a NULL pointer and sets appropriate errno.*/

	getcwd(temp,1023);
	pwd = showpath(root,temp);

	printf("<%s@",user);

	/*uname() returns system information in the structure pointed to by buf.  The utsname struct is defined in <sys/utsname.h>:
		struct utsname {
               char sysname[];    // Operating system name (e.g., "Linux") 
               char version[];    // Operating system version (e.g., "#1 SMP Debian 4.9.110-1 (2018-07-05)")
           };
    */

	struct utsname unameData;
	uname(&unameData);
	int s;
	for(s=0;unameData.version[s]!='D' && unameData.version[s]!='F' && unameData.version[s]!='U';++s);
	while(unameData.version[s]!=' ' && unameData.version[s]!='\n' && unameData.version[s]!='\0')
	{
		printf("%c",unameData.version[s]);
		s++;
	}
	printf(":%s/> ",pwd);

}

char *read_line()
{

	char *line = NULL;
	/*This data type is used to represent the sizes of blocks that can be read or written in a single operation. 
	  It is similar to size_t, but must be a signed type.*/
	ssize_t buffersize = 0;
	/*getline() reads an entire line from stream, storing the address of the buffer containing the text into *lineptr.
	  buffersize set to 0 , getline() will allocate a buffer for us.*/ 
	getline(&line,&buffersize,stdin);
	return line;

}

char **splitline(char* line)
{
	int buffer_size = 1024, ind = 0;
	char **commands = malloc(buffer_size * sizeof(char*));
	char* command;

	if(!commands)
	{
		// printing to the stderr the error
        fprintf(stderr, "Allocaiton Error \n");
        exit(EXIT_FAILURE);
    }

    /*strtok() Splits str[] according to given delimiters.
	  and returns next token. It needs to be called
	  in a loop to get all tokens. It returns NULL
	  when there are no more tokens.*/
    command = strtok(line,";");

    for(ind = 0; command ; ++ind)
    {
    	commands[ind] = command;
    	command = strtok(NULL,";");
    }

    commands[ind] = NULL;
    free(command);

    return commands;

}

char **splitcommand(char* each_line)
{
	int buffer_size = 256, ind = 0;
	char **words = malloc(buffer_size * sizeof(char*));
	char* word;

	if(!words)
	{
        fprintf(stderr, "Allocaiton Error \n");
        exit(EXIT_FAILURE);
    }

    word = strtok(each_line," \t\r\n\a");

    for(ind = 0; word ; ++ind)
    {
    	words[ind] = word;
    	word = strtok(NULL," \t\r\n\a");
    }

    words[ind] = NULL;
    free(word);

    return words;

}

char **splitpipe(char* each_line)
{
	int buffer_size = 256, ind = 0;
	char **words = malloc(buffer_size * sizeof(char*));
	char* word;

	if(!words)
	{
        fprintf(stderr, "Allocaiton Error \n");
        exit(EXIT_FAILURE);
    }

    word = strtok(each_line,"|");

    for(ind = 0; word ; ++ind)
    {
    	words[ind] = word;
    	word = strtok(NULL,"|");
    }

    words[ind] = NULL;
    free(word);

    return words;

}

int main()
{

	/*The C library function char *getenv(const char *name) searches for the environment
	 string pointed to by name and returns the associated value to the string.*/

	char* root = getenv("PWD");
	char* line;
	char** each_line;
	char** namepid = malloc(100 * sizeof(char*));
	char** remindpid = malloc(100 * sizeof(char*));
	char** command;
	char** piped_seg;
	int* ar = malloc(100 * sizeof(int));
	int* rpid = malloc(100 * sizeof(int));
	int state = 1, bg_flag = 0;
	int f_descs[2],j,l;
    ctrz = 0;
	for(int i=0;i<100;++i)
		ar[i] = 0;
		
	
	signal(SIGINT,sigintHandler);
    signal(SIGTSTP,sigstpHandler);

	while(1)
	{
        fg_flag = 0;
    	promtPrint(root);

    	line = read_line();
    	each_line = splitline(line);

    	for(int i = 0; each_line[i] != NULL ; ++i)
    	{

    		piped_seg = splitpipe(each_line[i]);
    		for(l = 0; piped_seg[l] ; ++l);
            
            int fs = 0;
    		for(int k = 0; k < l && l>1 ; ++k)
    		{
    			pipe(f_descs);
    			
                if(!fork())
    			{
                    dup2(fs,0);
                    if(k!=l-1)
    				    dup2(f_descs[1],1);
                    close(f_descs[0]);
    				
                    bg_flag = 0;
    				command = splitcommand(piped_seg[k]);
    				for(j = 0; command[j] ; ++j);
    				if(j>1 && command[j-1][0] == '&')
    				{
    					bg_flag = 1;
    					command[j-1] = NULL;
    				}
                    if(bg_flag == 0)
                        fg_flag = 1;
    				if(command[0] != NULL)
    				{
    					int saved_in = dup(0);
    					int saved_out = dup(1);
    					state = action(command,root,bg_flag,ar,namepid,rpid,remindpid);
    					dup2(saved_in,0);
    					dup2(saved_out,1);
    				}
			    	
                    free(command);
    				exit(0);
    			}
				wait(NULL);
				close(f_descs[1]);
    			fs = f_descs[0];
    		}
        	if(l==1)
            {
                bg_flag = 0;
                command = splitcommand(piped_seg[0]);
                for(j = 0; command[j] ; ++j);
                if(j>1 && command[j-1][0] == '&')
                {
                    bg_flag = 1;
                    command[j-1] = NULL;
                }
                if(bg_flag == 0)
                    fg_flag = 1;
                if(command[0] != NULL)
                {
                    int saved_in = dup(0);
                    int saved_out = dup(1);
                    state = action(command,root,bg_flag,ar,namepid,rpid,remindpid);
                    dup2(saved_in,0);
                    dup2(saved_out,1);
                }
                free(command);
            }
            free(piped_seg);
        }
        free(each_line);
        free(line);

    	if(!state) break;

    	for(int i=0;i<100;++i)
    	{
    		int status;
    		if(ar[i]>0 && waitpid(ar[i],&status,WNOHANG) == ar[i])
    		{
    			printf("%s with pid %d exited normally\n",namepid[i],ar[i]);
    			ar[i] = -1;
    			namepid[i] = NULL;
    		}
    	}

    	for(int i=0;i<100;++i)
    	{
    		int nstatus;
    		if(rpid[i]!=0 && waitpid(rpid[i],&nstatus,WNOHANG) == rpid[i])
    		{
    			printf("%s\n",remindpid[i]);
    			rpid[i] = 0;
    			remindpid[i] = NULL;
    		}
    	}
    }

    free(ar);
    free(namepid);
    free(rpid);
    free(remindpid);

    /*The EXIT_SUCCESS and EXIT_FAILURE macros expand into integral expressions that can be used as arguments
     to the exit function (and, therefore, as the values to return from the main function), and indicate program execution status.
     Used in various parts of code too.*/

	return EXIT_SUCCESS;

}
