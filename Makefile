CC=gcc
LINK=ld
CFLAGSSHORT=-Wall -Wextra -pedantic
CFLAGS=-Wall -Wextra -pedantic -c
PROGRAM=ProjetTremplin
RM=rm
CP=cp
MV=mv
MKDIR=mkdir
RMFLAGS=-fv
LIBDIR=lib/


all: ${LIBDIR}maze.o ${LIBDIR}mazeResolution.o ${LIBDIR}queue.o ${LIBDIR}player.o ${LIBDIR}stack.o ${LIBDIR}display.o main.o
	${CC} main.o ${LIBDIR}maze.o ${LIBDIR}mazeResolution.o ${LIBDIR}queue.o ${LIBDIR}player.o ${LIBDIR}stack.o ${LIBDIR}display.o -o ${PROGRAM} -lncurses -lm

project: ${LIBDIR}labs.h ${LIBDIR}maze.o ${LIBDIR}queue.o ${LIBDIR}player.o ${LIBDIR}stack.o ${LIBDIR}display.o main.o generator
	${MKDIR} -p etu
	${LINK} -r ${LIBDIR}maze.o ${LIBDIR}queue.o ${LIBDIR}player.o ${LIBDIR}stack.o ${LIBDIR}display.o main.o -o etu/labs.o
	${CP} ${LIBDIR}labs.h etu/labs.h
	scripts/simplify.sh ${LIBDIR}mazeResolution.c etu/mazeResolution.c
	${CP} generator etu/generator
	${CP} labs/first_lab.fodaly etu/first_lab.fodaly

generator: generation/generator.c
	${CC} ${CFLAGSSHORT} generation/generator.c -o generator

spliter: generation/spliter.c
	${CC} ${CFLAGSSHORT} generation/spliter.c -o spliter

main.o: main.c
	${CC} ${CFLAGS} main.c

maze.o: ${LIBDIR}maze.c ${LIBDIR}maze.h
	${CC} ${CFLAGS} ${LIBDIR}maze.c

mazeResolution.o: ${LIBDIR}mazeResolution.c ${LIBDIR}mazeResolution.h
	${CC} ${CFLAGS} ${LIBDIR}mazeResolution.c

queue.o: ${LIBDIR}queue.c ${LIBDIR}queue.h
	${CC} ${CFLAGS} ${LIBDIR}queue.c

player.o: ${LIBDIR}player.c ${LIBDIR}player.h
	${CC} ${CFLAGS} ${LIBDIR}player.c

stack.o: ${LIBDIR}stack.c ${LIBDIR}stack.h
	${CC} ${CFLAGS} ${LIBDIR}stack.c

display.o: ${LIBDIR}display.c ${LIBDIR}display.h
	${CC} ${CFLAGS} ${LIBDIR}display.c

clean:
	${RM} ${RMFLAGS} ${PROGRAM}
	${RM} ${RMFLAGS} *.o
	${RM} ${RMFLAGS} ${LIBDIR}*.o
	${RM} ${RMFLAGS} generator
