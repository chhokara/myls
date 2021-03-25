# build an executable named myls from myls.c
all: myls.c
	gcc -g -Wall -o myls myls.c

clean:
	$(RM) myls
