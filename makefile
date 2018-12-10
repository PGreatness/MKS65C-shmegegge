run: share.o
	gcc test.o
	./a.out

share.o: test.c
	gcc -c test.c

clean: a.out test.o
	@rm a.out *.o

bug: 
	@gcc -g test.c
	gdb a.out

