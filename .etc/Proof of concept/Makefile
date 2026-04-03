
CFLAGS = -std=c99 -Wall -pedantic -I./src
CC = clang
LINK = -shared -fPIC

all: bin/test
	bin/test

obj/helloWorld.o: src/helloWorld.c src/helloWorld.h
	$(CC) $(CFLAGS) -c src/helloWorld.c -o obj/helloWorld.o

obj/test.o: src/test.c
	$(CC) $(CFLAGS) -c src/test.c -o obj/test.o

bin/test: obj/helloWorld.o obj/test.o 
	$(CC) obj/helloWorld.o obj/test.o -o bin/test


cleaner:
	del .\obj\*.o 
	del .\bin\test