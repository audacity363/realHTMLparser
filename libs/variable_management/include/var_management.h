#ifndef RH4N_VARIABLE_MAN
#define RH4N_VARIABLE_MAN

#define NAME_COMPARE(cname, obj) strcmp(cname, obj->name) == 0

#define true  1
#define false 0

#define GROUP_NOT_DEFINED        1
#define VARIABLE_NOT_DEFINED     2
#define VARIABLE_ALREADY_DEFINED 3
#define MEMORY_ERROR             4
#define GROUP_ALREADY_DEFINED    5
#define X_INDEX_OUT_OF_RANGE     6
#define Y_INDEX_OUT_OF_RANGE     7
#define Z_INDEX_OUT_OF_RANGE     8
#define STRING_TO_LONG           9
#define WRONG_VAR_TYPE           10

#define RH4N_FLG_COPY  0x00000001 //The data entry is just a pointer to a other variable. Don't free it

#define SET_FLAG(target, flag) target |= flag
#define UNSET_FLAG(target, flag) target &= ~flags
#define ISSET_FLAG(target, flag) target & flags

#define PRINT_MODE_RAW    0
#define PRINT_MODE_JSON   1
#define PRINT_MODE_FORMAT 2

typedef struct variableobject {
    char *name;
    int type;

    void *data;

    int length;
    int array_length[3];
    int size;

    int flags;

    struct variableobject *next_lvl;
    struct variableobject *prev;
    struct variableobject *next;
} VariableObject;

#define SIZEOF_VARIABLEOBJ sizeof(VariableObject)

//XXX: Create a lookup table with the name and a pointer to the VariableObject?

typedef struct {

    int number_of_variables;
    VariableObject *variables;

} VariableAnker;

//Variable types
enum {
    GROUP,

    INTEGER,
    ONEDINTEGER,
    TWODINTEGER,
    THREEDINTEGER,

    STRING,
    ONEDSTRING,
    TWODSTRING,
    THREEDSTRING,

    BOOL,
    ONEDBOOL,
    TWODBOOL,
    THREEDBOOL,

    FLOAT,
    ONEDFLOAT,
    TWODFLOAT,
    THREEDFLOAT,
};

typedef unsigned short int bool;

//Defined in utils.c
extern int rh4n_errno;

#include "utils.h"

#include "strings.h"
#include "1dstring.h"
#include "2dstring.h"
#include "3dstring.h"

#include "integer.h"
#include "1dinteger.h"
#include "2dinteger.h"
#include "3dinteger.h"

#include "bool.h"
#include "1dbool.h"
#include "2dbool.h"
#include "3dbool.h"

#include "float.h"
#include "1dfloat.h"
#include "2dfloat.h"
#include "3dfloat.h"

#include "print.h"
#endif
