all:
	gcc -g -c utils.c -o utils.o
	gcc -g -c integer.c -o integer.o
	gcc -g -c string.c -o string.o
	gcc -g -c bool.c -o bool.o
	gcc -g -c float.c -o float.o
	gcc -g -c print.c -o print.o
	ar -vcru libvars.a ./*.o

test:
	gcc -g ./test_src/main.c -o ./main -I./ -L./ -lvars
