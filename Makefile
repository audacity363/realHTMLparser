CC = gcc

OBJ = function_dummys.o \
	  token.o \
	  parser.o \
	  parser_utils.o \
	  genjson.o \
	  dump_vars.o \
	  typeof.o \
	  static_types.o

VARIABLE = variable.o \
		   variable_attributes.o \
		   exec_attributes.o \
		   functions.o

MACROS = macro_handling.o \
		 save_macro.o \
		 exec_macro.o

FOR = for_handling.o \
	  exec_for.o \
	  parse_for.o

VARS = utils.o

VARS_STRING = 1dstring.o

INCLUDE = -I./include \
		  -I./libs/variable_management/include/

LIBS = -L./libs/variable_management/ -lvar_management

lib: $(OBJ) 
	$(CC) -g -o main ./src/main.c $(INCLUDE) ./bin/*.o $(LIBS)
	@./main
	@echo "==========================================================================="
	@cat ./out.html
	@echo "==========================================================================="

$(OBJ): $(VARIABLE) $(MACROS) $(FOR)
	$(CC) -g -c ./src/$*.c $(INCLUDE) -o ./bin/$*.o

$(VARIABLE):
	$(CC) -g -c ./src/variable/$*.c $(INCLUDE) -o ./bin/$*.o

$(MACROS):
	$(CC) -g -c ./src/macros/$*.c $(INCLUDE) -o ./bin/$*.o

$(FOR):
	$(CC) -g -c ./src/for/$*.c $(INCLUDE) -o ./bin/$*.o

vars: $(VARS)
	@echo "Done..."

$(VARS): $(VARS_STRING)
	$(CC) -g -c ./libs/variable_management/src/$*.c $(INCLUDE) \
		-o ./libs/variable_management/bin/$*.o -Wa,-adhlns=/tmp/$*.lst

$(VARS_STRING):
	$(CC) -g -c ./libs/variable_management/src/string/$*.c $(INCLUDE) \
		-o ./libs/variable_management/bin/$*.o

mem_check:
	valgrind \
		--leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		./main 1> valgrind.log 2>&1

clean: 
	rm -rf ./bin/*.o
	rm -rf ./main
