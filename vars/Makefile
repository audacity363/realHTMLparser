all:
	xlc -g -c utils.c -o utils.o
	xlc -g -c integer.c -o integer.o
	xlc -g -c string.c -o string.o
	xlc -g -c bool.c -o bool.o
	xlc -g -c float.c -o float.o
	xlc -g -c print.c -o print.o
	ar -vcru libvars.a ./*.o

test:
	xlc -g ./test_src/main.c -o ./main -I./ -L./ -lvars
