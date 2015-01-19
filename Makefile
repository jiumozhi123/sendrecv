#
# Makefile for sendrecv
#

all:server client fcreate fclean md5

server:server.c
	gcc server.c -o server -lpthread

client:client.c
	gcc client.c -o client -lpthread

fcreate:fcreate.c
	gcc fcreate.c -o fcreate

fclean:fclean.c
	gcc fclean.c -o fclean

md5:MD5.o MD5test.o
	gcc -o md5 MD5.o MD5test.o

MD5.o:MD5.c MD5.h
	gcc -c MD5.c

MD5test.o:MD5test.c MD5.h
	gcc -c MD5test.c

clean:
	rm -f server client fcreate fclean md5 MD5.o MD5test.o
