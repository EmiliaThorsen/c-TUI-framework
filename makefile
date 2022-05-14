CFLAGS = -Wall -g
OBJ = main.o cTUIframework.o keyStrokes.o

bld:
	mkdir build

%.o: %.c $.h
	$(CC) -c -o $@ $< $(CFLAGS)

build: ${OBJ}
	gcc -o cTuiTest ${OBJ} $(CFLAGS)

run: build
	./cTuiTest

clean:
	rm -rf *.o
	rm cTuiTest
