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
	  parse_for.o \
	  for_variables.o \
	  for_functions.o

VARS = utils.o

VARS_STRING = 1dstring.o

INCLUDE = -I./include \
		  -I./libs/variable_management/include/ \
		  -I./libs/jsmn

LIBS = -L./libs/variable_management/ -lvar_management

TESTOBJ = json.o json_array.o

all:
	@echo "targets:"
	@echo "\tparser"
	@echo "\tmem_check"
	@echo "\ttest"
	@echo "\truntests"
	@echo "\tdebug"

parser: $(OBJ) 
	$(CC) -g -o main ./src/main.c $(INCLUDE) ./bin/*.o $(LIBS)
	#@./main
	#@echo "==========================================================================="
	#@cat ./out.html
	#@echo "==========================================================================="

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

$(TESTOBJ):
	$(CC) -g -c ./tests/automatic_tests/src/$*.c $(INCLUDE) \
		-o ./tests/automatic_tests/bin/$*.o

test: $(TESTOBJ) $(OBJ)
	$(CC) -g ./tests/automatic_tests/src/tests.c -o ./tests/automatic_tests/parser_test \
		./bin/*.o ./tests/automatic_tests/bin/*.o $(INCLUDE) $(LIBS) \
			-L./libs/jsmn/ -ljsmn
	#./tests/automatic_tests/parser_test /tmp/test_in /tmp/test_out ./tests/automatic_tests/test_json 

runtests:
	@make test
	python3 ./tests/automatic_tests/main.py

debug:
	gdb ./main
	

clean: 
	rm -rf ./bin/*.o
	rm -rf ./main
	rm -rf ./tests/automatic_tests/bin/*.o
	rm -rf ./tests/automatic_tests/parser_test
