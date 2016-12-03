all:
	gcc -g -c utils.c -o utils.o
	gcc -g -c integer.c -o integer.o
	gcc -g -c print.c -o print.o
	ar -vcru libvars.a ./*.o

test:
	gcc -g ./test_src/main.c -o ./main -I./ -L./ -lvars
