# Makefile for pang

COMPILER_TYPE= gnu

CC= gcc

PROG= pang
SRCS= main.c vbe.c timer.c keyboard.c game.c bitmap.c graphics.c  BMPLoaded.c menu.c mouse.c uart.c  rtc.c rtc_asm.S

CCFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -llm -lsys 

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>