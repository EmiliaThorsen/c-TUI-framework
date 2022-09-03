cc = clang
cflags = -Wall -g

.PHONY: all run clean

all: main.c cTUIFramework.c cTUIFramework.h
	$(cc) -o cTUIFramework main.c cTUIFramework.c $(cflags)

run: all
	./cTUIFramework

clean:
	rm ./cTUIFramework
