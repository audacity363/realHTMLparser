CC = gcc

INCLUDE = -I./include/

OBJ = utils.o print.o

STRINGS = string.o 1dstring.o 2dstring.o 3dstring.o

lib: $(OBJ) $(STRINGS)
	ar -vcru libvar_management.a \
		./bin/*.o \
		./bin/string/*.o
	@echo "Done..."

%.o: ./src/%.c
	$(CC) -g -c $< $(INCLUDE) -o ./bin/$@

%.o: ./src/string/%.c
	$(CC) -g -c $< $(INCLUDE) -o ./bin/string/$@

test:
	$(CC) -g ./test_src/main.c $(INCLUDE) -L./ -lvar_management -o ./test_src/main
	./test_src/main

mem_test:
	valgrind \
		--leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		./test_src/main 1> valgrind.log 2>&1
	

clean:
	rm -rf ./bin/*.o
	rm -rf ./bin/string/*.o
	rm -rf ./libvar_management.a
