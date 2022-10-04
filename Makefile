all: hencode hdecode

hencode: hencode.o htable.o
	gcc -o hencode -ansi -Wall -pedantic -g hencode.o htable.o
hencode.o: hencode.c
	gcc -ansi -Wall -pedantic -g -c hencode.c
hdecode: hdecode.o htable.o
	gcc -o hdecode -ansi -Wall -pedantic hdecode.o htable.o
hdecode.o: hdecode.c
	gcc -ansi -Wall -pedantic -g -c hdecode.c
htable.o: htable.c htable.h
	gcc -ansi -Wall -pedantic -g -c htable.c
clean:
	rm *.o hencode
handin:
	scp hencode.c htable.c htable.h Makefile README jpino01@unix5.csc.calpoly.edu:./assign32
test:
	./hencode testfile testfile.huff 
