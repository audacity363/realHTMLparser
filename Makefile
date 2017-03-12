CC = gcc

OBJ = function_dummys.o parser.o token.o

C_FLAGS=

INCLUDE = ./include/

lib: $(OBJ)
	$(CC) -g $(C_FLAGS) -o main ./src/main.c -I$(INCLUDE) ./bin/*.o

debug_lib: C_FLAGS=-DDEBUG_FLAG
debug_lib: clean lib

%.o: ./src/%.c
	$(CC) -g3 -c $(C_FLAGS) $< -I$(INCLUDE) -o ./bin/$@

check_mem:
	valgrind \
	--leak-check=full \
	--show-leak-kinds=all \
	--track-origins=yes \
	./main 1> valgrind.log 2>&1

clean:
	rm -rf main
	rm -rf ./bin/*.o
