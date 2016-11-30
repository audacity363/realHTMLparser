all:
	gcc -c -g token_handling.c -o token_handling.o
	gcc -c -g command_parsing.c -o command_parsing.o
	gcc -c -g variablen_parsing.c -o variablen_parsing.o
	gcc -c -g if.c -o if.o
	gcc -c -g for.c -o for.o
	gcc -g main.c -o main ./*.o

