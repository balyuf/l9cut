# This is the Makefile for gcc

all : l9cut l9cutd l9pic

l9cut : l9cut.o
	gcc -o l9cut l9cut.o

l9cutd : l9cutd.o
	gcc -o l9cutd l9cutd.o

l9pic : l9pic.o
	gcc -o l9pic l9pic.o

l9cut.o : l9cut.c l9crc.h l9data_d.h l9data_p.h
	gcc -c -Wall -o l9cut.o l9cut.c

l9cutd.o : l9cut.c l9crc.h l9data_d.h l9data_p.h
	gcc -c -D DEBUG -Wall -o l9cutd.o l9cut.c

l9pic.o : l9pic.c
	gcc -c -o l9pic.o l9pic.c

clean :
	-rm l9cut l9cutd l9pic l9cut.o l9cutd.o l9pic.o
