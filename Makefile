# this is the Make file for gcc (DJGPP)

all : l9cut l9cutd

l9cut : l9cut.o
	gcc -o l9cut l9cut.o

l9cutd : l9cutd.o
	gcc -o l9cutd l9cutd.o

l9cut.o : l9cut.c l9crc.h l9data_d.h l9data_p.h
	gcc -c -Wall -o l9cut.o l9cut.c

l9cutd.o : l9cut.c l9crc.h l9data_d.h l9data_p.h
	gcc -c -D DEBUG -Wall -o l9cutd.o l9cut.c

clean :
	-rm l9cut l9cutd l9cut.o l9cutd.o
