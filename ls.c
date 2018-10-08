#include "globals.h"

int ls_func(int flag)
{
	/* The structure, struct dirent refers to directory entry.
	   The scandir() function scans the directory dirp, calling filter() on
       each directory entry.  Entries for which filter() returns nonzero are
       stored in strings allocated via malloc(3), sorted using qsort(3) with
       the comparison function compar(), and collected in array namelist
       which is allocated via malloc(3).  If filter is NULL, all entries are
       selected.

       The alphasort() and versionsort() functions can be used as the
       comparison function compar().*/

	struct dirent **lsf;
	int lsn = scandir(".", &lsf, 0, alphasort);
	if(lsn>=0)
	{
		for(int i = 0;i < lsn; ++i)
		{
			if(flag==0 && lsf[i]->d_name[0] == '.')
            	continue;
            printf("%s\n",lsf[i]->d_name);
		}
	}
	else
		perror("Error in ls");
	return 1;
}

int lsl_func(int flag)
{
	char last_mod_time[16];
	struct dirent **lsf;
	//These functions return information about a file, in the buffer pointed to by statbuf.
	struct stat info;
	long long int total = 0;
	int lsn = scandir(".",&lsf,0,alphasort);
	if(lsn > 0)
	{
		for(int i = 0; i < lsn ; ++i)
		{
			if(flag == 0 && lsf[i]->d_name[0] == '.')
				continue;
			// stat() retrieve information about the file pointed to by pathname
			if(stat(lsf[i]->d_name,&info)==0)
				total+=info.st_blocks;
		}
	}
	else
	{
		perror("Error in ls -l");
		return 1;
	}

	printf("total %lld\n",total/2);

	for(int i = 0; i < lsn ; ++i)
	{
		if(flag == 0 && lsf[i]->d_name[0] == '.')
			continue;
		if(stat(lsf[i]->d_name,&info)==0)
		{
			//st_mode gives us the file mode
			if(S_ISDIR(info.st_mode)) printf("d");
			else printf("-");
			
			if(info.st_mode & S_IRUSR) printf("r");
			else printf("-");

			if(info.st_mode & S_IWUSR) printf("w");
			else printf("-");

			if(info.st_mode & S_IXUSR) printf("x");
			else printf("-");

			if(info.st_mode & S_IRGRP) printf("r");
			else printf("-");

			if(info.st_mode & S_IWGRP) printf("w");
			else printf("-");

			if(info.st_mode & S_IXGRP) printf("x");
			else printf("-");

			if(info.st_mode & S_IROTH) printf("r");
			else printf("-");

			if(info.st_mode & S_IWOTH) printf("w");
			else printf("-");

			if(info.st_mode & S_IXOTH) printf("x");
			else printf("-");

			printf("%2lld ",info.st_nlink); // Number of hard links
			printf("%s ",(getpwuid(info.st_uid))->pw_name); // owner name
            printf("%s ",(getgrgid(info.st_gid))->gr_name); // group name
                
            printf("%10lld ",info.st_size); // Total size, in bytes

            strftime(last_mod_time,14,"%h %d %H:%M",localtime(&info.st_mtime)); // Time of last modification
            printf("%s ",last_mod_time);

            printf("%s\n",lsf[i]->d_name);

        }
    }

    return 1;
}




