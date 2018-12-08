run: share.o
	gcc share.o
	./a.out

share.o: share.c
	gcc -c share.c

clean: a.out testfile share.o
	@rm a.out testfile