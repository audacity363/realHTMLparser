all:
	xlc -c -g token_handling.c -o token_handling.o
	xlc -c -g command_parsing.c -o command_parsing.o
	xlc -c -g variablen_parsing.c -o variablen_parsing.o
	xlc -c -g if.c -o if.o
	xlc -c -g exec_if.c -o exec_if.o
	xlc -c -g for.c -o for.o
	xlc -g main.c -o main ./*.o

