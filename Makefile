INCLUDES = -I./vars/
all:
	gcc -c -g token_handling.c -o token_handling.o $(INCLUDES)
	gcc -c -g command_parsing.c -o command_parsing.o $(INCLUDES)
	gcc -c -g variablen_parsing.c -o variablen_parsing.o $(INCLUDES)
	gcc -c -g if.c -o if.o $(INCLUDES)
	gcc -c -g exec_if.c -o exec_if.o $(INCLUDES)
	gcc -c -g for.c -o for.o $(INCLUDES)
	gcc -g main.c -o main ./*.o $(INCLUDES) -L./vars -lvars

