#include "globals.h"

int clock_func(char **command)
{
	char date[100],time[100];
	if(command[4] == NULL || command[5] != NULL || strcmp(command[1],"-t") || strcmp(command[3],"-n"))
	{
		printf("Invalid Syntax\n");
		printf("Original Syntax clock -t <interval> -n <duration> \n");
		return 1;
	}
	int interval = atoi(command[2]);
	int duration = atoi(command[4]);
	int cycles = duration/interval;
	
	for(int i = 1; i <= cycles + 1; ++i)
	{
		// *** date ***
		FILE *rtcdate = fopen("/sys/class/rtc/rtc0/date","r");
		fscanf(rtcdate,"%s",date);
		fclose(rtcdate);

		// *** time ***
		FILE *rtctime = fopen("/sys/class/rtc/rtc0/time","r");
		fscanf(rtctime,"%s",time);
		fclose(rtctime);

		printf("%s, %s\n",date,time);
		sleep(interval);
	}

	for(int i=0;i<100;++i)
		date[i]=time[i]='\0';
	return 1;
}
