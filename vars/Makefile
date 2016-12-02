all:
	xlc -g -c utils.c -o utils.o
	xlc -g -c integer.c -o integer.o
	xlc -g -c print.c -o print.o
	ar -vcru libvars.a ./*.o
