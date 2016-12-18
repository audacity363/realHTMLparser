INCLUDES = -I./vars/
CARGS = -Wall -g

all:
	gcc -c $(CARGS) token_handling.c -o token_handling.o $(INCLUDES)
	gcc -c $(CARGS) command_parsing.c -o command_parsing.o $(INCLUDES)
	gcc -c $(CARGS) variablen_parsing.c -o variablen_parsing.o $(INCLUDES)
	gcc -c $(CARGS) if.c -o if.o $(INCLUDES)
	gcc -c $(CARGS) exec_if.c -o exec_if.o $(INCLUDES)
	gcc -c $(CARGS) for.c -o for.o $(INCLUDES)
	gcc -c $(CARGS) json.c -o json.o $(INCLUDES)
	gcc -g main.c -o main ./*.o $(INCLUDES) -L./vars -lvars

