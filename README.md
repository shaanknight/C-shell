
 ShaanKnight Shell (sksh) 
======================================

> Author : Sayantan Jana (20171185)

> Features

* This is a C-shell built for the 2nd Assignement of OS course in IIIT Hyderabad.
* It performs basic built in commands like :
	-> echo
	-> cd
	-> ls (with flags -a and -l)
	-> pinfo
	-> pwd
	-> setenv
	-> unsetenv
	-> jobs
	-> kjob
	-> fg
	-> bg
	-> overkill
* It can perform other system commands(used "execvp") with and without arguements in foreground and background processes.
* It supports piping and redirection as well.
* It takes care of the signals. CTR+C && CTR+Z

> Execute

* to compile : make main
* to run : ./sksh
* to remove the .o files and sksh : make clean
