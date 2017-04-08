CC = gcc

OBJ = function_dummys.o \
	  token.o \
	  parser.o \
	  parser_utils.o \
	  genjson.o \
	  dump_vars.o

VARIABLE = variable.o \
		   variable_attributes.o \
		   exec_attributes.o \
		   functions.o

VARS = utils.o

VARS_STRING = 1dstring.o

INCLUDE = -I./include \
		  -I./libs/variable_management/include/

LIBS = -L./libs/variable_management/ -lvar_management

lib: $(OBJ) 
	$(CC) -g -o main ./src/main.c $(INCLUDE) ./bin/*.o $(LIBS)

$(OBJ): $(VARIABLE)
	$(CC) -g -c ./src/$*.c $(INCLUDE) -o ./bin/$*.o

$(VARIABLE):
	$(CC) -g -c ./src/variable/$*.c $(INCLUDE) -o ./bin/$*.o

vars: $(VARS)
	@echo "Done..."

$(VARS): $(VARS_STRING)
	$(CC) -g -c ./libs/variable_management/src/$*.c $(INCLUDE) \
		-o ./libs/variable_management/bin/$*.o -Wa,-adhlns=/tmp/$*.lst

$(VARS_STRING):
	$(CC) -g -c ./libs/variable_management/src/string/$*.c $(INCLUDE) \
		-o ./libs/variable_management/bin/$*.o


clean: 
	rm -rf ./bin/*.o
	rm -rf ./main
