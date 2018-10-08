GC=gcc

main:
	${GC} -c syscommand.c action.c cd.c echo.c ls.c pinfo.c pwd.c clock.c reminder.c userdef.c
	${GC} main.c syscommand.o action.o cd.o echo.o ls.o pinfo.o pwd.o clock.o reminder.o userdef.o -o sksh

clean:
	rm *.o
	rm sksh
